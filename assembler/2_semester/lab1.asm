BITS 16   	;we inform that this is a 16-bit code
ORG 0x7c00	;physical address of the program

mov ax, 0x03	;move cursor and shape to the start screen
int 0x10	;BIOS interrupt to call video mode

mov si, msg	;we write the address of the string into the register si
mov ah, 0xe	;console output of a character to the screen
point:
	lodsb		;we write to the register al the character located at the address in the si
	test al, al	;checking that we have reached the end of the line
	jz exit		;in case of equality, move to the label 'exit'
	int 0x10	;BIOS interrupt to call video mode
	jmp point

exit:
	jmp $		;endless cycle

msg db 'Hello, world!', 0	;message to display
times 510 - ($ - $$) db 0	;filling the rest of the sector with zeros
dw 0xAA55			;boot sector signature
	
