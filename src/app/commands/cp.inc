; ===============================================================
; CP (Copy File) Command
;
; This file contains the command handler for the built-in 'cp'
; command.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_cp
;-------------------------------------------------------------------
; Description: Copies a source file to a destination file using the
;              dedicated API_FN_FILE_COPY function. It expects two
;              arguments, both of which are converted to the
;              length-prefixed string format required by the API.
; Inputs: X = index in INPUT_BUFFER where arguments start.
; Returns: None
; Destroys: a, x, y
;-------------------------------------------------------------------
do_cp:
    ; --- Step 1: Get source filename ---
    ; Point to the first buffer (DIR_ENTRY_BUFFER) and call the helper.
    lda #<DIR_ENTRY_BUFFER
    sta ZP_PTR_LO
    lda #>DIR_ENTRY_BUFFER
    sta ZP_PTR_HI
    jsr get_next_argument_lp
    bcs @cp_error_exit ; Branch if carry is set (error/no argument)

    ; --- Step 2: Get destination filename ---
    ; Point to the second buffer (FILENAME_BUFFER2) and call the helper.
    ; The helper will continue from where it left off in the INPUT_BUFFER.
    lda #<FILENAME_BUFFER2
    sta ZP_PTR_LO
    lda #>FILENAME_BUFFER2
    sta ZP_PTR_HI
    jsr get_next_argument_lp
    bcs @cp_error_exit ; Branch if carry is set (error/no argument)

    ; --- Step 3: Set up API parameters and call the copy function ---
    ; Param 0-1: Pointer to the length-prefixed source filename
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+0
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    ; Param 2-3: Pointer to the length-prefixed destination filename
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
    
    ; The command handler is finished.
    rts

@cp_error_exit:
    ; Print an error if arguments are missing or invalid.
    lda #<str_copy_err
    sta ZP_PTR_LO
    lda #>str_copy_err
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts
