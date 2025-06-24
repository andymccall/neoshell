do_clear_screen:
    lda #API_FN_CLEAR_SCREEN
    sta API_FUNCTION
@wait_clear_api_ready:
    lda API_COMMAND
    bne @wait_clear_api_ready

    lda #API_GROUP_CONSOLE
    sta API_COMMAND
@wait_clear_api_done:
    lda API_COMMAND
    bne @wait_clear_api_done
    rts