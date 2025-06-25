do_mkdir:
    ; --- Step 1: Find the start of the directory name argument ---
@skip_mkdir_spaces:
    lda INPUT_BUFFER, x
    beq @mkdir_done     ; No argument provided
    cmp #' '
    bne @mkdir_arg_found
    inx
    jmp @skip_mkdir_spaces

@mkdir_arg_found:
    ; X now points to the start of the directory name.
    stx ZP_TEMP             ; Save start index of the argument.

    ; --- Step 2: Find the end of the argument & calculate length ---
    ldy #0                  ; Y will be our length counter
@find_mkdir_arg_end:
    lda INPUT_BUFFER, x
    cmp #' '
    beq @mkdir_arg_len_found
    cmp #0
    beq @mkdir_arg_len_found
    iny                     ; Increment length
    inx                     ; Move to next character
    jmp @find_mkdir_arg_end

@mkdir_arg_len_found:
    ; Length is in Y. Start index is in ZP_TEMP.
    
    ; --- Step 3: Build the length-prefixed string ---
    ; We'll use DIR_ENTRY_BUFFER for this.
    sty DIR_ENTRY_BUFFER    ; Store length as the first byte.

    ; Copy the name itself into the buffer after the length byte
    ldx ZP_TEMP             ; Restore start index of arg
    ldy #1                  ; Y is the destination index in DIR_ENTRY_BUFFER
@copy_loop:
    lda INPUT_BUFFER, x
    cmp #' '
    beq @copy_done
    cmp #0
    beq @copy_done
    sta DIR_ENTRY_BUFFER, y
    inx
    iny
    jmp @copy_loop

@copy_done:
    ; --- Step 4: Set up API parameters ---
    ; Param 0-1: Pointer to the new length-prefixed string
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+0
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    
    ; --- Step 5: Issue the command ---
    lda #API_FN_DIR_MKDIR
    sta API_FUNCTION
@wait_mkdir_api_ready:
    lda API_COMMAND
    bne @wait_mkdir_api_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_mkdir_api_done:
    lda API_COMMAND
    bne @wait_mkdir_api_done

@mkdir_done:
    rts