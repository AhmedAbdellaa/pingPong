
#include <stdlib.h>
#include <util/delay.h>

#include "../../libraries/STD_TYPES_H.h"
#include "../../libraries/ErrType.h"
#include "../../0-MCAL/1-DIO/DIO_interface.h"
#include "../../0-MCAL/3-EXTI/EXTI_interface.h"
#include "../../1-HAL/CLCD/CLCD_interface.h"

#include "PP_interface.h"

uint8 PP_u8initPlayer(PP_player_Config_t *ref_player){
	uint8 Local_u8Error_state = OK;
	if (ref_player != NULL)
	{
		if (ref_player->screenPos == RIGHT)
			CLCD_voidGoToXY(15,ref_player->pos);
		else if (ref_player->screenPos == LEFT)
			CLCD_voidGoToXY(0,ref_player->pos);
		CLCD_VoidSendData('|');
	}
	else
		Local_u8Error_state = NULL_PTR;
	return Local_u8Error_state;
}

uint8 PP_u8BallMoving(PP_ball_Config_t *ref_ball, PP_player_Config_t *ref_player)
{
	uint8 Local_u8Error_state = OK;

	if (ref_ball != NULL && ref_player != NULL)
	{
		if(ref_ball->flag == INSCREEN){

			sint8 Local_s8_iter0=0;
			sint8 Local_s8EndLocation ;

			if (ref_ball->WalkDirection == LEFT)
			{
				if (ref_player->screenPos==RIGHT)
					Local_s8EndLocation = 0;
				else
					Local_s8EndLocation = 1;
				// 15 : length of screen without player cell
				for (Local_s8_iter0 = ref_ball->posX; Local_s8_iter0 >=Local_s8EndLocation; Local_s8_iter0--)
				{
					// clear old ball pos from screen
					CLCD_voidGoToXY(ref_ball->posX , ref_ball->posY);
					CLCD_VoidSendData(' ');
					// random flip ball pos up and down every 4 step
					if (Local_s8_iter0 % 4 == 0)
						ref_ball->posY = rand() % 2; // return 0 or 1

					ref_ball->posX = Local_s8_iter0;
					// put ball in the new pos on lcd
					CLCD_voidGoToXY(ref_ball->posX, ref_ball->posY);
					CLCD_VoidSendData('*');
					// how fast moving the ball
					_delay_ms(ref_ball->speed);
				}

				if (ref_player->screenPos == RIGHT)
				{
					// finally ball out of screen : raising flag
					ref_ball->flag = OUTSCREEN;
					// clear old ball pos from screen
					CLCD_voidGoToXY(ref_ball->posX , ref_ball->posY);
					CLCD_VoidSendData(' ');
				}else{
//
				}
			}
			else if (ref_ball->WalkDirection == RIGHT)
			{
				if (ref_player->screenPos==RIGHT)
					Local_s8EndLocation = 14;
				else
					Local_s8EndLocation = 15;
				// 15 : Length of screen without player cell
				for (Local_s8_iter0 = ref_ball->posX ; Local_s8_iter0 <=Local_s8EndLocation; Local_s8_iter0++)
				{
					// clear old ball pos from screen
					CLCD_voidGoToXY(ref_ball->posX , ref_ball->posY);
					CLCD_VoidSendData(' ');
					// random flip ball pos up and down every 4 step
					if (Local_s8_iter0 % 4 == 0)
						ref_ball->posY = rand() % 2; // return 0 or 1
					ref_ball->posX = Local_s8_iter0;
					// put ball in the new pos on lcd
					CLCD_voidGoToXY(ref_ball->posX, ref_ball->posY);
					CLCD_VoidSendData('*');

					// how fast moving the ball
					_delay_ms(ref_ball->speed);
				}
				if (ref_player->screenPos == LEFT)
				{
					// finally ball out of screen : raising flag
					ref_ball->flag = OUTSCREEN;

					CLCD_voidGoToXY(ref_ball->posX , ref_ball->posY);
					CLCD_VoidSendData(' ');
				}
			}

			else
			Local_u8Error_state = NOK;
		}
		else
		Local_u8Error_state = NOK;
	}
	else
	Local_u8Error_state = NULL_PTR;
	return Local_u8Error_state;
}

