ZP_TOKEN_START  = $FD   ; Holds the start index of the current token
ZP_PARSE_FLAG   = $FD   ; Used by parser to track state (1=in_whitespace)
ZP_JUMP_VEC     = $FD   ; Low byte of the indirect jump vector
ZP_TEMP         = $F8   ; A general-purpose temporary byte

;-------------------------------------------------------------------
; Name: parse_input
;-------------------------------------------------------------------
; Description: Parses the null-terminated string in INPUT_BUFFER.
;              It tokenizes the string by spaces and prints each
;              token followed by its index number.
;-------------------------------------------------------------------
; Inputs: INPUT_BUFFER containing user input.
; Returns: None
; Destroys: a, x, y
;-------------------------------------------------------------------
parse_input:
    ldx #0
@skip_parse_loop:
    lda INPUT_BUFFER, x
    bne @not_empty_line
    jmp @parse_done
@not_empty_line:
    cmp #' '
    bne @token_start
    inx
    jmp @skip_parse_loop

@token_start:
    ldy #0
@command_loop:
    lda command_table, y
    ora command_table+1, y
    beq @not_found
    lda command_table, y
    sta ZP_PTR_LO
    lda command_table+1, y
    sta ZP_PTR_HI
    phx
    phy
    ldy #0
@compare_loop:
    lda (ZP_PTR_LO), y
    beq @command_part_matched
    lda INPUT_BUFFER, x
    cmp (ZP_PTR_LO), y
    bne @no_match
    inx
    iny
    jmp @compare_loop
@command_part_matched:
    lda INPUT_BUFFER, x
    cmp #' '
    beq @execute_match
    cmp #0
    beq @execute_match
@no_match:
    ply
    plx
    tya
    clc
    adc #4
    tay
    jmp @command_loop

@execute_match:
    ply
    ; *** FIX: Use PLA instead of PLX to balance the stack
    ; and correctly preserve the argument pointer in X.
    pla
    tya
    clc
    adc #2
    tay
    lda command_table, y
    sta ZP_JUMP_VEC
    iny
    lda command_table, y
    sta ZP_JUMP_VEC+1
    jsr execute_handler
    rts

@not_found:
    lda #<str_cmd_not_found
    sta ZP_PTR_LO
    lda #>str_cmd_not_found
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts

@parse_done:
    rts
    
execute_handler:
    jmp (ZP_JUMP_VEC)


;-------------------------------------------------------------------
; Name: br_text_print_byte
;-------------------------------------------------------------------
; Description: Prints an 8-bit number in the A register as a
;              decimal value (0-255).
;-------------------------------------------------------------------
br_text_print_byte:
    ;jsr br_debug_dot
    phy                 ; *** FIX: Save Y register on the stack
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
    ply                 ; *** FIX: Restore original Y register from stack
    rts

get_next_argument_lp:
@skip_arg_spaces:
    lda INPUT_BUFFER, x
    beq @no_more_args
    cmp #' '
    bne @arg_start
    inx
    jmp @skip_arg_spaces
@arg_start:
    stx ZP_TOKEN_START
    ldy #0
@find_arg_end:
    lda INPUT_BUFFER, x
    beq @arg_len_found
    cmp #' '
    beq @arg_len_found
    iny
    inx
    jmp @find_arg_end
@arg_len_found:
    phy                     ; Save length (Y)
    
    ; Get pointer to destination buffer from ZP_PTR_LO/HI
    lda ZP_PTR_LO
    sta ZP_PTR2_LO
    lda ZP_PTR_HI
    sta ZP_PTR2_HI
    
    ; Store length as the first byte of the destination buffer
    tya
    sta (ZP_PTR2_LO)
    
    ; Copy the argument string itself
    ldx ZP_TOKEN_START
    ldy #1
@copy_arg_loop:
    lda INPUT_BUFFER, x
    cmp #' '
    beq @copy_arg_done
    cmp #0
    beq @copy_arg_done
    sta (ZP_PTR2_LO),y
    inx
    iny
    jmp @copy_arg_loop

@copy_arg_done:
    ply                     ; Restore length
    clc                     ; Success
    rts
@no_more_args:
    sec                     ; Failure
    rts