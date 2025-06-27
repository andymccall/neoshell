; ===============================================================
; CD (Change Directory) Command
;
; This file contains the command handler for the built-in 'cd'
; command.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_cd
;-------------------------------------------------------------------
; Description: Changes the current directory. It parses the first
;              argument as the directory name and uses the API to
;              change to it. It expects the API to use a
;              length-prefixed string.
; Inputs: X = index in INPUT_BUFFER where arguments start.
; Returns: None
; Destroys: a, x, y
;-------------------------------------------------------------------
do_cd:
    ; Use the helper function to get the first argument and create
    ; a length-prefixed string in DIR_ENTRY_BUFFER.
    lda #<DIR_ENTRY_BUFFER
    sta ZP_PTR_LO
    lda #>DIR_ENTRY_BUFFER
    sta ZP_PTR_HI
    jsr get_next_argument_lp
    bcc @cd_has_arg
    rts ; No argument was given, so just return.

@cd_has_arg:
    ; The length-prefixed string is now in DIR_ENTRY_BUFFER.
    ; Set up the API parameters and call the function.
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+0
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    
    lda #API_FN_DIR_CHDIR
    sta API_FUNCTION
@wait_cd_api_ready:
    lda API_COMMAND
    bne @wait_cd_api_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cd_api_done:
    lda API_COMMAND
    bne @wait_cd_api_done
    
    rts
