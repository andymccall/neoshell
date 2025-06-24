;-------------------------------------------------------------------
; Name: br_screen_clear
;-------------------------------------------------------------------
; Description: Clears the screen by calling the relevant API
;              function.
; Note: 
; URL:
;-------------------------------------------------------------------
; Inputs: None
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
br_screen_clear:
    lda #API_FN_CLEAR_SCREEN
    sta API_FUNCTION

; Wait for any previous API command to finish
@wait_api_ready:
    lda API_COMMAND
    bne @wait_api_ready

    ; Issue the clear screen command
    lda #API_GROUP_CONSOLE
    sta API_COMMAND

; Wait for OUR clear screen command to finish
@wait_api_done:
    lda API_COMMAND
    bne @wait_api_done
    
    rts
