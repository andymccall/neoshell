; ===============================================================
; Help Command
;
; This file contains the command handler for the built-in 'help'
; command.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_help
;-------------------------------------------------------------------
; Description: Prints the shell's version information. This is the
;              handler function for the 'help' command.
; Inputs: None
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
do_help:
    lda #<str_version
    sta ZP_PTR_LO
    lda #>str_version
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts
