.include "includes/system/neo6502.asm"

.org $800
.segment "STARTUP"

   jmp start

; API includes
   .include "includes/api/br_text.inc"
   .include "includes/api/br_screen.inc"
   .include "includes/api/br_graphics.inc"
   .include "includes/api/br_debug.inc"

; App includes
   .include "includes/app/constants.inc"
   .include "includes/app/prompt.inc"
   .include "includes/app/version.inc"
   .include "includes/app/input.inc"
   .include "includes/app/parse.inc"
   .include "includes/app/commands.inc"
   .include "includes/app/clear.inc"
   .include "includes/app/exit.inc"
   .include "includes/app/echo.inc"
   .include "includes/app/ls.inc"
   .include "includes/app/cwd.inc"
   .include "includes/app/mkdir.inc"
   .include "includes/app/cd.inc"
   .include "includes/app/cat.inc"
   .include "includes/app/cp.inc"

;---------------------------------------------------------------
; Menu equates
;---------------------------------------------------------------
;
start:

   jsr br_screen_clear

   jsr print_version_info
   jsr br_text_print_new_line

app_loop:
  
   jsr print_prompt

   jsr get_input

   ;lda #<INPUT_BUFFER
   ;sta ZP_PTR_LO
   ;lda #>INPUT_BUFFER
   ;sta ZP_PTR_HI
   ;jsr br_text_print_string_nl
   
   jsr parse_input

done:
   jmp app_loop

