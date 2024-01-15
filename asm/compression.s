.include "macros.inc"
.section .text, "ax"


glabel slidec1
	lw	$24,4($4)	
	lw	$7,8($4)	
	lw	$25,12($4)	
	move	$6,$0	
	add	$24,$5
	add	$7,$4		
	add	$25,$4		
	add	$4,16
	
slidemain2:	bne	$6,$0,codecheck2
	lw	$8,($4)
	li	$6,32
	add	$4,4
codecheck2:	slt	$9,$8,$0
	beq	$9,$0,pressdata2
	lb	$10,($25)
	add	$25,1
	sb	$10,($5)
	add	$5,1
	b	loopend2
pressdata2:	lhu	$10,($7)
	add	$7,2
	srl	$11,$10,12
	and	$10,0xfff
	sub	$9,$5,$10
	add	$11,3
pressloop2:	lb	$10,-1($9)
	sub	$11,1
	add	$9,1
	sb	$10,($5)
	add	$5,1
	bne	$11,$0,pressloop2
loopend2:	sll	$8,1
	sub	$6,1
	bne	$5,$24,slidemain2
	jr	$31

