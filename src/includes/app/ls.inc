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
    ; --- Step 1: Open the directory ---
    ; Set up API parameters for API_FN_DIR_OPEN
    ; Param 0-1: Pointer to the directory path string
    lda #<str_root_dir
    sta API_PARAMETERS+0
    lda #>str_root_dir
    sta API_PARAMETERS+1
    
    ; Issue the command
    lda #API_FN_DIR_OPEN
    sta API_FUNCTION
@wait_ls_open_ready:
    lda API_COMMAND
    bne @wait_ls_open_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_ls_open_done:
    lda API_COMMAND
    bne @wait_ls_open_done
    
    ; The directory handle is now in API_PARAMETERS+0.
    ; A value of 0 means an error occurred.
    lda API_PARAMETERS+0
    beq @ls_error
    
    sta ZP_TEMP             ; Store the handle in our temp variable

    ; --- Step 2: Read directory entries in a loop ---
@read_dir_loop:
    ; Set up API parameters for API_FN_DIR_READ
    ; Param 0: The directory handle
    ; Param 1-2: Pointer to the buffer to receive entry data
    lda ZP_TEMP
    sta API_PARAMETERS+0
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+2

    ; Issue the command
    lda #API_FN_DIR_READ
    sta API_FUNCTION
@wait_ls_read_ready:
    lda API_COMMAND
    bne @wait_ls_read_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_ls_read_done:
    lda API_COMMAND
    bne @wait_ls_read_done

    ; The result is in API_PARAMETERS+0. 0 means end of directory.
    lda API_PARAMETERS+0
    beq @ls_close           ; If 0, no more files, so close the dir.
    
    ; The filename is a null-terminated string at the start of DIR_ENTRY_BUFFER
    lda #<DIR_ENTRY_BUFFER
    sta ZP_PTR_LO
    lda #>DIR_ENTRY_BUFFER
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line

    jmp @read_dir_loop      ; Go back to read the next entry.

@ls_error:
    ; Print an error message
    lda #<str_open_dir_err
    sta ZP_PTR_LO
    lda #>str_open_dir_err
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line
    ; Fall through to close, which is safe even with a null handle.

@ls_close:
    ; --- Step 3: Close the directory ---
    lda ZP_TEMP             ; The handle is still in ZP_TEMP
    sta API_PARAMETERS+0
    
    lda #API_FN_DIR_CLOSE
    sta API_FUNCTION
@wait_ls_close_ready:
    lda API_COMMAND
    bne @wait_ls_close_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_ls_close_done:
    lda API_COMMAND
    bne @wait_ls_close_done
    
    rts