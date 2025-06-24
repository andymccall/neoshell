br_debug_dot:
    pha
    lda #API_FN_WRITE_CHAR
    sta API_FUNCTION 
    lda API_COMMAND
   
    lda #$2E

    sta API_PARAMETERS + 0
    lda #API_GROUP_CONSOLE
    sta API_COMMAND
    pla
    rts