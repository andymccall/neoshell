;-------------------------------------------------------------------
; Name: print_version_info
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
print_version_info:
    lda #<str_version
    sta ZP_PTR_LO
    lda #>str_version
    sta ZP_PTR_HI

    jsr br_text_print_string_nl
    rts