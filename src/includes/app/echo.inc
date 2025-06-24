;-------------------------------------------------------------------
; Name: do_echo
;-------------------------------------------------------------------
; Description: Prints the arguments that follow the "echo" command.
; Inputs: X = index in INPUT_BUFFER where arguments start.
;-------------------------------------------------------------------
do_echo:
    ; The X register holds the index of the character just after the
    ; "echo" command token (usually a space).
    ; We need to skip any additional spaces to find the real start
    ; of the arguments.
@skip_echo_spaces:
    lda INPUT_BUFFER, x
    cmp #' '
    bne @args_found
    inx
    jmp @skip_echo_spaces

@args_found:
    ; X now points to the start of the arguments.
    ; Set ZP_PTR to point to this location.
    txa
    clc
    adc #<INPUT_BUFFER
    sta ZP_PTR_LO
    lda #>INPUT_BUFFER
    adc #0
    sta ZP_PTR_HI
    
    ; Print the rest of the buffer and a newline.
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts