#include <stdio.h>
#include "villian.h"

/**************************************************************************************/
/*                                                                                    */
/*                                VILLIAN BACKEND                                     */
/*                                                                                    */
/**************************************************************************************/

bool UploadVillianFrames(struct Villian *v, const char *v_path)
{
    char filename[256];
    for(int i = 0; i < VILLIAN_TOTAL_FRAMES; i++)
    {
        snprintf(filename, sizeof(filename), "%s/%d.png", v_path, i);
        v->Frames[i] = al_load_bitmap(filename);
        if(!v->Frames[i])
        {
            printf("[ERRO]: UPLOAD VILLIAN FRAMES %s\n", filename);
            return false;
        }
    }

    v->Current_frame = 0;
    v->Frame_timer = 0.0f;
    v->Frame_duration = 0.15f;
    return true;
}

struct Villian* CreateVillian(struct Position position, Difficult difficult, VillianType villian_type, char name[50], const char *villian_path)
{
    if(!IsValidPosition(position)) return NULL;

    struct Villian *v = malloc(sizeof(struct Villian));
    if(!v)
    {
        printf("[ERROR]: CREATING VILLIAN!");
        return NULL;
    }
    v->Position = position;

    int cooldown;
    if(difficult == MEDIUM)
        cooldown = VILLIAN_MEDIUN_COOLDOWN;
    
    else if(difficult == EASY)
        cooldown = VILLIAN_EASY_COOLDOWN;       
    
    else if(difficult == HARD)
        cooldown = VILLIAN_HARD_COOLDOWN;

    v->Gun = GunCreate(cooldown);
    if(!v->Gun) return NULL;

    v->Type = villian_type;
    v->QtdLife = villian_type == BOSS ? (VILLIAN_LIFE + 1) * 2 : VILLIAN_LIFE;
    v->CurrentLife = v->QtdLife;
    v->Life = malloc(sizeof(struct Life) *  v->QtdLife);
    if(!v->Life)
    {
        printf("[ERROR]: ALLOCATE LIFE");
        return NULL;
    }

    for(int i = 0; i < v->QtdLife; i++)
        v->Life[i] = CreateLife();
        
    strncpy(v->Name, name, sizeof(v->Name));
    v->Name[sizeof(v->Name) - 1] = '\0';

    if(!UploadVillianFrames(v, villian_path))
        return NULL;

    return v;
}

bool ExistColision(struct Villian *villian, int player_x)
{
    int villian_x_res = villian->Type == BOSS ? VILLIAN_BOSS_X_RESOLUTION : VILLIAN_SUB_X_RESOLUTION;

    if (villian->Position.X + villian_x_res < player_x) return false;
    if (villian->Position.X > player_x + PLAYER_X_RESOLUTION) return false; 

    return true; 
}

void DestroyVillian(struct Villian *villian)
{
    for(int i = 0; i < FRAME_COUNT; i++)
    {
        if(villian->Frames[i])
            al_destroy_bitmap(villian->Frames[i]);
    }

    GunDestroy(villian->Gun);
    DestroyLife(villian->Life);
    free(villian->Life);
    free(villian);
}

void MoveVillian(struct Villian *villian, int player_x, int background_x_init, int background_x_fim)
{
    const int speed = 4;
    int villian_x_res = villian->Type == BOSS ? VILLIAN_BOSS_X_RESOLUTION : VILLIAN_SUB_X_RESOLUTION;
    int villian_y_res = villian->Type == BOSS ? VILLIAN_BOSS_Y_RESOLUTION : VILLIAN_SUB_Y_RESOLUTION; 

    //Player na frente, villao pra dir 
    if(player_x > villian->Position.X + villian_x_res && villian->Position.X + villian_x_res < X_BACKGROUND && background_x_init <= (villian->Position.X + villian_y_res))
    {
        villian->Face = 1;
        if(!ExistColision(villian, player_x))
            villian->Position.X += speed;
    }
            
    //Player atrás, villao pra esq
    if(player_x < villian->Position.X && villian->Position.X > 0 && background_x_fim >= villian_x_res)
    {
        villian->Face = 0;
        if(!ExistColision(villian, player_x))
            villian->Position.X -= speed;
    }

    if(villian->Position.X < 0 || villian->Position.X > X_BACKGROUND)
        villian->Position.X = 2;

    if(villian->Position.Y < 0)
        villian->Position.Y = 0;

    if(villian->Position.Y + villian_y_res >= Y_SCREEN)
        villian->Position.Y = Y_SCREEN - villian_y_res;

    //se janela aponta pro inicio ou fim do vilão dispara
    if(villian->Position.X + villian_x_res > background_x_init && villian->Position.X < background_x_fim)
    {
        if(!villian->Gun->Timer)
        {
            ShotVillian(villian);
            villian->Gun->Timer = villian->Gun->Cooldown;
        }
    }
}

