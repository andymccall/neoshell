do_exit:
    jsr br_screen_clear
@wait_api:
    lda API_COMMAND
    bne @wait_api
    lda #API_FN_RESET
    sta API_FUNCTION
    lda #API_GROUP_SYSTEM
    sta API_COMMAND
    rts