; ===============================================================
; Clear Command
;
; This file contains the command handler for the built-in 'clear'
; command.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_clear
;-------------------------------------------------------------------
; Description: Clears the screen by calling the relevant API
;              function.
; Inputs: None
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
do_clear:
    lda #API_FN_CLEAR_SCREEN
    sta API_FUNCTION
@wait_cls_api_ready:
    lda API_COMMAND
    bne @wait_cls_api_ready

    lda #API_GROUP_CONSOLE
    sta API_COMMAND
@wait_cls_api_done:
    lda API_COMMAND
    bne @wait_cls_api_done
    rts
