; ===============================================================
; Command Definitions
;
; This file defines all command name strings and the main
; command_table. It then includes all the individual command
; handler files, making it the central place to add new commands.
; ===============================================================

; --- Command Name String Definitions ---
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
str_cmd_pwd:
    .asciiz "pwd"
str_cmd_mkdir:
    .asciiz "mkdir"
str_cmd_cd:
    .asciiz "cd"
str_cmd_cat:
    .asciiz "cat"
str_cmd_cp:
    .asciiz "cp"

; --- Command Error and Info Strings ---
str_cmd_not_found:
    .asciiz "Command not found"
str_version:
    .asciiz "NeoShell, version 0.01-alpha (neo6502-6502)"
str_root_dir:
    .asciiz "/"
str_open_dir_err:
    .asciiz "Error opening directory"
str_mkdir_err:
    .asciiz "Error creating directory"
str_file_open_err:
    .asciiz "Error opening file"
str_copy_err:
    .asciiz "Error copying file"
str_prompt:
    .asciiz "$ > "


; ===============================================================
; Command Table
;
; Format:
;   .word <address of command string>
;   .word <address of handler function>
;
; The table is terminated by a null pointer (.word 0).
; ===============================================================
command_table:
    .word str_cmd_help          ; String to match
    .word do_help               ; Address of the handler function
    
    .word str_cmd_clear         ; String to match
    .word do_clear              ; Address of the handler function
    
    .word str_cmd_exit          ; String to match
    .word do_exit               ; Address of the handler function
    
    .word str_cmd_echo          ; String to match
    .word do_echo               ; Address of the handler function
    
    .word str_cmd_cwd           ; String to match
    .word do_cwd                ; Address of the handler function
    .word str_cmd_pwd           ; String to match
    .word do_cwd                ; Address of the handler function

    .word str_cmd_ls            ; String to match
    .word do_ls                 ; Address of the handler function
    
    .word str_cmd_mkdir         ; String to match
    .word do_mkdir              ; Address of the handler function
    
    .word str_cmd_cd            ; String to match
    .word do_cd                 ; Address of the handler function
    
    .word str_cmd_cat           ; String to match
    .word do_cat                ; Address of the handler function
    
    .word str_cmd_cp            ; String to match
    .word do_cp                 ; Address of the handler function
    
    .word 0                     ; End of table marker
