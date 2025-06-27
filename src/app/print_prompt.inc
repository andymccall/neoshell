; ===============================================================
; Print Prompt
;
; This file contains the subroutine for printing the shell prompt.
; ===============================================================

;-------------------------------------------------------------------
; Name: print_prompt
;-------------------------------------------------------------------
; Description: Prints the shell prompt string to the console.
; Inputs: None
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
print_prompt:
    lda #<str_prompt
    sta ZP_PTR_LO
    lda #>str_prompt
    sta ZP_PTR_HI
    jsr br_text_print_string
    rts