	.file	"main.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	main
	.type	main, @function
	
main:
	push r29
	push r28
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	std Y+2,__zero_reg__
	std Y+1,__zero_reg__
	rjmp .L2
.L3:
	ldd r24,Y+2
	subi r24,lo8(-(1))
	std Y+2,r24
	ldd r24,Y+1
	subi r24,lo8(-(1))
	std Y+1,r24
.L2:
	ldd r24,Y+1
	cpi r24,lo8(2)
	brlt .L3
	ldi r24,lo8(0)
	ldi r25,hi8(0)
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	main, .-main
