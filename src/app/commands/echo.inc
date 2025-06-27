; ===============================================================
; Echo Command
;
; This file contains the command handler for the built-in 'echo'
; command.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_echo
;-------------------------------------------------------------------
; Description: Prints the arguments that follow the "echo" command.
; Inputs: X = index in INPUT_BUFFER where arguments start.
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
do_echo:
    ; --- Step 1: Find the start of the arguments ---
@skip_echo_spaces:
    lda INPUT_BUFFER, x
    cmp #' '
    bne @args_found
    inx
    jmp @skip_echo_spaces

@args_found:
    ; --- Step 2: Set up a pointer to the arguments and print ---
    txa
    clc
    adc #<INPUT_BUFFER
    sta ZP_PTR_LO
    lda #>INPUT_BUFFER
    adc #0
    sta ZP_PTR_HI
    
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts
