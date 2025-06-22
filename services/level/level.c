#include <stdio.h>
#include "../../config/config.h"
#include "../../entities/level/level.h"
#include "../../entities/players/player.h"
#include "../../entities/villains/villian.h"
#include "../../entities/utilities/difficult.h"
#include "../../entities/utilities/position.h"

#define DELTA_TIMER (1.0f / 30.0f)

/**************************************************************************************/
/*                                                                                    */
/*                                 PLAYER FRONTEND                                    */
/*                                                                                    */
/**************************************************************************************/

void UpdateBullet(struct Bullet *index)
{
    if(!index->Trajectory) index->X -= BULLET_MOVE;
    else if(index->Trajectory == 1) index->X += BULLET_MOVE;
}

void CheckVillianShotColision(struct Villian *villian, struct Player *player, struct Escene background)
{
    struct Bullet *prev = NULL;
    struct Bullet *curr = villian->Gun->Shots;

    while (curr != NULL)
    {
        //UpdateBullet(curr);
        if(!curr->Trajectory) curr->X -= BULLET_MOVE;
        else if(curr->Trajectory == 1) curr->X += BULLET_MOVE;

        bool hit = false;

        // Verifica se a bala atingiu o jogador
        if ((curr->X >= player->Position.X && curr->X <= player->Position.X + PLAYER_X_RESOLUTION) &&
            (curr->Y >= player->Position.Y && curr->Y <= player->Position.Y + PLAYER_Y_RESOLUTION))
        {
            player->CurrentLife--;
            LoseLife(player->Life, player->QtdLife);
            hit = true;
        }

        if (hit)
        {
            struct Bullet *to_delete = curr;

            if (prev)
            {
                prev->Next = curr->Next;
                curr = (struct Bullet *)prev->Next;
            }
            else
            {
                villian->Gun->Shots = (struct Bullet *)curr->Next;
                curr = villian->Gun->Shots;
            }

            BulletDestroy(to_delete); 
        }
        else
        {
            prev = curr;
            curr = (struct Bullet *)curr->Next;
        }
    }
}

void CheckPlayerShotColision(struct Player *player, struct Fase *fase, struct Escene background)
{
    struct Bullet *prev = NULL;
    struct Bullet *curr = player->Gun->Shots;

    while (curr != NULL)
    {
        //UpdateBullet(curr);
        if(!curr->Trajectory) curr->X -= BULLET_MOVE;
        else if(curr->Trajectory == 1) curr->X += BULLET_MOVE;

        bool hit = false;
        bool out_the_escene = (curr->X < background.Init_x || curr->X > background.Fim_X); //case a bala esteja fora da janela

        // Verifica colisão com todos os vilões
        for (int i = 0; i < fase->QtdVilhoes; i++)
        {
            struct Villian *v = fase->Villian[i];
            if (!v)
                continue;

            if ((curr->X >= v->Position.X && curr->X <= v->Position.X + VILLIAN_SUB_X_RESOLUTION) &&
                (curr->Y >= v->Position.Y && curr->Y <= v->Position.Y + VILLIAN_SUB_Y_RESOLUTION))
            {
                v->CurrentLife--;
                LoseLife(v->Life, v->QtdLife);
                
                if(v->CurrentLife == 0)
                {
                    DestroyVillian(v);
                    fase->Progress.QtdKilledVillians++;
                    fase->Villian[i] = NULL;
                }

                hit = true;
                break;
            }
        }

        if (hit || out_the_escene)
        {
            struct Bullet *to_delete = curr;
            if (prev)
            {
                prev->Next = curr->Next;
                curr = (struct Bullet *)prev->Next;
            }
            else
            {
                player->Gun->Shots = (struct Bullet *)curr->Next;
                curr = player->Gun->Shots;
            }

            BulletDestroy(to_delete);
        }
        else
        {
            prev = curr;
            curr = (struct Bullet *)curr->Next;
        }
    }
}

