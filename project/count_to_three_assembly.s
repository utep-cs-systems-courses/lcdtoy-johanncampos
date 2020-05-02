	.arch msp430g2553
	
	.data
num:	.word 0			;static char num = 0
	
	.text			;jump table
jt:	.word case0
	.word case1
	.word case2
	.word case3
	
	.global count_to_three	;void count_to_three()
count_to_three:
	mov &num, r12		;num in r12
	add r12, r12		;num = 2*num
	mov jt(r12), r0		;go to appropriate case

case0:	mov #0, &red_on		;red_on = 0
	mov #0, &green_on	;green_on = 0
	mov #7644, r12		;C4
	jmp end
	
case1:	xor #1, &red_on		;red_on ^= 1
	mov #6067, r12		;E4
	jmp end
	
case2:	xor #1, &red_on		;red_on ^= 1
	xor #1, &green_on	;green_on ^= 1
	mov #5102, r12		;G4
	jmp end
	
case3:	xor #1, &red_on		;red_on ^= 1
	mov #3822, r12		;C5
	jmp end

end:	call #buzzer_set_period	;buzzer_set_period(r12 = C4, E4, G4, or C5)
	add #1, &num		;num++
	cmp #4, &num		;if (num == 4)
	jge reset
	ret			;end count_to_three

reset:	mov #0, &num		;num = 0
	ret			;end count_to_three
