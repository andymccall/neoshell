str_version:         .asciiz "NeoShell, version 0.01-alpha (neo6502-6502)"
str_prompt:          .asciiz "$ > "
str_equals:          .asciiz " = "
str_t:               .asciiz "t"
str_comma_space:     .asciiz ", "

str_cmd_not_found:   .asciiz "Command not found"

ZP_PTR2_LO      = $FB   ; Low byte of a general-purpose pointer 2
ZP_PTR2_HI      = $FC   ; High byte of a general-purpose pointer 2

str_found_cmd:       .asciiz "Found: "