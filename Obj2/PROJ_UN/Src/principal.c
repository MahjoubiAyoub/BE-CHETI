#include "gassp72.h"

#define SYSTICK_PER 360000
// 360000 =  72M * 5ms

extern int DFT(int, unsigned short *);
unsigned short dma_buf[64];
int cmp_occurrences[6] = {0, 0, 0, 0, 0, 0};
int score[6] = { 0, 0, 0, 0, 0, 0};
int frequence[6] = {17, 18, 19, 20, 23, 24};
int M2TIR = 0xf0400;


void sys_callback(void){


    // Démarrage DMA pour 64 points
    Start_DMA1(64);
    Wait_On_End_Of_DMA1();
    Stop_DMA1;

    // Pour calculer la DFT des frequences des 6 pistolets 
    for (int i = 0; i < 6; i++) {

        if(DFT(frequence[i], dma_buf) >= M2TIR){
            cmp_occurrences[i]++;
        }
	else
	    cmp_occurrences[i] = 0;

        if(cmp_occurrences[i] > 15){
            cmp_occurrences[i] = 0;
            score[i]++;
        }

    }


}

int main(void){


    // activation de la PLL qui multiplie la fréquence du quartz par 9
    CLOCK_Configure();
	
    // PA2 (ADC voie 2) = entrée analog
    GPIO_Configure(GPIOA, 2, INPUT, ANALOG);
    // PB1 = sortie pour profilage à l'oscillo
    GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
    // PB14 = sortie pour LED
    GPIO_Configure(GPIOB, 14, OUTPUT, OUTPUT_PPULL);

    // activation ADC, sampling time 1us
    Init_TimingADC_ActiveADC_ff( ADC1, 82 );
    Single_Channel_ADC( ADC1, 2 );
    // Déclenchement ADC par timer2, periode (72MHz/320kHz)ticks
    Init_Conversion_On_Trig_Timer_ff( ADC1, TIM2_CC2, 225 );
    // Config DMA pour utilisation du buffer dma_buf (a créér)
    Init_ADC1_DMA1( 0, dma_buf);

    // Config Timer, période exprimée en périodes horloge CPU (72 MHz)
    Systick_Period_ff( SYSTICK_PER );
    // enregistrement de la fonction de traitement de l'interruption timer
    // ici le 3 est la priorité, sys_callback est l'adresse de cette fonction, a créér en C
    Systick_Prio_IT( 3, sys_callback );
    SysTick_On;
    SysTick_Enable_IT;  

    while (1){


	}
 
    
}