uint8 PP_u8PlayerToggle(PP_player_Config_t *ref_player)
{
	uint8 Local_u8Error_state = OK;
	if (ref_player != NULL)
	{

		uint8 Local_u8PosX;
		CLCD_VoidSendCommand(CLCD_CLEAR_CMD);
		// get player x location
		if (ref_player->screenPos == LEFT)
		Local_u8PosX = 0;
		else
		Local_u8PosX = 15;
		CLCD_voidGoToXY(Local_u8PosX, ref_player->pos);
		CLCD_VoidSendData(' ');
		// new pos
		if (ref_player->pos == 0)

		ref_player->pos = 1;

		else
		ref_player->pos = 0;
		// display new pos
		CLCD_voidGoToXY(Local_u8PosX, ref_player->pos);
		CLCD_VoidSendData('|');

	}
	else
	Local_u8Error_state = NULL_PTR;

	return Local_u8Error_state;
}

uint8 PP_u8BallArrive(PP_ball_Config_t *ref_ball, PP_player_Config_t *ref_player, uint8 *ref_u8GameState)
{
	uint8 Local_u8Error_state = OK;
	if (ref_ball != NULL && ref_player != NULL)
	{
		// player hit the ball
		if (ref_player->pos == ref_ball->posY)
		{
			// toggle ball direction
			if (ref_ball->WalkDirection == RIGHT)
			ref_ball->WalkDirection = LEFT;
			else
			ref_ball->WalkDirection = RIGHT;
			//move the ball in opposite direction
			PP_u8BallMoving(ref_ball, ref_player);
			*ref_u8GameState = 1U; // game continue
		}
		// player missed the ball
		else
		{
			*ref_u8GameState = 0U;
		}
	}
	else
	Local_u8Error_state = NULL_PTR;

	return Local_u8Error_state;
}

uint8 PP_u8NotifyMC_ball(PP_ball_Config_t* ref_ball){

	uint8 Local_u8Error_state = OK;
	uint8 Local_u8Iter0 ;
	if (ref_ball != NULL )
	{
		//ball in pos 0 INT0=3
		if(ref_ball->posY == 0){
			EXTI_u8IntDisable(INT0);
			for(Local_u8Iter0=0;Local_u8Iter0<3;Local_u8Iter0++){
				DIO_u8_SetPinDir(DIO_PORTD,DIO_PIN2,DIO_PIN_OUTPUT);
				DIO_u8_SetPinVal(DIO_PORTD,DIO_PIN2,DIO_PIN_HIGH);

				DIO_u8_SetPinVal(DIO_PORTD,DIO_PIN2,DIO_PIN_LOW);
				DIO_u8_SetPinDir(DIO_PORTD,DIO_PIN2,DIO_PIN_INPUT);
				_delay_us(50);
			}

			EXTI_u8IntEnable(INT0);
		}
		else if(ref_ball->posY == 1){
			//ball in pos 1 INT0=2
			EXTI_u8IntDisable(INT0);
			for(Local_u8Iter0=0;Local_u8Iter0<2;Local_u8Iter0++){
				DIO_u8_SetPinDir(DIO_PORTD,DIO_PIN2,DIO_PIN_OUTPUT);
				DIO_u8_SetPinVal(DIO_PORTD,DIO_PIN2,DIO_PIN_HIGH);

				DIO_u8_SetPinVal(DIO_PORTD,DIO_PIN2,DIO_PIN_LOW);
				DIO_u8_SetPinDir(DIO_PORTD,DIO_PIN2,DIO_PIN_INPUT);
				_delay_us(50);
			}
			EXTI_u8IntEnable(INT0);
		}
		else
		Local_u8Error_state = NOK;
	}
	else
	Local_u8Error_state = NULL_PTR;

	return Local_u8Error_state;
}

