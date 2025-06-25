do_cd:
    ; This logic is identical to do_mkdir, but calls a different API function.
@skip_cd_spaces:
    lda INPUT_BUFFER, x
    beq @cd_done     ; No argument provided
    cmp #' '
    bne @cd_arg_found
    inx
    jmp @skip_cd_spaces

@cd_arg_found:
    stx ZP_TEMP             ; Save start index of the argument.
    ldy #0                  ; Y will be our length counter
@find_cd_arg_end:
    lda INPUT_BUFFER, x
    cmp #' '
    beq @cd_arg_len_found
    cmp #0
    beq @cd_arg_len_found
    iny                     ; Increment length
    inx                     ; Move to next character
    jmp @find_cd_arg_end

@cd_arg_len_found:
    sty DIR_ENTRY_BUFFER    ; Store length as the first byte.
    ldx ZP_TEMP             ; Restore start index of arg
    ldy #1                  ; Y is the destination index in DIR_ENTRY_BUFFER
@copy_cd_loop:
    lda INPUT_BUFFER, x
    cmp #' '
    beq @copy_cd_done
    cmp #0
    beq @copy_cd_done
    sta DIR_ENTRY_BUFFER, y
    inx
    iny
    jmp @copy_cd_loop

@copy_cd_done:
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+0
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    
    lda #API_FN_DIR_CHDIR
    sta API_FUNCTION
@wait_cd_api_ready:
    lda API_COMMAND
    bne @wait_cd_api_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cd_api_done:
    lda API_COMMAND
    bne @wait_cd_api_done

@cd_done:
    rts