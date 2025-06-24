;-------------------------------------------------------------------
; Zero Page Definitions
;-------------------------------------------------------------------
; These locations are used as pointers by the string printing
; routine. Using the zero page for pointers allows for faster
; memory access using indirect indexed addressing.
;-------------------------------------------------------------------
ZP_PTR_LO = $FB               ; Low byte of the string address pointer
ZP_PTR_HI = $FC               ; High byte of the string address pointer

;-------------------------------------------------------------------
; Name: br_text_print_string
;-------------------------------------------------------------------
; Description: Prints a null-terminated string to the console, 
;              The address of the string must be in the
;              ZP_PTR_LO/HI pointer.
; Note: 
; URL:
;-------------------------------------------------------------------
; Inputs: ZP_PTR_LO/HI - location of the string to print
; Returns: None
; Destroys: a, y
;-------------------------------------------------------------------
br_text_print_string:
    phy                 ; Save the Y register so we don't clobber it
    ldy #0
@loop:
    lda (ZP_PTR_LO), y
    beq @done_printing
    jsr br_text_print_char
    iny
    bne @loop
@done_printing:
    ply                 ; Restore the original Y register
    rts

;-------------------------------------------------------------------
; Name: br_text_print_string_nl
;-------------------------------------------------------------------
; Description: Prints a null-terminated string to the console,
;              followed by a new line. The address of the string 
;              must be in the ZP_PTR_LO/HI pointer.
; Note: 
; URL:
;-------------------------------------------------------------------
; Inputs: ZP_PTR_LO/HI - location of the string to print
; Returns: None
; Destroys: a, y
;-------------------------------------------------------------------
br_text_print_string_nl:
    ldy #0                    ; Use Y as an index into the string, start at 0.
@loop:
    lda (ZP_PTR_LO), y        ; Load character from (address), indexed by Y.
    beq @done                 ; If character is 0 (null terminator), we are done.

    jsr br_text_print_char    ; It's a valid character, so print it.
    iny                       ; Move to the next character in the string.
    bne @loop                 ; If Y is not zero, loop back.
                              ; NOTE: This simple loop handles strings up to 255
                              ; characters long. For longer strings, you would
                              ; need to also increment ZP_PTR_HI when Y wraps
                              ; around from $FF to $00.

@done:
    jsr br_text_print_new_line; New line
    rts                       ; Return to the caller.

;-------------------------------------------------------------------
; Name: br_text_print_char
;-------------------------------------------------------------------
; Description: Prints a single character to the console.
; 
; Note: 
; URL:
;-------------------------------------------------------------------
; Inputs: a - the character to print
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
br_text_print_char:
    pha                       ; The A register holds the character we want to print.
                              ; We must save it on the stack because the wait loop
                              ; below uses 'lda', which will overwrite A.

    lda #API_FN_WRITE_CHAR    ; Select the "write character" function.
    sta API_FUNCTION

; Wait for the API to be free (i.e., for the previous command to finish).
@wait_api_ready:
    lda API_COMMAND
    bne @wait_api_ready

    pla                       ; The API is free. Restore our character from the stack.
    sta API_PARAMETERS + 0    ; Pass the character as a parameter to the API.

    lda #API_GROUP_CONSOLE    ; Select the Console I/O group.
    sta API_COMMAND           ; Issue the command to the API.

; Wait for our own command to be processed by the API.
@wait_api_done:
    lda API_COMMAND
    bne @wait_api_done

    rts 

;-------------------------------------------------------------------
; Name: br_text_print_new_line
;-------------------------------------------------------------------
; Description: Prints a new line (carriage return) to the console.
;
; Note: 
; URL:
;-------------------------------------------------------------------
; Inputs: None
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
br_text_print_new_line:
    lda #$0D                  ; Load the carriage return character into the Accumulator.
    jsr br_text_print_char    ; Call our generic routine to print it.
    rts                       ; Return to the caller.
