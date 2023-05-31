#ifndef PP_INTERFACE_H
#define PP_INTERFACE_H

#define INSCREEN 1u
#define OUTSCREEN 2u

#define LEFT 1u
#define RIGHT 2u


typedef struct PP_ball_Config_t
{
    uint8 posX;
    uint8 posY;
    uint8 flag;// 1-inscreen 2-outScreen
    uint32 speed;
    uint8 WalkDirection; //1-left 2-right 
}PP_ball_Config_t;

typedef struct PP_player_Config_t{
    uint8 pos;
    const uint8 screenPos;//1-left 2-right 
}PP_player_Config_t;

typedef struct PP_Flags_Config_t{
    uint8 INT_0FLAG ;
    uint8 INT_1FLAG ;

}PP_Flags_Config_t;

uint8 PP_u8initPlayer(PP_player_Config_t *ref_player);

uint8 PP_u8BallMoving(PP_ball_Config_t* referance_ball,PP_player_Config_t* referance_player);
uint8 PP_u8PlayerToggle(PP_player_Config_t* referance_player);
uint8 PP_u8NotifyMC_ball(PP_ball_Config_t* referance_ball);
void PP_voidNotifyMCGameEnd();
void PP_voidGameEnd(uint8 copy_winStatus); //0-lose 1 -win
uint8 PP_u8BallArrive(PP_ball_Config_t* referance_ball,PP_player_Config_t* referance_player,uint8* refrance_u8GameState);
uint8 PP_voidCheckINTFlag(uint8* referance_Flag,PP_ball_Config_t* referance_ball,PP_player_Config_t* ref_player);
uint8 PP_u8GameRun(PP_ball_Config_t *ref_ball, PP_player_Config_t *ref_player);
#endif

