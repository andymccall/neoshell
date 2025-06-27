; ===============================================================
; LS (List Directory) Command
;
; This file contains the command handler for the built-in 'ls'
; command.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_ls
;-------------------------------------------------------------------
; Description: Lists files in the current directory. This version
;              uses the high-level API_FN_LIST_DIRECTORY function to
;              avoid a bug in the emulator's low-level directory
;              read implementation. The -l flag is temporarily
;              disabled for stability.
; Inputs: X = index in INPUT_BUFFER where arguments start.
; Returns: None
; Destroys: a, x
;-------------------------------------------------------------------
do_ls:
    ; --- Step 1: Clear the destination buffer ---
    ldx #0
@clear_ls_buf:
    lda #0
    sta DIR_ENTRY_BUFFER, x
    inx
    cpx #INPUT_BUFFER_SIZE
    bne @clear_ls_buf

    ; --- Step 2: Set up API parameters ---
    ; We'll list the current directory, ".".
    ; Param 0-1: Pointer to the directory path string
    ; Param 2-3: Pointer to the destination buffer
    lda #<str_dot
    sta API_PARAMETERS+0
    lda #>str_dot
    sta API_PARAMETERS+1
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
    
    rts

; --- Data for ls command ---
str_dot:
    .asciiz "."
