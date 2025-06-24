do_cwd:
    ; --- Clear the buffer before calling the API ---
    ldx #0
@clear_cwd_buf:
    lda #0
    sta INPUT_BUFFER, x
    inx
    cpx #INPUT_BUFFER_SIZE
    bne @clear_cwd_buf

    ; Param 0-1: Pointer to buffer
    lda #<INPUT_BUFFER
    sta API_PARAMETERS+0
    lda #>INPUT_BUFFER
    sta API_PARAMETERS+1
    ; Param 2: Size of buffer
    lda #INPUT_BUFFER_SIZE
    sta API_PARAMETERS+2

    ; Issue the command
    lda #API_FN_DIR_CWD ; Use the correct API constant
    sta API_FUNCTION
@wait_cwd_api_ready:
    lda API_COMMAND
    bne @wait_cwd_api_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cwd_api_done:
    lda API_COMMAND
    bne @wait_cwd_api_done
    
    ; --- Defensive Programming: Manually null-terminate the string ---
    ; Find the end of the string returned by the API.
    ldx #0
@find_null:
    lda INPUT_BUFFER, x
    beq @found_null
    inx
    cpx #INPUT_BUFFER_SIZE - 1 ; Prevent running off the end
    bne @find_null

@found_null:
    ; X now holds the index of the null terminator (or the end of the buffer).
    ; We'll place another null right after it, just in case.
    inx
    lda #0
    sta INPUT_BUFFER, x

    ; Now that we have a guaranteed-safe string, print it.
    lda #<INPUT_BUFFER
    sta ZP_PTR_LO
    lda #>INPUT_BUFFER
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts