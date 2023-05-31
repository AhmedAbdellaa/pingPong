/*
 * main.c
 *
 *  Created on: May 7, 2023
 *      Author: ahmed
 */

#include <util/delay.h>
#include "0-MCAL/1-DIO/DIO_interface.h"
#include "0-MCAL/2-Port/PORT_interface.h"
#include "libraries/STD_TYPES_H.h"
#include "libraries/BIT_MATH.h"
#include "libraries/ErrType.h"
#include "0-MCAL/4-GIE/GIE_interface.h"
#include "0-MCAL/3-EXTI/EXTI_interface.h"
#include "3-app/ping_pong/PP_interface.h"
#include "1-HAL/CLCD/CLCD_interface.h"
// global
static uint8 Global_flag= 0x00;
static PP_ball_Config_t Global_ball = {3U,1U, INSCREEN, 75, LEFT};
static PP_player_Config_t Global_leftPlayer = {1U, LEFT};

void INT0ISR(void)
{
	Global_flag += 1;
	DIO_u8_SetPinDir(DIO_PORTD,DIO_PIN2,DIO_PIN_INPUT);
	DIO_u8_SetPinVal(DIO_PORTD,DIO_PIN2,DIO_PIN_LOW);
}
void INT2ISR(void)
{
	PP_u8PlayerToggle(&Global_leftPlayer);
}

////******left screen***////////
void main()
{
	/*Initialize*/
	Port_voidInit();
	GIE_voidEnableGlobal();
	EXTI_voidInit();
	CLCD_VoidInit();
//	/*initialize ISR*/
	EXTI_u8SetCallBack(INT0, &INT0ISR);
	EXTI_u8SetCallBack(INT2, &INT2ISR);

	// move the ball across the screen if the ball in the screen
	PP_u8initPlayer(&Global_leftPlayer);
while (1)
	{
	PP_u8GameRun(&Global_ball, &Global_leftPlayer);
	PP_voidCheckINTFlag(&Global_flag,&Global_ball,&Global_leftPlayer);
	}
	}
