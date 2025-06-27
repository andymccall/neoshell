; ===============================================================
; User Input Handler
;
; This file contains the main routine for reading a line of user
; input from the keyboard, handling backspace, and storing the
; result in the INPUT_BUFFER.
; ===============================================================

;-------------------------------------------------------------------
; Name: get_input
;-------------------------------------------------------------------
; Description: Reads a line of text from the user, echoing each
;              character back to the console. Handles backspace.
;              Input is stored in INPUT_BUFFER and is terminated
;              when the user presses Enter ($0D). The buffer will
;              be null-terminated upon completion.
; Inputs: None
; Returns: None
; Destroys: a, x
;-------------------------------------------------------------------
get_input:
    ; --- First, clear the input buffer before accepting new input ---
    ldx #0
@clear_input_loop:
    lda #0
    sta INPUT_BUFFER, x
    inx
    cpx #INPUT_BUFFER_SIZE
    bne @clear_input_loop
    ldx #0                  ; Reset X to 0 for reading input.

@read_loop:
    ; --- Call API to read a single character ---
@wait_get_api_ready:
    lda API_COMMAND
    bne @wait_get_api_ready
    lda #API_FN_READ_CHAR
    sta API_FUNCTION
    lda #API_GROUP_CONSOLE
    sta API_COMMAND
@wait_get_api_done:
    lda API_COMMAND
    bne @wait_get_api_done
    
    lda API_PARAMETERS + 0
    beq @read_loop          ; If API returned 0, no key was pressed. Loop.

    ; --- Check for special characters ---
    cmp #$0D                ; Is it Enter?
    beq @input_finished     ; If yes, we're done.
    
    cmp #$08                ; Is it Backspace (ASCII $08)?
    beq @handle_backspace   ; If yes, handle it.

    ; --- Process normal character ---
    ; First, check if there is space in the buffer.
    cpx #INPUT_BUFFER_SIZE - 1 ; Leave room for null terminator
    bcs @read_loop             ; If buffer is full, ignore character.
    
    pha                     ; Push Accumulator onto the stack
    jsr br_text_print_char
    pla                     ; Pull Accumulator from the stack

    ; Store the character in our buffer and advance the index
    sta INPUT_BUFFER, x
    inx
    jmp @read_loop          ; Go back for the next character

@handle_backspace:
    cpx #0                  ; Is the buffer already empty?
    beq @read_loop          ; If yes, can't backspace. Ignore and get next char.
    
    dex                     ; Decrement buffer index to effectively delete char
    
    ; Erase character on screen by printing backspace, space, backspace
    lda #$08
    jsr br_text_print_char
    lda #' '
    jsr br_text_print_char
    lda #$08
    jsr br_text_print_char
    
    jmp @read_loop          ; Go back to get the next character

@input_finished:
    ; --- Add a null terminator to the end of the string ---
    lda #0
    sta INPUT_BUFFER, x

    ; --- Print a final newline to move the cursor down ---
    jsr br_text_print_new_line
    
    rts