void ShotVillian(struct Villian *villian)
{
    int villian_x_res = villian->Type == BOSS ? VILLIAN_BOSS_X_RESOLUTION : VILLIAN_SUB_X_RESOLUTION;
    struct Bullet *shot = NULL;
    if(villian->Face == 0) shot = GunShot(villian->Position.X, villian->Position.Y, villian->Face, villian->Gun);
    if(villian->Face == 1) shot = GunShot(villian->Position.X + villian_x_res, villian->Position.Y, villian->Face, villian->Gun);
    
    if(shot){
        shot->Next = villian->Gun->Shots;
        villian->Gun->Shots = shot;
    }

    if(!shot) printf("[ERRO]: CRATING SHOT");
}


/**************************************************************************************/
/*                                                                                    */
/*                                 VILLIAN FRONTEND                                   */
/*                                                                                    */
/**************************************************************************************/

void UpdateVillianFrames(struct Villian *v, float delta_timer)
{
    v->Frame_timer += delta_timer;
    if(v->Frame_timer >= v->Frame_duration)
    {
        v->Frame_timer = 0.0f;
        v->Current_frame = (v->Current_frame + 1) % VILLIAN_FRAME_COUNT;
    }
}

void DrawVillian(struct Villian *villian, ALLEGRO_FONT *player_font, int background_x)
{
    int draw_x = villian->Position.X - background_x;
    int base_index = 0;
    int villian_x_res = villian->Type == BOSS ? VILLIAN_BOSS_X_RESOLUTION : VILLIAN_SUB_X_RESOLUTION;
    int villian_y_res = villian->Type == BOSS ? VILLIAN_BOSS_Y_RESOLUTION : VILLIAN_SUB_Y_RESOLUTION;

    if(!villian->Face)
        base_index = 4;

    if(villian->Face == 1)
        base_index = 0;

    int frame_index = base_index + villian->Current_frame;
    
    int life_x = draw_x;
    for(int i = 0; i < villian->QtdLife; i++)
    {
        life_x += LIFE_X_RESOLUTION; //desenhar um do lado do outro
        al_draw_bitmap(villian->Life[i].Frames[villian->Life[i].CurrentFrame], life_x - 50, villian->Position.Y - 50, 0);
    }

    al_draw_bitmap(villian->Frames[frame_index], draw_x, villian->Position.Y, 0);
    al_draw_text(player_font, al_map_rgb(0,0,0), draw_x + (villian_x_res/2 - 20), villian->Position.Y + villian_y_res, 0, villian->Name);
}

void DrawVillianBullet(struct Villian *villian, struct Escene background)
{
 	for(struct Bullet *index = villian->Gun->Shots; index != NULL; index = (struct Bullet*) index->Next)
    {
        int draw_x = index->X - background.Init_x;
        int draw_y = index->Y;

        if(villian->Type == SUB)
            al_draw_filled_circle(draw_x, draw_y + 30, 10, al_map_rgb(255,194,34));
        else
            al_draw_filled_circle(draw_x, draw_y + 30, 20, al_map_rgb(255,0,0));
    }
    
    if(villian->Gun->Timer)  
        villian->Gun->Timer--;
}

