; ===============================================================
; Definitions
; ===============================================================
INPUT_BUFFER    = $0300 ; Start address of our input buffer in RAM
INPUT_BUFFER_SIZE = 255 ; Size of the input buffer

;-------------------------------------------------------------------
; Name: get_input
;-------------------------------------------------------------------
; Description: Reads a line of text from the user, echoing each
;              character back to the console. Input is stored in
;              INPUT_BUFFER and is terminated when the user
;              presses Enter ($0D). The buffer will be
;              null-terminated upon completion.
;-------------------------------------------------------------------
; Inputs: None
; Returns: None
; Destroys: a, x
;-------------------------------------------------------------------
get_input:
    ldx #0                  ; Use X as the index for our input buffer.
@clear_loop:
    lda #0
    sta INPUT_BUFFER, x
    inx
    cpx #INPUT_BUFFER_SIZE
    bne @clear_loop

    ldx #0                  ; Reset X to 0 for reading input.


@read_loop:
    ; --- Call API to read a single character ---
    ; This is a standard, safe API call pattern.
    @wait_api_ready:
        lda API_COMMAND
        bne @wait_api_ready

    lda #API_FN_READ_CHAR
    sta API_FUNCTION
    lda #API_GROUP_CONSOLE
    sta API_COMMAND

    @wait_api_done:
        lda API_COMMAND
        bne @wait_api_done

    ; The character is now ready in the API parameters.
    lda API_PARAMETERS + 0
    
    ; --- Check if a key was actually pressed ---
    ; If the API returns 0, it means no key was available.
    ; We must loop back and poll again until we get a character.
    beq @read_loop

    ; --- Check if the user pressed Enter ---
    cmp #$0D                ; Compare character with Carriage Return
    beq @input_finished     ; If it's Enter, we're done.

    ; --- Echo the valid character back to the screen ---
    ; FIX: We must save the character in 'A' before calling a
    ; subroutine that might change it.
    pha                     ; Push Accumulator onto the stack
    jsr br_text_print_char
    pla                     ; Pull Accumulator from the stack

    ; --- Store the character in our buffer and advance the index ---
    sta INPUT_BUFFER, x
    inx
    cpx #255                ; Prevent buffer overflow
    bne @read_loop          ; If not full, loop back for the next character

@input_finished:
    ; --- Add a null terminator to the end of the string ---
    lda #0
    sta INPUT_BUFFER, x

    ; --- Print a final newline to move the cursor down ---
    jsr br_text_print_new_line
    
    rts
