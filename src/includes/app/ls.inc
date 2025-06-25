DIR_ENTRY_BUFFER = $0400 ; Buffer to hold directory entry data

str_root_dir:
    .asciiz "/"
str_open_dir_err:
    .asciiz "Error opening directory"

;-------------------------------------------------------------------
; Name: do_ls
;-------------------------------------------------------------------
; Description: Lists files in a directory. Currently hardcoded to
;              list the root directory "/".
;-------------------------------------------------------------------
do_ls:
    ; This function uses API_FN_LIST_DIRECTORY. We assume it takes
    ; a pointer to a path string (params 0,1) and a pointer to a
    ; destination buffer (params 2,3) where it will write the
    ; complete, formatted directory listing.

    ; --- Step 1: Clear the destination buffer ---
    ldx #0
@clear_ls_buf:
    lda #0
    sta DIR_ENTRY_BUFFER, x
    inx
    cpx #INPUT_BUFFER_SIZE  ; Reusing this size constant for now
    bne @clear_ls_buf

    ; --- Step 2: Set up API parameters ---
    ; Param 0-1: Pointer to the directory path string
    lda #<str_root_dir
    sta API_PARAMETERS+0
    lda #>str_root_dir
    sta API_PARAMETERS+1
    ; Param 2-3: Pointer to the destination buffer
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+2
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+3

    ; --- Step 3: Issue the command ---
    lda #API_FN_LIST_DIRECTORY
    sta API_FUNCTION
@wait_ls_api_ready:
    lda API_COMMAND
    bne @wait_ls_api_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_ls_api_done:
    lda API_COMMAND
    bne @wait_ls_api_done

    ; --- Step 4: Print the result ---
    ; The result from the API should now be in DIR_ENTRY_BUFFER.
    lda #<DIR_ENTRY_BUFFER
    sta ZP_PTR_LO
    lda #>DIR_ENTRY_BUFFER
    sta ZP_PTR_HI
    jsr br_text_print_string
    ; No extra newline needed if the API provides a formatted list.
    
    rts