void UpdateFase(struct Fase *fase)
{
    printf("fase update %s\n", fase->Name);
    
    MovePlayer(fase->Player);
    UpdateEscene(&fase->Background, fase->Player->Position.X, X_BACKGROUND, X_SCREEN);
    UpdatePlayerFrames(fase->Player, DELTA_TIMER);
    CheckPlayerShotColision(fase->Player, fase, fase->Background);

    for(int i = 0; i < fase->QtdVilhoes; i++)
    {
        struct Villian *v = fase->Villian[i];
        if(v != NULL)
            printf("VILLIAN: [%d]: Name: %s POS_X: %d\n", i, v->Name, v->Position.X);
        else
            printf("VILLIAN: [%d]: NULL\n", i);
    }

    for(int i = 0; i < fase->CurrentActiveVillians; i++)
    {
        struct Villian *v = fase->Villian[i];
        if(v != NULL)
        {
            printf("POSIION: %d NOME:%s\n", v->Position.X, v->Name); 
            MoveVillian(v, fase->Player->Position.X, fase->Background.Init_x, fase->Background.Fim_X);
            UpdateVillianFrames(v, DELTA_TIMER);
            CheckVillianShotColision(v, fase->Player, fase->Background);
        }
        else{
            printf("INDEX %d -> null\n", i);
        }
    }

    if(fase->Progress.QtdKilledVillians == fase->CurrentActiveVillians && fase->CurrentActiveVillians <= fase->QtdVilhoes)
        fase->CurrentActiveVillians++;

}

void DrawFase(struct Fase *fase, ALLEGRO_BITMAP *bg, ALLEGRO_AUDIO_STREAM *music, ALLEGRO_FONT *font, ALLEGRO_FONT *player_font)
{
    al_draw_bitmap_region(bg, fase->Background.Init_x, 0, X_SCREEN, Y_SCREEN, 0, 0, 0);
    al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
    al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
    al_draw_text(font, al_map_rgb(0,0,0), X_SCREEN/2 - 20, 90, 0, fase->Name);

    DrawPlayer(fase->Player, fase->Background.Init_x, player_font);
    DrawPlayerBullet(fase->Player, fase->Background);

    //Caso algum vilhão seja criado o renderizo
    for(int i = 0; i < fase->CurrentActiveVillians; i++)
    {
        struct Villian *v = fase->Villian[i];
        if(v != NULL)
        {
            printf("POSIION: %d NOME:%s\n", v->Position.X, v->Name);
            DrawVillian(v, player_font, fase->Background.Init_x);
            DrawVillianBullet(v, fase->Background);
        }
        else{
            printf("INDEX %d -> null\n", i);
        }
    }

    if(fase->Progress.QtdKilledVillians == fase->CurrentActiveVillians && fase->CurrentActiveVillians <= fase->QtdVilhoes)
        fase->CurrentActiveVillians++;
}


/**************************************************************************************/
/*                                                                                    */
/*                                   FASE BACKEND                                     */
/*                                                                                    */
/**************************************************************************************/

struct Fase* CreateFase(Difficult level_dificult, int level, const char *player_basePath, char *player_name)
{
    struct Fase *fase = malloc(sizeof(struct Fase));
    switch (level_dificult)
    {
        case EASY:
            fase->QtdVilhoes = EASY_QTD_VILLAINS;
        break;
    
        case MEDIUM:
            fase->QtdVilhoes = MEDIUM_QTD_VILLAINS;
        break;

        case HARD:
            fase->QtdVilhoes = HARD_QTD_VILLIANS;
        break;
    };

    fase->Villian = calloc(fase->QtdVilhoes, sizeof(struct Villian *)); //villians vetor: calloc para incializar zerado
    if(!fase->Villian)
    {
        printf("[ERROR]: VILLIANS VETOR FASE CREATION");
        return NULL;
    }

    //Crea jogador e determina posição inical na tela
    struct Position player_pos;
    player_pos.X = PLAYER_X_MARGIN;
    player_pos.Y = Y_SCREEN - PLAYER_Y_RESOLUTION - PLAYER_Y_MARGIN;
    struct Player *player = CreatePlayer(player_pos, player_basePath, player_name);
    if(!player)
    {
        printf("[ERROR]: CREATING PLAYER\n");
        return NULL;
    }

    fase->Progress.Level = level;
    fase->Progress.QtdKilledVillians = 0;
    fase->Player = player; 
    const char *sub_villian_path;
    const char *boss_villian_path;

    snprintf(fase->Name, sizeof(fase->Name), "FASE %d", level);
    switch (level)
    {
        case 0:
            fase->Background_image = FASE_0_BACKGROUND;
            fase->Music = FASE_0_MUSIC;
            fase->SubVillianPath = VILLIAN_SUB_01;
            fase->BossVillianPath = VILLIAN_BOSS_01;
        break;

        case 1:
            fase->Background_image = FASE_1_BACKGROUND;
            fase->Music = FASE_1_MUSIC;
            fase->SubVillianPath = VILLIAN_SUB_02;
            fase->BossVillianPath = VILLIAN_BOSS_02;
        break;    
    }

