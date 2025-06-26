ZP_FILE_HANDLE  = $F7   ; A dedicated ZP location for the cat file handle
ZP_BYTE_COUNT   = $F8   ; A dedicated ZP location for the cat byte count

str_file_found:
    .asciiz " found"
str_file_not_found:
    .asciiz " not found"
str_debug_cat:
    .asciiz "File opened, attempting to read..."
str_cat_debug:
    .asciiz "Trying to cat file: "

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
    ; Param 0: File channel to use (e.g., channel 1)
    ; Param 1,2: Pointer to length-prefixed filename
    ; Param 3: Mode (0 = read-only)
    lda #1                      ; Use file channel 1
    sta API_PARAMETERS+0
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+2
    lda #0                      ; Mode 0 = read-only
    sta API_PARAMETERS+3

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

    ; The API returns the handle in API_PARAMETERS+0 if successful.
    lda API_PARAMETERS+0
    cmp #1
    bne @cat_error
    
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
    lda #$FF
    sta API_PARAMETERS+3
    lda #$00
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
    
    lda API_PARAMETERS+3
    beq @close_cat_file
    
    sta ZP_BYTE_COUNT
    
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
    ; --- Step 4: Close the file ---
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
    
    jsr br_text_print_new_line
    
@cat_done:
    rts