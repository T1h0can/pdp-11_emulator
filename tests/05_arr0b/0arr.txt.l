000000:			.=200
000200:		A:	.BYTE	2, 3, -1, 4, 0
	    002
	 003
	    377
	 004
	    000
000205:		; R0 - адрес очередного числа
000205:		; R1 - очередное число
000205:		; R2 - сумма
000205:			
000205:			.=1000
001000:			mov		#A, R0		; положить 200 в R0
	 012700
	 000200
001004:		Z:	movb 	(R0)+, R1
	 112001
001006:			beq 	END
	 001402
001010:			add 	R1, R2		; R2 = R2 + R1
	 060102
001012:			br		Z
	 000774
001014:		
001014:		END:
001014:			halt
	 000000
001016:		
