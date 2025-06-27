; ===============================================================
; CWD (Current Working Directory) Command
;
; This file contains the command handler for the built-in 'cwd'
; command.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_cwd
;-------------------------------------------------------------------
; Description: Gets the current working directory and prints it.
; Inputs: None
; Returns: None
; Destroys: a, x
;-------------------------------------------------------------------
do_cwd:
    ; --- Clear the buffer before calling the API ---
    ldx #0
@clear_cwd_buf:
    lda #0
    sta INPUT_BUFFER, x
    inx
    cpx #INPUT_BUFFER_SIZE
    bne @clear_cwd_buf

    ; --- Set up API parameters ---
    ; Param 0-1: Pointer to buffer
    ; Param 2: Size of buffer
    lda #<INPUT_BUFFER
    sta API_PARAMETERS+0
    lda #>INPUT_BUFFER
    sta API_PARAMETERS+1
    lda #INPUT_BUFFER_SIZE
    sta API_PARAMETERS+2

    ; --- Issue the command ---
    lda #API_FN_DIR_CWD
    sta API_FUNCTION
@wait_cwd_api_ready:
    lda API_COMMAND
    bne @wait_cwd_api_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cwd_api_done:
    lda API_COMMAND
    bne @wait_cwd_api_done
    
    ; --- Print the result ---
    ; The API has now written the CWD string into INPUT_BUFFER.
    lda #<INPUT_BUFFER
    sta ZP_PTR_LO
    lda #>INPUT_BUFFER
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts
