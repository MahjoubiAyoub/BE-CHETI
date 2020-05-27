 	thumb		
	area  moncode, code, readonly

	import TabSin		
	import TabCos
	import TabSig
		
	export calcul_Reel
	export calcul_Im
	export DFT

DFT proc
	push {LR, R0, R1,R4-R7} 
	bl calcul_Reel
	mov r3, r0 ; La partie Reelle dans r1
	pop {r0, r1}
	push {r3}
	bl calcul_Im
	mov r2, r0 ; La partie Imaginaire dans r2
	pop {r3}
	
	smull r4, r5, r3, r3 ; Pour calculer Re²
	smull r6, r7, r2, r2 ; Pour calculer Im²
	add r5, r7 ; Pour la somme du DFT = Re² + Im²
	mov r0, r5 ;
	pop {r4-r7}
	pop{lr}
	
	bx lr
	endp
	

calcul_Im proc
    push {R4-R7}

    mov r12, #0x00 ; Pour initialiser le i    
    ldr r2, =TabSin ; Pour mettre l'adresse de TabSin
    b loop
    endp

calcul_Reel proc     
    push {R4-R7}
    
    mov r12, #0x00 ; Pour initialiser le i  

    ldr r2, =TabCos ; Pour mettre l'adresse de TabCos
    
    b loop

loop
    mul r5, r12, r0;
    and r5, #0x3F; Pour modulo 64
    
    ldrsh r4, [r2, r5, lsl #1] ; cos/sin(ik) dans r4
    ldrsh r3, [r1, r12, lsl #1] ; L'echantillon de x(i) dans r3

    mul r7, r3, r4; Multiplication x(i) avec cos/sin(ik)
    add r6, r7; La somme de x(i) avec cos/sin(ik)

    add r12, #0x01 ; i++
    cmp r12, #64 ; Pour comparer si i=N
    bne loop
    mov r0,r6 ; Sinon pour quitter la boucle
    b fin
    
fin
    pop {R4-R7}
    bx lr
    endp
    end