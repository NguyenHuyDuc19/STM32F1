		
		AREA	asm, code, readonly
		entry
		
		export delay
delay
		MOV R0, #0x00  ; set register R0
		B LOOP ; jump to loop
CONG
		ADD R0,R0,#1 ; add 1 to register R0
LOOP
		CMP R0,#22 ; compare R0 with 5
		BNE CONG ; jump to CONG if not equal
		mov pc, lr	; return
END
	
