; ===============================================================
; Definitions
;
; This file contains all memory layout definitions, such as
; zero-page variable addresses and buffer locations.
; ===============================================================

; --- Zero Page Variables ---
; These are used for frequently accessed pointers and variables
; to take advantage of the faster zero-page addressing modes.
ZP_TOKEN_START  = $F6   ; A dedicated ZP location for the start of a token/argument
ZP_FILE_HANDLE  = $F7   ; A dedicated ZP location for the source file handle
ZP_BYTE_COUNT   = $F8   ; A dedicated ZP location for the cat byte count
ZP_PTR_LO       = $F9   ; Low byte of a general-purpose pointer 1
ZP_PTR_HI       = $FA   ; High byte of a general-purpose pointer 1
ZP_PTR2_LO      = $FB   ; Low byte of a general-purpose pointer 2
ZP_PTR2_HI      = $FC   ; High byte of a general-purpose pointer 2
ZP_JUMP_VEC     = $FD   ; Low byte of the indirect jump vector for command handlers

; --- Memory Buffers ---
INPUT_BUFFER    = $0300 ; Start address of our main input buffer in RAM
INPUT_BUFFER_SIZE = 255 ; Size of the input buffer

DIR_ENTRY_BUFFER = $0400 ; A general-purpose buffer for directory entry data
                         ; and for building length-prefixed strings.

FILENAME_BUFFER2 = $0500 ; A second buffer specifically for the destination
                         ; filename in the 'cp' command.
