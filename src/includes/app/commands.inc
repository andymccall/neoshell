; --- Command Table ---
str_cmd_help:
    .asciiz "help"
str_cmd_clear:
    .asciiz "clear"
str_cmd_exit:
    .asciiz "exit"
str_cmd_echo:
    .asciiz "echo"
str_cmd_ls:
    .asciiz "ls"
str_cmd_cwd:
    .asciiz "cwd"

command_table:
    .word str_cmd_help          ; String to match
    .word print_version_info    ; Address of the handler function
    .word str_cmd_clear         ; String to match
    .word do_clear_screen       ; Address of the handler function
    .word str_cmd_exit          ; String to match
    .word do_exit               ; Address of the handler function
    .word str_cmd_echo          ; String to match
    .word do_echo               ; Address of the handler function
    .word str_cmd_ls            ; String to match
    .word do_ls                 ; Address of the handler function
    .word str_cmd_cwd            ;
    .word do_cwd
    .word 0                     ; End of table marker

;-------------------------------------------------------------------
; Name: strncmp
;-------------------------------------------------------------------
; Description: Compares a non-null-terminated string in the input
;              buffer with a null-terminated string from memory.
; Inputs: X = length of the buffer string
;         ZP_PTR_LO/HI = start address of the buffer string
;         ZP_PTR2_LO/HI = start address of the null-terminated string
; Returns: Carry flag is clear if strings match, set if they don't.
;-------------------------------------------------------------------
strncmp:
    phy                 ; Save Y register
    ldy #0              ; Y is our shared index
@loop:
    cpx #0              ; Have we compared all characters in the user token?
    beq @check_table_end ; If X is 0, then yes.

    lda (ZP_PTR_LO), y
    cmp (ZP_PTR2_LO), y
    bne @no_match_strncmp ; Characters do not match

    dex                 ; Decrement length counter
    iny                 ; Increment index
    jmp @loop

@check_table_end:
    ; We've matched all chars in the user token.
    ; Now, ensure the command table string ALSO ends here.
    ; This handles cases where user types "hel" for "help"
    lda (ZP_PTR2_LO), y
    bne @no_match_strncmp 

@match_strncmp:
    clc                 ; It's a perfect match, clear carry
    jmp @done_strncmp

@no_match_strncmp:
    sec                 ; Not a match, set carry

@done_strncmp:
    ply                 ; Restore original Y
    rts
