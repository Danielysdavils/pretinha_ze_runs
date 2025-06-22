#ifndef __CONFIG__
#define __CONFIG__

#include <allegro5/allegro.h>	
#include <allegro5/allegro_ttf.h>																																						//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_image.h>																																							//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

/* DISPLAY - CONFIG DATA */
#define X_SCREEN 1000 //width da janela do jogo																																											//Definição do tamanho da tela em pixels no eixo x (!)
#define Y_SCREEN 700 //heigth da janela do jogo
#define X_BACKGROUND 3124 //Width total da imagem do fundo

/* PLAYER - CONFIG DATA */
#define PLAYER_PRETINHA "assets/players/pretinha"
#define PLAYER_X_MARGIN 110
#define PLAYER_Y_MARGIN 55
#define PLAYER_X_RESOLUTION 360
#define PLAYER_Y_RESOLUTION 274
#define PLAYER_COOLDOWN 10

/* VILIANS - CONFIG DATA */
#define VILLIAN_SUB_01 "assets/players/villian/sub_01"
#define VILLIAN_SUB_02 "assets/players/villian/sub_02"

#define VILLIAN_BOSS_01 "assets/players/villian/boss_01"
#define VILLIAN_BOSS_02 "assets/players/villian/boss_02"

#define VILLIAN_EASY_COOLDOWN 70
#define VILLIAN_MEDIUN_COOLDOWN 40
#define VILLIAN_HARD_COOLDOWN 20

#define VILLIAN_SUB_X_RESOLUTION 185
#define VILLIAN_SUB_Y_RESOLUTION 141

#define VILLIAN_BOSS_X_RESOLUTION 280
#define VILLIAN_BOSS_Y_RESOLUTION 270

/* ELEMENTS - CONFIG DATA */
#define LIFE_PATH "assets/elements/life"
#define LIFE_X_RESOLUTION 65

/* MENU - CONFIG DATA */
#define MENU_BACKGROUND "assets/backgrounds/menu.png"
#define MENU_SONG "assets/music/song_menu.ogg"
#define MENU_FONT "assets/fonts/font2.ttf"
#define EMOJI_FONT "assets/fonts/emoji.ttf"

/* LOSE - CONFIG DATA */
#define LOSE_SONG "assets/music/lose.ogg"

/* WINN - CONFIG DATA */
#define WINN_BACKGROUND "assets/backgrounds/winn.png"
#define WINN_SONG "assets/music/winn.ogg"
#define WINN_ELEMENT "assets/elements/winn/winn.png"

/* FASE 0 - CONFIG DATA */
#define FASE_0_BACKGROUND "assets/backgrounds/fase_1.png"
#define FASE_0_MUSIC "assets/music/fase_0.ogg"
#define FASE_FONT "assets/fonts/font3.ttf" 

/* FASE 1 - CONFIG DATA */
#define FASE_1_BACKGROUND "assets/backgrounds/fase_2.png"
#define FASE_1_MUSIC "assets/music/fase_1_re.ogg"

/* FASE ENDGAME - CONFIG DATA */
#define FASE_ENDGAME 1 //numero de fase na qual o jogo termina

/* DIFFICULT CONFIGS */
#define EASY_QTD_VILLAINS 7
#define EASY_QTD_ELEMENTS 3
#define MEDIUM_QTD_VILLAINS 10
#define MEDIUM_QTD_ELEMENTS 5
#define HARD_QTD_VILLIANS 15
#define HARD_QTD_ELEMENTS 10

typedef enum {
    STATE_MENU,
    STATE_STARTGAME,
    STATE_PAUSE,
    STATE_EXIT

} GameState;

typedef enum {
    COMPLEATED,
    LOST,
    CANCELLED
    
} FaseState;


#endif