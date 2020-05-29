#include "gassp72.h"
#define periode_PWN 360 //On intie la periode comme elle est donnee
extern void callback(void);	

extern short Son;
extern int LongueurSon; //On recupere toutes ces valeurs dans le fichier du bruit de verre casse
extern int PeriodeSonMicroSec;

typedef struct {
int position;		// 0
int taille;		// 4
void * son;		// 8
int resolution;		// 12
int periode_ticks;	// 16
} type_etat;

type_etat etat;

int main(void)
{
	// config port PB0 pour être utilisé par TIM3-CH3
	GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL);
	// On met TIM3-CH3 en mode PWM
	etat.resolution = PWM_Init_ff( TIM3, 3, periode_PWN );
	etat.son = &Son;
	etat.taille = LongueurSon * 2; // On met les valeurs du son dans notre avriable etat
	etat.position = 0;
	etat.periode_ticks = PeriodeSonMicroSec*72;
	
	// activation de la PLL qui multiplie la fréquence du quartz par 9
	CLOCK_Configure();
	// config port PB1 pour être utilisé en sortie
	GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
	
	// initialisation du timer 4
	// Periode_en_Tck doit fournir la durée entre interruptions,
	// exprimée en périodes Tck de l'horloge principale du STM32 (72 MHz)
	Timer_1234_Init_ff( TIM4, etat.periode_ticks );
	
	// enregistrement de la fonction de traitement de l'interruption timer
	// ici le 2 est la priorité, timer_callback est l'adresse de cette fonction, a créér en asm,
	// cette fonction doit être conforme à l'AAPCS
	Active_IT_Debordement_Timer( TIM4, 2, callback );
	
	// lancement du timer
	Run_Timer( TIM4 );
	Run_Timer( TIM3 );
	
while	(1)
	{
	}
}
