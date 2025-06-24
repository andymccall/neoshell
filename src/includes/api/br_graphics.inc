;-------------------------------------------------------------------
; Name: br_graphics_wait_for_vsync
;-------------------------------------------------------------------
; Description: Waits for the next video frame (vsync) by polling
;              the API's frame counter. It returns once the frame
;              count has changed from the last time this
;              function was called.
; Note: 
; URL:
;-------------------------------------------------------------------
; Inputs: None
; Returns: None
; Destroys: a
;-------------------------------------------------------------------
br_graphics_wait_for_vsync:
@polling_loop:
    ; ===============================================================
    ; Perform a complete and safe API call to get the current frame count.
    ; ===============================================================

    ; Step 1: Wait for the API to be ready for a new command.
    @wait_api_ready:
        lda API_COMMAND
        bne @wait_api_ready

    ; Step 2: Set up the command parameters.
    lda #API_FN_FRAME_COUNT
    sta API_FUNCTION

    ; Step 3: Issue the command to the graphics API group.
    lda #API_GROUP_GRAPHICS
    sta API_COMMAND

    ; Step 4: CRITICAL - Wait for our command to complete.
    ; The original code was missing this step.
    @wait_api_done:
        lda API_COMMAND
        bne @wait_api_done

    ; The result is now safely available in the API_PARAMETERS.
    ; ===============================================================

    ; Compare the new frame count with the one from the last vsync.
    lda API_PARAMETERS + 0
    cmp last_frame_count + 0
    bne @frame_has_changed    ; If different, a new frame has occurred.

    lda API_PARAMETERS + 1
    cmp last_frame_count + 1
    bne @frame_has_changed

    lda API_PARAMETERS + 2
    cmp last_frame_count + 2
    bne @frame_has_changed

    lda API_PARAMETERS + 3
    cmp last_frame_count + 3
    beq @polling_loop         ; If all bytes are the same, loop and check again.

@frame_has_changed:
    ; The frame count is new. Update our stored value for the next call.
    lda API_PARAMETERS + 0
    sta last_frame_count + 0
    lda API_PARAMETERS + 1
    sta last_frame_count + 1
    lda API_PARAMETERS + 2
    sta last_frame_count + 2
    lda API_PARAMETERS + 3
    sta last_frame_count + 3

    rts

last_frame_count:   .res 4 ; 4 bytes to store the last known frame count.
