#include <stdio.h>
#include <direct.h>
#include "player.h"
#include "../gun/gun.h"
#include "../../services/joystick/joystick.h"

/**************************************************************************************/
/*                                                                                    */
/*                                 PLAYER BACKEND                                     */
/*                                                                                    */
/**************************************************************************************/

bool UploadFrames(struct Player *player, const char *player_path)
{
    char filename[256];
    for(int i = 0; i < TOTAL_FRAMES; i++)
    {
        snprintf(filename, sizeof(filename), "%s/%d.png", player_path, i);
        player->Frames[i] = al_load_bitmap(filename);
        if(!player->Frames[i])
        {
            printf("[ERRO]: UPLOAD FRAMES %s\n", filename);
            return false;
        }
    }

    player->Current_frame = 0;
    player->Frame_timer = 0.0f;
    player->Frame_duration = 0.15f;
    return true;
}

struct Player* CreatePlayer(struct Position position, const char *player_path, const char *name)
{
    if(!IsValidPosition(position)) return NULL;

    struct Player *player = malloc(sizeof(struct Player));
    if(!player)
    {
        printf("[ERROR]: CREATING PLAYER!");
        return NULL;
    }

    player->Gun = GunCreate(PLAYER_COOLDOWN);
    if(!player->Gun) return NULL;

    player->Control = JoystickCreate();
    if(!player->Control) return NULL;

    player->QtdLife = PLAYER_LIFE;
    player->CurrentLife = player->QtdLife;
    player->Life = malloc(sizeof(struct Life) * player->QtdLife);
    if(!player->Life)
    {
        printf("[ERRRO]: ALLOCATE PLAYER LIFE!");
        return NULL;
    }

    for(int i = 0; i < player->QtdLife; i++)
        player->Life[i] = CreateLife();

    player->Position = position;
    player->Face = 1;
    player->IsJumping = false;
    player->VelJump = 0;
    player->IsCrouching = false;

    strncpy(player->Name, name, sizeof(player->Name));
    player->Name[sizeof(player->Name) - 1] = '\0';

    if(!UploadFrames(player, player_path))
        return NULL;

    return player;
}

void DestroyPlayer(struct Player *player)
{
    for(int i = 0; i < FRAME_COUNT; i++)
    {
        if(player->Frames[i])
            al_destroy_bitmap(player->Frames[i]);
    }

    GunDestroy(player->Gun);
    DestroyLife(player->Life);
    free(player->Life);
    JoystickDestroy(player->Control);
    free(player);
}

void MovePlayer(struct Player *player)
{
    const int speed = 4;
    const int gravity = 2;
    const int jump_strenght = -40;

    //Player mov pra esq
    if(player->Control->left && player->Position.X > 0)
    {
        player->Face = 0;
        player->Position.X -= speed;
    }
        
    //Player mov para dir
    if(player->Control->right && player->Position.X + PLAYER_X_RESOLUTION < X_BACKGROUND)
    {
        player->Face = 1;
        player->Position.X += speed;
    }
        
    //Limite do player com laterar esq da tela
    if(player->Position.X < 0 || player->Position.X > X_BACKGROUND)
        player->Position.X = 2;

    //Player salto
    if(player->Control->up && player->Position.Y > 0 && !player->IsJumping)
    {
        if(player->IsCrouching)
            player->IsCrouching = false;
        else
        {
            player->VelJump = jump_strenght;
            player->IsJumping = true;
        }
    }

    //Player pra abaixo
    player->IsCrouching = player->Control->down;

    if(player->Position.Y < 0)
    {
        player->Position.Y = 0;
        player->VelJump = 0;
    }
        
    //Player desce do salto
    player->VelJump += gravity;
    player->Position.Y += player->VelJump;

    //Player limite chão
    if(player->Position.Y + PLAYER_Y_RESOLUTION >= Y_SCREEN)
    {
        player->Position.Y = Y_SCREEN - PLAYER_Y_RESOLUTION;
        player->VelJump = 0;
        player->IsJumping = false;
    }
    
    //Player atira
    if (player->Control->fire)
    {
        if(!player->Gun->Timer)
        {
            ShotPlayer(player);
            player->Gun->Timer = player->Gun->Cooldown;
        }   
    }
    printf("player Y: %d\n", player->Position.Y);
    printf("player X: %d\n", player->Position.X);
}

void ShotPlayer(struct Player *player)
{
    struct Bullet *shot = NULL;
    if(!player->Face) shot = GunShot(player->Position.X + 42, player->Position.Y + 120, player->Face, player->Gun);
    if(player->Face == 1) shot = GunShot(player->Position.X + (PLAYER_X_RESOLUTION - 42), player->Position.Y + 120, player->Face, player->Gun);
    if(shot){
        shot->Next = player->Gun->Shots;
        player->Gun->Shots = shot;
    }

    if(!shot) printf("[ERRO]: CREATING SHOT");
}

/**************************************************************************************/
/*                                                                                    */
/*                                 PLAYER FRONTEND                                    */
/*                                                                                    */
/**************************************************************************************/

void UpdatePlayerFrames(struct Player *player, float delta_timer)
{
    player->Frame_timer += delta_timer;
    if(player->Frame_timer >= player->Frame_duration)
    {
        player->Frame_timer = 0.0f;
        player->Current_frame = (player->Current_frame + 1) % FRAME_COUNT;
    }
}

void DrawPlayer(struct Player *player, int background_x, ALLEGRO_FONT *player_font)
{
    int draw_x = player->Position.X - background_x;
    int base_index = 0;
    if(!player->IsCrouching && player->Face == 1) //pé olhando direita
        base_index = 0;

    else if(!player->IsCrouching && !player->Face) //pé olhando esq
        base_index = 4;

    else if(player->IsCrouching && player->Face == 1) //abaixo dir
        base_index = 8;

    else if(player->IsCrouching && !player->Face) //abaixo esq
        base_index = 12;
    
    int frame_index = base_index + player->Current_frame;
    
    int life_x = draw_x;
    for(int i = 0; i < player->QtdLife; i++)
    {
        life_x += LIFE_X_RESOLUTION;
        al_draw_bitmap(player->Life[i].Frames[player->Life[i].CurrentFrame], life_x, player->Position.Y, 0);
    }

    al_draw_bitmap(player->Frames[frame_index], draw_x, player->Position.Y, 0);
    al_draw_text(player_font, al_map_rgb(0,0,0), draw_x + (PLAYER_X_RESOLUTION/2 - 20), player->Position.Y + PLAYER_Y_RESOLUTION, 0, player->Name);
}

void DrawPlayerBullet(struct Player *player, struct Escene background)
{
 	for(struct Bullet *index = player->Gun->Shots; index != NULL; index = (struct Bullet*) index->Next)
    {
        int draw_x = index->X - background.Init_x;
        int draw_y = index->Y;
        al_draw_filled_circle(draw_x, draw_y, 10, al_map_rgb(0,0,0));
    }
    
    if(player->Gun->Timer)  
        player->Gun->Timer--;
}

