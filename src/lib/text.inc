; ===============================================================
; Text and Character Library
;
; This file contains all the low-level subroutines for printing
; strings, characters, and numbers to the console.
; ===============================================================

;-------------------------------------------------------------------
; Name: br_text_print_string
;-------------------------------------------------------------------
; Description: Prints a null-terminated string.
; Inputs: ZP_PTR_LO/HI point to the string to be printed.
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
br_text_print_string:
    phy                 ; Save the Y register so we don't clobber it
    ldy #0
@print_loop:
    lda (ZP_PTR_LO), y
    beq @done_printing
    jsr br_text_print_char
    iny
    jmp @print_loop
@done_printing:
    ply                 ; Restore the original Y register
    rts

;-------------------------------------------------------------------
; Name: br_text_print_new_line
;-------------------------------------------------------------------
; Description: Prints a new line (carriage return) to the console.
; Inputs: None
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
br_text_print_new_line:
    lda #$0D
    jsr br_text_print_char
    rts

;-------------------------------------------------------------------
; Name: br_text_print_char
;-------------------------------------------------------------------
; Description: Prints a single character to the console.
; Inputs: a - the character to print
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
br_text_print_char:
    pha
    lda #API_FN_WRITE_CHAR
    sta API_FUNCTION
@wait_char_api_ready:
    lda API_COMMAND
    bne @wait_char_api_ready
    pla
    sta API_PARAMETERS + 0
    lda #API_GROUP_CONSOLE
    sta API_COMMAND
@wait_char_api_done:
    lda API_COMMAND
    bne @wait_char_api_done
    rts

;-------------------------------------------------------------------
; Name: br_text_print_byte
;-------------------------------------------------------------------
; Description: Prints an 8-bit number in the A register as a
;              decimal value (0-255).
; Inputs: a - the byte to print
; Returns: None
; Destroys: a, x, y
;-------------------------------------------------------------------
br_text_print_byte:
    phx                 ; Save X register
    phy                 ; Save Y register on the stack
    ldy #0              ; Y is the "have printed" flag
    ; --- Hundreds ---
    ldx #'0' - 1
@h_loop:
    inx
    cmp #100
    bcc @h_done
    sbc #100
    jmp @h_loop
@h_done:
    cpx #'0'
    beq @tens           ; If hundreds digit is 0, skip printing
    pha                 ; Save remainder in A
    txa                 ; Move digit from X to A
    jsr br_text_print_char
    pla                 ; Restore remainder to A
    ldy #1              ; Set flag: we've printed something

@tens:
    ; A contains remainder < 100
    ldx #'0' - 1
@t_loop:
    inx
    cmp #10
    bcc @t_done
    sbc #10
    jmp @t_loop
@t_done:
    cpy #1              ; Have we printed a hundreds digit?
    bne @t_check_zero   ; No, so check if tens is also zero
    
    ; If we printed hundreds, we MUST print the tens digit, even if 0.
    pha                 ; Save remainder
    txa
    jsr br_text_print_char
    pla                 ; Restore remainder
    jmp @ones

@t_check_zero:
    cpx #'0'
    beq @ones           ; If tens digit is 0, skip it
    pha                 ; Save remainder
    txa
    jsr br_text_print_char
    pla                 ; Restore remainder
    ldy #1              ; Set printed flag

@ones:
    ; A contains remainder < 10 (the ones digit)
    ora #'0'
    jsr br_text_print_char
    ply                 ; Restore original Y register from stack
    plx                 ; Restore X register
    rts
