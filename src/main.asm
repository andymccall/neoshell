; ===============================================================
; NeoShell - Main Program
;
; Author: Andy McCall, mailme@andymccall.co.uk
; Date:   27/06/2025
;
; This is the main entry point for the NeoShell project. It
; includes all necessary libraries and application modules,
; and contains the main application loop.
; ===============================================================

.include "lib/system/neo6502.asm"

.org $800
.segment "STARTUP"

   jmp start

; ===============================================================
; Project-Specific Includes
; ===============================================================

; --- Core Definitions ---
.include "includes/definitions.inc"

; --- Libraries ---
.include "lib/text.inc"
.include "lib/argument.inc"

; --- Application Modules ---
.include "app/get_input.inc"
.include "app/print_prompt.inc"
.include "app/commands/commands.inc"

; --- Command Handlers ---
.include "app/commands/help.inc"
.include "app/commands/clear.inc"
.include "app/commands/exit.inc"
.include "app/commands/echo.inc"
.include "app/commands/cwd.inc"
.include "app/commands/ls.inc"
.include "app/commands/mkdir.inc"
.include "app/commands/cd.inc"
.include "app/commands/cat.inc"
.include "app/commands/cp.inc"
.include "app/commands/rm.inc"
.include "app/commands/mv.inc"

; --- Parser (must be included AFTER command handlers) ---
.include "app/parse_input.inc"

start:

; ===============================================================
; Main Program Entry Point
; ===============================================================
main:
    ; This is the main application loop. It demonstrates how to use
    ; the various subroutines to build a simple command shell.
app_loop:
    jsr print_prompt        ; Show "neoshell> "
    jsr get_input           ; Read a line of text from the user
    jsr parse_input         ; Parse the input and execute commands
    jmp app_loop            ; Loop forever

done:
    ; This is the final halt point for the 'exit' command.
    jmp done

