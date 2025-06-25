ZP_FILE_HANDLE  = $F7   ; A dedicated ZP location for the cat file handle
ZP_BYTE_COUNT   = $F8   ; A dedicated ZP location for the cat byte count

str_file_found:
    .asciiz " found"
str_file_not_found:
    .asciiz " not found"

do_cat:
    ; --- Step 1: Get the filename argument ---
@skip_cat_spaces:
    lda INPUT_BUFFER, x
    bne @cat_has_filename
    jmp @cat_done

@cat_has_filename:
    cmp #' '
    bne @cat_arg_found
    inx
    jmp @skip_cat_spaces

@cat_arg_found:
    ; The filename starts at index X. We need to create a
    ; length-prefixed string for the file open API.
    stx ZP_TOKEN_START
    ldy #0
@find_cat_arg_end:
    cpx #INPUT_BUFFER_SIZE
    beq @cat_arg_len_found

    lda INPUT_BUFFER, x
    cmp #' '
    beq @cat_arg_len_found
    cmp #0
    beq @cat_arg_len_found
    iny
    inx
    jmp @find_cat_arg_end

@cat_arg_len_found:
    sty DIR_ENTRY_BUFFER
    ldx ZP_TOKEN_START
    ldy #1
@copy_cat_loop:
    lda INPUT_BUFFER, x
    cmp #' '
    beq @copy_cat_done
    cmp #0
    beq @copy_cat_done
    sta DIR_ENTRY_BUFFER, y
    inx
    iny
    jmp @copy_cat_loop

@copy_cat_done:
    ; --- Step 2: Open the file ---
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+0
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    lda #1
    sta API_PARAMETERS+2

    lda #API_FN_FILE_OPEN
    sta API_FUNCTION
@wait_cat_open_ready:
    lda API_COMMAND
    bne @wait_cat_open_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cat_open_done:
    lda API_COMMAND
    bne @wait_cat_open_done

    lda API_PARAMETERS+0
    beq @cat_error
    sta ZP_FILE_HANDLE

    ; --- Step 3: Read from file in a loop ---
@cat_read_loop:
    ; Param 0: File handle
    lda ZP_FILE_HANDLE
    sta API_PARAMETERS+0
    ; Param 1-2: Pointer to buffer
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+2
    ; Param 3,4: Number of bytes to read (16-bit)
    lda #$FF                ; Read up to 255 bytes (low byte)
    sta API_PARAMETERS+3
    lda #$00                ; (high byte)
    sta API_PARAMETERS+4
    
    lda #API_FN_FILE_READ
    sta API_FUNCTION
@wait_cat_read_ready:
    lda API_COMMAND
    bne @wait_cat_read_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cat_read_done:
    lda API_COMMAND
    bne @wait_cat_read_done
    
    ; Number of bytes actually read is in API_PARAMETERS+3,4
    ; We only need to check the low byte since we read < 256 bytes.
    lda API_PARAMETERS+3
    beq @close_cat_file     ; If 0, we've reached the end of the file
    
    sta ZP_BYTE_COUNT
    
    ; Print the bytes we just read
    ldy #0
@cat_print_loop:
    cpy ZP_BYTE_COUNT
    beq @cat_read_loop
    lda DIR_ENTRY_BUFFER, y
    jsr br_text_print_char
    iny
    jmp @cat_print_loop
    
@cat_error:
    lda #<str_file_open_err
    sta ZP_PTR_LO
    lda #>str_file_open_err
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts

@close_cat_file:
    lda ZP_FILE_HANDLE
    sta API_PARAMETERS+0
    lda #API_FN_FILE_CLOSE
    sta API_FUNCTION
@wait_cat_close_ready:
    lda API_COMMAND
    bne @wait_cat_close_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cat_close_done:
    lda API_COMMAND
    bne @wait_cat_close_done
    
@cat_done:
    rts