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
    ldx #0                  ; X is the buffer index

    ; --- Step 1: Skip leading spaces to find the command ---
@skip_loop:
    lda INPUT_BUFFER, x
    bne @not_empty_line     ; Branch if not zero (not an empty line)
    jmp @parse_done         ; It was zero, so jump to the end.
@not_empty_line:
    cmp #' '
    bne @token_start
    inx
    jmp @skip_loop

@token_start:
    ; --- Step 2: Loop through the command table ---
    ldy #0                  ; Y is the index into the command table
@command_loop:
    ; Check for end of table marker (a null pointer).
    lda command_table, y
    ora command_table+1, y
    beq @not_found
    
    ; Get pointer to the command string from the table
    lda command_table, y
    sta ZP_PTR_LO
    lda command_table+1, y
    sta ZP_PTR_HI
    
    ; Compare user input with table command
    phx                     ; Save user's buffer index (X)
    phy                     ; Save command table index (Y)

    ldy #0                  ; Use Y as the shared string index for comparison
@compare_loop:
    lda (ZP_PTR_LO), y      ; Get char from table command
    pha                     ; Save it
    lda INPUT_BUFFER, x     ; Get char from user input
    cmp (ZP_PTR_LO), y      ; Compare user char with table char
    bne @no_match           ; If different, it's not a match

    ; Characters match. If it was null, both strings have ended. Match!
    pla                     ; Restore table char
    beq @match_found

    ; Not null, so continue
    inx
    iny
    jmp @compare_loop
    
@no_match:
    pla                     ; Balance stack
    ply                     ; Restore command table index (Y)
    plx                     ; Restore user buffer index (X)
    
    ; Advance Y to the next command entry and try again
    tya
    clc
    adc #4
    tay
    jmp @command_loop

@match_found:
    ; We have a match, but we need to ensure the user's token ALSO ends here.
    lda INPUT_BUFFER, x
    cmp #' '
    beq @execute_match
    cmp #0
    beq @execute_match
    ; If it doesn't end here (e.g., user typed "clears"), it's not a match
    jmp @no_match

@execute_match:
    ply                     ; Restore original command table index Y
    plx                     ; Restore original buffer index X

    ; Get the handler address (offset is original Y + 2 bytes)
    tya
    clc
    adc #2
    tay
    lda command_table, y    ; Get handler low byte
    sta ZP_JUMP_VEC
    iny
    lda command_table, y    ; Get handler high byte
    sta ZP_JUMP_VEC+1
    
    jsr execute_handler
    rts                     ; Command finished, return from parser

@not_found:
    ; --- No Match Found in entire table ---
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