; ===============================================================
; Argument Parsing Library
;
; This file contains helper subroutines for parsing arguments
; from the main INPUT_BUFFER.
; ===============================================================

;-------------------------------------------------------------------
; Name: get_next_argument_lp
;-------------------------------------------------------------------
; Description: Finds the next space-separated argument in the
;              INPUT_BUFFER and creates a length-prefixed version
;              of it in a destination buffer. This is required for
;              file system API calls. The X register is updated to
;              point to the character after the parsed argument,
;              allowing for sequential calls to get multiple arguments.
;
; Inputs:      X = The index in INPUT_BUFFER to start searching from.
;              ZP_PTR_LO/HI = Pointer to the destination buffer
;                             where the length-prefixed string will be built.
;
; Returns:     Carry Flag: clear on success, set on failure (no more args).
;              X register is updated to point after the found argument.
;
; Destroys:    a, y
;-------------------------------------------------------------------
get_next_argument_lp:
    ; --- Step 1: Skip leading spaces to find the argument start ---
@skip_arg_spaces:
    lda INPUT_BUFFER, x
    beq @no_more_args       ; Reached end of the line
    cmp #' '
    bne @arg_start
    inx
    jmp @skip_arg_spaces

@arg_start:
    ; --- Step 2: Found an argument. Save its start position. ---
    stx ZP_TOKEN_START

    ; --- Step 3: Find the end of the argument to calculate its length ---
    ldy #0                  ; Y will be our length counter
@find_arg_end:
    lda INPUT_BUFFER, x
    beq @arg_len_found      ; Reached end of the line
    cmp #' '
    beq @arg_len_found      ; Reached end of the argument
    iny                     ; Increment length counter
    inx                     ; Move to next character
    jmp @find_arg_end

@arg_len_found:
    phy                     ; Save the length (in Y) on the stack

    ; --- Step 4: Build the length-prefixed string ---
    ; Get the pointer to the destination buffer (passed in ZP_PTR_LO/HI)
    lda ZP_PTR_LO
    sta ZP_PTR2_LO
    lda ZP_PTR_HI
    sta ZP_PTR2_HI
    
    ; Store the length as the very first byte of the destination buffer
    tya
    sta (ZP_PTR2_LO)
    
    ; Copy the argument string from INPUT_BUFFER to the destination
    ldx ZP_TOKEN_START      ; Go back to the start of the argument in the input
    ldy #1                  ; Start copying to the byte after the length prefix
@copy_arg_loop:
    lda INPUT_BUFFER, x
    cmp #' '
    beq @copy_arg_done
    cmp #0
    beq @copy_arg_done
    sta (ZP_PTR2_LO),y
    inx
    iny
    jmp @copy_arg_loop

@copy_arg_done:
    ply                     ; Restore the length into Y (just to balance the stack)
    clc                     ; Return with carry clear to indicate success
    rts

@no_more_args:
    sec                     ; Return with carry set to indicate failure
    rts