void PP_voidNotifyMCGameEnd(){
	// game end INT0=1
	EXTI_u8IntDisable(INT0);
	DIO_u8_SetPinDir(DIO_PORTD,DIO_PIN2,DIO_PIN_OUTPUT);
	DIO_u8_SetPinVal(DIO_PORTD,DIO_PIN2,DIO_PIN_HIGH);

	DIO_u8_SetPinVal(DIO_PORTD,DIO_PIN2,DIO_PIN_LOW);
	DIO_u8_SetPinDir(DIO_PORTD,DIO_PIN2,DIO_PIN_INPUT);
	EXTI_u8IntEnable(INT0);
}

void PP_voidGameEnd(uint8 copy_winStatus){
	//lose
	uint8 Local_u8Iter0;
	if (copy_winStatus == 0){
		for (Local_u8Iter0 = 0; Local_u8Iter0 < 5; Local_u8Iter0++)
		{
			// clear screen
			CLCD_VoidSendCommand(CLCD_CLEAR_CMD);

			// flashing player loos
			_delay_ms(100);
			CLCD_voidGoToXY(0u, 0u);
			CLCD_VoidPrintChars("  game over ...");
			CLCD_voidGoToXY(0u, 1u);
			CLCD_VoidPrintChars(" you loss _-_");
			_delay_ms(100);
		}
	}
	//win
	else{
		for (Local_u8Iter0 = 0; Local_u8Iter0 < 5; Local_u8Iter0++)
		{
			// clear screen
			CLCD_VoidSendCommand(CLCD_CLEAR_CMD);

			// flashing player loos
			_delay_ms(100);
			CLCD_voidGoToXY(0u, 0u);
			CLCD_VoidPrintChars("  game over ...");
			CLCD_voidGoToXY(0u, 1u);
			CLCD_VoidPrintChars(" you win *_*");
			_delay_ms(100);
		}
	}
}

uint8 PP_voidCheckINTFlag(uint8* ref_Flag,PP_ball_Config_t* ref_ball,PP_player_Config_t* ref_player){
	/*	check the interrupts flag
	 * INT0
	 * 3 ball in posY0
	 * 2 ball in posY1
	 * 1 game end
	 *  */
	uint8 Local_u8Error_state = OK;
		if ((ref_ball != NULL  )&& (*ref_Flag!=NULL) &&(ref_player !=NULL))
		{
			if (*ref_Flag== 1){
				_delay_ms(50);
				switch(*ref_Flag){
				case 1:
					//reset flag to 0;
					*ref_Flag= 0;
					ref_ball->flag= OUTSCREEN;
					PP_voidGameEnd(1);
					break;
				case 2:
					// reset flag to 0;
					*ref_Flag= 0;
					ref_ball->posY = 1;
					ref_ball->flag = INSCREEN;

					break;
				case 3:
					// reset flag to 0;
					*ref_Flag = 0;
					ref_ball->posY =0;
					ref_ball->flag = INSCREEN;

					break;
				default: break;
				}
				//load ball data
				if(ref_player->screenPos == LEFT){
					ref_ball->WalkDirection = LEFT;
					ref_ball->posX = 15;
				}
				else{
					ref_ball->WalkDirection = RIGHT;
					ref_ball->posX = 0;
				}

		}
		}else
			Local_u8Error_state = NULL_PTR;
		return Local_u8Error_state;
}

uint8 PP_u8GameRun(PP_ball_Config_t *ref_ball, PP_player_Config_t *ref_player){
	uint8 Local_u8Error_state = OK;
	if (ref_ball != NULL && ref_player != NULL)
	{
		uint8 Local_u8GameState = 0;
		if (PP_u8BallMoving(ref_ball, ref_player) == OK)
		{
			// check if the ball arrive to the end of screen
			if (PP_u8BallArrive(ref_ball, ref_player, &Local_u8GameState) == OK)
			{
				//player hit the ball and return it
				if (Local_u8GameState == 1U)
				{
					PP_u8NotifyMC_ball(ref_ball);
					//increase playing level by increasing the speed
					if(ref_ball->speed-25 >0)
						ref_ball->speed -=25;
				}
				// player missed the ball
				else if (Local_u8GameState == 0U)
				{
					ref_ball->flag= OUTSCREEN;
					PP_voidNotifyMCGameEnd();
					PP_voidGameEnd(0);

				}
			}
		}
	}else
		Local_u8Error_state = NULL_PTR;
	return Local_u8Error_state;
}
