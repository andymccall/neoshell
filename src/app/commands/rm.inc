; ===============================================================
; RM (Remove File) Command
;
; This file contains the command handler for the built-in 'rm'
; command.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_rm
;-------------------------------------------------------------------
; Description: Deletes a file or an empty directory. It parses the
;              first argument as the filename and uses the API to
;              delete it. It now checks for an error condition and
;              reports it to the user.
; Inputs: X = index in INPUT_BUFFER where arguments start.
; Returns: None
; Destroys: a, x, y
;-------------------------------------------------------------------
do_rm:
    ; Use the helper function to get the first argument and create
    ; a length-prefixed string in DIR_ENTRY_BUFFER.
    lda #<DIR_ENTRY_BUFFER
    sta ZP_PTR_LO
    lda #>DIR_ENTRY_BUFFER
    sta ZP_PTR_HI
    jsr get_next_argument_lp
    bcc @rm_has_arg
    rts ; No argument was given, so just return.

@rm_has_arg:
    ; The length-prefixed string is now in DIR_ENTRY_BUFFER.
    ; Set up the API parameters and call the function.
    ; Parameters:0,1 points to the length-prefixed filename string.
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
    
    ; --- Check for an error condition ---
    ; The API returns its main status code (0 for success) in API_ERROR ($FF02).
    lda API_ERROR
    beq @rm_done            ; If zero, success, so we're done.

    ; --- Error occurred, print message ---
    lda #<str_rm_err
    sta ZP_PTR_LO
    lda #>str_rm_err
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line

@rm_done:
    rts
