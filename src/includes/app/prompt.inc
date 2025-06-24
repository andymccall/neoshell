print_prompt:

    lda #<str_prompt
    sta ZP_PTR_LO
    lda #>str_prompt
    sta ZP_PTR_HI

    jsr br_text_print_string

    rts