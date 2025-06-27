; ===============================================================
; MV (Move File) Command
;
; This file contains the command handler for the built-in 'mv'
; command.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_mv
;-------------------------------------------------------------------
; Description: Moves a source file to a destination. This is
;              implemented as a copy followed by a delete.
; Inputs: X = index in INPUT_BUFFER where arguments start.
; Returns: None
; Destroys: a, x, y
;-------------------------------------------------------------------
do_mv:
    ; --- Step 1: Get the source filename ---
    ; Use the helper function to get the first argument and create
    ; a length-prefixed string in DIR_ENTRY_BUFFER.
    lda #<DIR_ENTRY_BUFFER
    sta ZP_PTR_LO
    lda #>DIR_ENTRY_BUFFER
    sta ZP_PTR_HI
    jsr get_next_argument_lp
    bcs @mv_error_exit ; Branch if carry is set (error/no argument)

    ; --- Step 2: Get the destination filename ---
    ; Use the helper function again for the second argument, placing
    ; it in FILENAME_BUFFER2.
    lda #<FILENAME_BUFFER2
    sta ZP_PTR_LO
    lda #>FILENAME_BUFFER2
    sta ZP_PTR_HI
    jsr get_next_argument_lp
    bcs @mv_error_exit ; Branch if carry is set (error/no argument)

    ; --- Step 3: Call the copy file API ---
    ; Set up parameters to point to the two filename buffers.
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
    
    ; Check for an error on the copy operation.
    lda API_ERROR
    bne @mv_error_exit

    ; --- Step 4: If copy was successful, delete the original file ---
    ; The source filename is still in DIR_ENTRY_BUFFER.
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+0
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    
    lda #API_FN_FILE_DELETE
    sta API_FUNCTION
@wait_rm_api_ready:
    lda API_COMMAND
    bne @wait_rm_api_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_rm_api_done:
    lda API_COMMAND
    bne @wait_rm_api_done
    
    ; Check for an error on the delete operation.
    lda API_ERROR
    bne @mv_error_exit

    ; --- Success ---
    rts

@mv_error_exit:
    ; Print a generic error message for now.
    lda #<str_mv_err
    sta ZP_PTR_LO
    lda #>str_mv_err
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts
