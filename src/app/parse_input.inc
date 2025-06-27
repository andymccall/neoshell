; ===============================================================
; Command Parser and Dispatcher
;
; This file contains the main routine for parsing the user's
; input string to identify the command and its arguments.
; ===============================================================

;-------------------------------------------------------------------
; Name: parse_input
;-------------------------------------------------------------------
; Description: Finds the first token in the INPUT_BUFFER and tries
;              to match it with a built-in command. If a match is
;              found, it executes the corresponding handler. The X
;              register is passed to the handler, pointing to the
;              character after the command token (i.e., the start
;              of the arguments).
; Inputs: INPUT_BUFFER contains the user's command line.
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
    ; *** FIX: Use PLA instead of PLX to balance the stack. ***
    ; This preserves the X register, which now correctly points to
    ; the start of the command's arguments.
    pla
    
    ; Get the handler address from the table
    tya
    clc
    adc #2
    tay
    lda command_table, y
    sta ZP_JUMP_VEC
    iny
    lda command_table, y
    sta ZP_JUMP_VEC+1
    
    ; Call the handler. X now correctly points to the arguments.
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
