; ce programme est pour l'assembleur RealView (Keil)
	thumb

	area   moncode, code, readwrite
	export callback
	import etat
		
TIM3_CCR3	equ	0x4000043C	; adresse registre PWM

; Offsets pour l'accès au différentes valeurs de etat
E_POS	equ	0; position
E_TAI	equ	4; taille
E_SON	equ	8; son
E_RES	equ	12; resolution
E_PER	equ	16; periode en ticks	
	
callback	proc

	; Gestion de la fin du son
	ldr r2, =etat ;adresse de la structure
	ldr r0, [r2, #E_POS] ; Mettre l'@ de la taille dans r0
	ldr r1, [r2, #E_TAI] ; Mettre l'@ de la position dans r1
	cmp r0, r1 ; Si la position arrive a une valeur superieure ou egale a taille
	bne silence ; On emet du silence
	
	ldr r4,=TIM3_CCR3 ; adr de timer3
	mov r5,#0 
	str r5, [r4]; on injecte 0
	b return
	
;Silence
silence
	ldr r3, [r2, #E_SON] ; On recupere le son
	add r3, r0 ; On voit où on est positionne
	add r0, #2 ; On le met en 16 bits
	str r0, [r2, #E_POS]
	
	ldrsh r8, [r3] ; Recuperation de l'echantillon
	add r8, #32768 ;calibrage 
	ldr r0, [r2, #E_RES] ; Pareil avec la resolution
	mul r8, r0
	lsr r8, #16
	
	;Envoi du son
	ldr r4, =TIM3_CCR3 ;adr du timer
	str r8, [r4] ; Pour l'affichage dans le logic analyser
	
	b return
	
return
	bx lr
	endp
	end