#include <stm32f10x.h>
#include "system_dumby.h"
#include "MAE.h"
#include "motor.h"



/**	
	*	@brief 	Cette fonction met � jour la machine � �tat de dumby.
	*					les �tats peuvent �tre : IDLE, RUN, DISABLE, LOW, CHARGE.
	*	@param 	Aucun
	*/
void machineWithStates(void)
{
	switch (Dumber.StateSystem)
	{
		case IDLE : // Le changement d'�tat se fait uniquement par commande
							break;
		case RUN:		if(Dumber.LowBat == TRUE)
										Dumber.StateSystem=LOW;								
							break;
		case DISABLE: cmdLeftMotor(BRAKE,255);
									cmdRightMotor(BRAKE,255);
									while(1);
									break;
		case LOW		: 	if(Dumber.cpt_watchdog>=3)
											Dumber.StateSystem=DISABLE;
							break;
		case CHARGE : 
							break;
	}
}
