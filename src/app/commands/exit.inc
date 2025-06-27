; ===============================================================
; Exit Command
;
; This file contains the command handler for the built-in 'exit'
; command.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_exit
;-------------------------------------------------------------------
; Description: Halts the shell by jumping to the final 'done' loop
;              in the main program.
; Inputs: None
; Returns: None
; Destroys: None
;-------------------------------------------------------------------
do_exit:
    jmp done