    fase->CurrentActiveVillians = (fase->QtdVilhoes - 1) / 2;
    int vil_y_pos = Y_SCREEN - VILLIAN_SUB_Y_RESOLUTION - VILLIAN_MARGIN;
    for(int i = 0; i < fase->CurrentActiveVillians; i++)
    {
        struct Position pos;
        pos.X = GenerateAleatValue(X_SCREEN, X_BACKGROUND);
        pos.Y = vil_y_pos;
        fase->Villian[i] = CreateVillian(pos, level_dificult, SUB, "CARECA", fase->SubVillianPath);
        if(!fase->Villian[i])
        {
            printf("[ERRO]: VILLAIN CREATION");
            return NULL;
        }   

        printf("creando [%d]: nome: %s. pos_x: %d\n", i, fase->Villian[i]->Name, fase->Villian[i]->Position.X);
    }

    for(int i = fase->CurrentActiveVillians; i < fase->QtdVilhoes - 1; i++)
    {
        struct Position pos;
        pos.X = GenerateAleatValue(1, X_BACKGROUND);
        pos.Y = vil_y_pos;
        fase->Villian[i] = CreateVillian(pos, level_dificult, SUB, "CARECA", fase->SubVillianPath);
        if(!fase->Villian[i])
        {
            printf("[ERRO]: VILLAIN CREATION");
            return NULL;
        }   
    }

    struct Position pos;
    pos.X = GenerateAleatValue(1, X_BACKGROUND);
    pos.Y = vil_y_pos;
    fase->Villian[fase->QtdVilhoes - 1] = CreateVillian(pos, level_dificult, BOSS, "BOSS", fase->BossVillianPath);
    if(!fase->Villian[fase->QtdVilhoes])
    {
        printf("[ERRO]: BOSS CREATION");
        return NULL;
    }    

    fase->Background.Init_x = 1;
    fase->Background.Fim_X = 999;
    return fase;
}

FaseState PlayFase(struct Fase *fase, GameState *state, ALLEGRO_EVENT_QUEUE *event_queue)
{
    bool running = true;
    FaseState fase_state = LOST;

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    ALLEGRO_BITMAP *bg = al_load_bitmap(fase->Background_image);
    ALLEGRO_AUDIO_STREAM *music = al_load_audio_stream(fase->Music, 4, 2048);
    ALLEGRO_FONT* font = al_load_font(FASE_FONT, 40, 0);
    ALLEGRO_FONT* player_font = al_load_font(FASE_FONT, 20, 0);

    while(running)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            UpdateFase(fase);

            //Fase termina caso player fique sem vidas
            if(fase->Player->CurrentLife <= 0)
            {
                fase_state = LOST;
                break;
            }

            //Fase é completada caso player elimine todos os vilhoes da fase
            if(fase->Player->CurrentLife != 0 && fase->Progress.QtdKilledVillians == fase->QtdVilhoes)
            {
                fase_state = COMPLEATED;
                break;
            }

            al_clear_to_color(al_map_rgb(0,0,0));
            UpdateEscene(&fase->Background, fase->Player->Position.X, X_BACKGROUND, X_SCREEN);
            DrawFase(fase, bg, music, font, player_font);
            al_flip_display();
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            //Menu de pause ativo na tecla esc
            if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                *state = STATE_PAUSE;
                fase_state = CANCELLED;
                running = false;
                break;
            }
            else
                JoystickPress(fase->Player->Control, ev.keyboard.keycode);
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
            JoystickRelease(fase->Player->Control, ev.keyboard.keycode);

        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            *state = STATE_EXIT;
            fase_state = CANCELLED;
            break;
        }
    }

    if(fase)
    {
        printf("deleting fase in fase");
        DeleteFase(fase, timer, bg, music, font, player_font);
    }
    
    return fase_state;
}

void DeleteFase(struct Fase *fase, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP *bg , ALLEGRO_AUDIO_STREAM *music, ALLEGRO_FONT* font, ALLEGRO_FONT* player_font)
{
    printf("fase delete %s\n", fase->Name);

    al_destroy_timer(timer);
    al_destroy_font(font);
    al_destroy_font(player_font);
    al_destroy_bitmap(bg);
    al_detach_audio_stream(music);
    al_destroy_audio_stream(music);
}

void DestroyFase(struct Fase *fase)
{
    //Destroi vetor de vilhoes
    for(int i = 0; i < fase->QtdVilhoes; i++)
    {
        if(fase->Villian[i])
        {
            DestroyVillian(fase->Villian[i]);
            fase->Villian[i] = NULL;
        }
    }
    free(fase->Villian);
    DestroyPlayer(fase->Player);
    free(fase);
}



