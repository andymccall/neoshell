FILENAME_BUFFER2 = $0500 ; A second buffer for the destination filename in 'cp'

str_copy_err:
    .asciiz "Error copying file"

do_cp:
    ; --- Step 1: Get source filename ---
    lda #<DIR_ENTRY_BUFFER
    sta ZP_PTR_LO
    lda #>DIR_ENTRY_BUFFER
    sta ZP_PTR_HI
    jsr get_next_argument_lp
    bcs @cp_error_exit ; No source file given

    ; --- Step 2: Get destination filename ---
    lda #<FILENAME_BUFFER2
    sta ZP_PTR_LO
    lda #>FILENAME_BUFFER2
    sta ZP_PTR_HI
    jsr get_next_argument_lp
    bcs @cp_error_exit ; No destination file given

    ; --- Step 3: Call the copy file API ---
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+0
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    lda #<FILENAME_BUFFER2
    sta API_PARAMETERS+2
    lda #>FILENAME_BUFFER2
    sta API_PARAMETERS+3
    lda #API_FN_FILE_COPY
    sta API_FUNCTION
@wait_cp_api_ready:
    lda API_COMMAND
    bne @wait_cp_api_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cp_api_done:
    lda API_COMMAND
    bne @wait_cp_api_done
    rts

@cp_error_exit:
    lda #<str_copy_err
    sta ZP_PTR_LO
    lda #>str_copy_err
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts