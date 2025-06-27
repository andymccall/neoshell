; ===============================================================
; CAT Command
;
; This file contains the command handler for the built-in 'cat'
; command, which prints the contents of a file to the screen.
; ===============================================================

;-------------------------------------------------------------------
; Name: do_cat
;-------------------------------------------------------------------
; Description: Opens a file and prints its contents to the screen.
;              This version is now refactored to use the shared
;              get_next_argument_lp helper function.
; Inputs: X = index in INPUT_BUFFER where arguments start.
; Returns: None
; Destroys: a, x, y
;-------------------------------------------------------------------
do_cat:
    ; --- Step 1: Get the filename argument ---
    ; Use the helper function to get the first argument and create
    ; a length-prefixed string in DIR_ENTRY_BUFFER.
    lda #<DIR_ENTRY_BUFFER
    sta ZP_PTR_LO
    lda #>DIR_ENTRY_BUFFER
    sta ZP_PTR_HI
    jsr get_next_argument_lp
    bcc @cat_has_arg
    rts ; No argument given, so just return

@cat_has_arg:
    ; --- Step 2: Open the file ---
    ; Param 0: File channel to use (e.g., channel 1)
    ; Param 1,2: Pointer to length-prefixed filename
    ; Param 3: Mode (0 = read-only)
    lda #1                      ; Use file channel 1
    sta API_PARAMETERS+0
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+2
    lda #0                      ; Mode 0 = read-only
    sta API_PARAMETERS+3

    lda #API_FN_FILE_OPEN
    sta API_FUNCTION
@wait_cat_open_ready:
    lda API_COMMAND
    bne @wait_cat_open_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cat_open_done:
    lda API_COMMAND
    bne @wait_cat_open_done

    ; The API returns the handle in API_PARAMETERS+0 if successful.
    lda API_PARAMETERS+0
    cmp #1
    bne @cat_error
    
    sta ZP_FILE_HANDLE

    ; --- Step 3: Read from file in a loop ---
@cat_read_loop:
    ; Param 0: File handle
    lda ZP_FILE_HANDLE
    sta API_PARAMETERS+0
    ; Param 1-2: Pointer to buffer
    lda #<DIR_ENTRY_BUFFER
    sta API_PARAMETERS+1
    lda #>DIR_ENTRY_BUFFER
    sta API_PARAMETERS+2
    ; Param 3,4: Number of bytes to read (16-bit)
    lda #$FF                ; Read up to 255 bytes (low byte)
    sta API_PARAMETERS+3
    lda #$00                ; (high byte)
    sta API_PARAMETERS+4
    
    lda #API_FN_FILE_READ
    sta API_FUNCTION
@wait_cat_read_ready:
    lda API_COMMAND
    bne @wait_cat_read_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cat_read_done:
    lda API_COMMAND
    bne @wait_cat_read_done
    
    ; Number of bytes actually read is in API_PARAMETERS+3,4
    lda API_PARAMETERS+3
    beq @close_cat_file     ; If 0, we've reached the end of the file
    
    sta ZP_BYTE_COUNT
    
    ; Print the bytes we just read
    ldy #0
@cat_print_loop:
    cpy ZP_BYTE_COUNT
    beq @cat_read_loop
    lda DIR_ENTRY_BUFFER, y
    jsr br_text_print_char
    iny
    jmp @cat_print_loop
    
@cat_error:
    lda #<str_file_open_err
    sta ZP_PTR_LO
    lda #>str_file_open_err
    sta ZP_PTR_HI
    jsr br_text_print_string
    jsr br_text_print_new_line
    rts

@close_cat_file:
    ; --- Step 4: Close the file ---
    lda ZP_FILE_HANDLE
    sta API_PARAMETERS+0
    lda #API_FN_FILE_CLOSE
    sta API_FUNCTION
@wait_cat_close_ready:
    lda API_COMMAND
    bne @wait_cat_close_ready
    lda #API_GROUP_FILEIO
    sta API_COMMAND
@wait_cat_close_done:
    lda API_COMMAND
    bne @wait_cat_close_done
    
    jsr br_text_print_new_line
    rts
