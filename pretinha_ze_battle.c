#include <stdio.h>
#include "config/config.h"
#include "entities/level/level.h"
#include "services/data/data.h"
#include "entities/utilities/difficult.h"

void DisplayMenu(GameState *state, ALLEGRO_EVENT_QUEUE *event_queue)
{
    ALLEGRO_BITMAP *background = al_load_bitmap(MENU_BACKGROUND);
	ALLEGRO_FONT* font = al_load_font(MENU_FONT, 80, 0);
	ALLEGRO_FONT* sub_font = al_load_font(MENU_FONT, 40, 0);
    ALLEGRO_FONT* emoji = al_load_font(EMOJI_FONT, 40, 0);

    ALLEGRO_AUDIO_STREAM *music = al_load_audio_stream(MENU_SONG, 4, 2048);
    al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
    al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);

    bool running = true;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    while(running)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch (ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_ENTER:
                    *state = 1;
                    running = false;
                break;
            }
        }

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            *state = STATE_EXIT;
            running = false;
        }

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            al_draw_bitmap(background, 0,0,0);
		    al_draw_text(font, al_map_rgb(255,255,255), 220, 100, 0, "Ze e pretinha's");

            if(((int)(al_get_time() * 2)) % 2 == 0)
            {
                al_draw_text(emoji, al_map_rgb(0,0,0), 400 - 80, 500, 0, "MN");
                al_draw_text(sub_font, al_map_rgb(0,0,0), 400, 500, 0, "PRESS START");
                al_draw_text(emoji, al_map_rgb(0,0,0), 400 + 250, 500, 0, "MN");
            }
            
            al_flip_display();
        }
    }

    al_destroy_timer(timer);
    al_destroy_font(font);
    al_destroy_font(emoji);
    al_destroy_font(sub_font);
    al_destroy_bitmap(background);
    al_detach_audio_stream(music);
    al_destroy_audio_stream(music);
}

void DisplayLostMenu(GameState *state, ALLEGRO_EVENT_QUEUE *event_queue)
{
    ALLEGRO_BITMAP *background = al_load_bitmap(MENU_BACKGROUND);
	ALLEGRO_FONT* font = al_load_font(MENU_FONT, 70, 0);
	ALLEGRO_FONT* sub_font = al_load_font(MENU_FONT, 40, 0);

    ALLEGRO_AUDIO_STREAM *music = al_load_audio_stream(LOSE_SONG, 4, 2048);
    al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
    al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);

    bool running = true;
    int selected = 0; 
    const char *options[] = { "Tentar Novamente", "Menu Principal" };
    const int option_count = 2;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    while(running)
    {
        al_draw_bitmap(background, 0,0,0);
		al_draw_text(font, al_map_rgb(255,255,255), X_SCREEN/2 - 200, 100, 0, "GAME OVER ;(");
        for (int i = 0; i < option_count; i++)
        {
            ALLEGRO_COLOR color = (i == selected) ? al_map_rgb(255, 255, 0) : al_map_rgb(0, 0, 0);
            al_draw_text(sub_font, color, X_SCREEN / 2 - 150, 300 + i * 80, 0, options[i]);
        }

        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
       if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_UP:
                    selected = (selected + option_count - 1) % option_count;
                    break;
                case ALLEGRO_KEY_DOWN:
                    selected = (selected + 1) % option_count;
                    break;
                case ALLEGRO_KEY_ENTER:
                case ALLEGRO_KEY_SPACE:
                    *state = (selected == 0) ? STATE_STARTGAME : STATE_MENU;
                    running = false;
                    break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            *state = STATE_EXIT;
            running = false;
        }
    }

    al_destroy_timer(timer);
    al_destroy_font(font);
    al_destroy_font(sub_font);
    al_destroy_bitmap(background);
    al_detach_audio_stream(music);
    al_destroy_audio_stream(music);
}

void DisplayWinnFase(GameState *state, struct Fase *level, ALLEGRO_EVENT_QUEUE *event_queue)
{
    ALLEGRO_BITMAP *background = al_load_bitmap(WINN_BACKGROUND);
    ALLEGRO_BITMAP *winn = al_load_bitmap(WINN_ELEMENT);
	ALLEGRO_FONT* font = al_load_font(MENU_FONT, 80, 0);
	ALLEGRO_FONT* sub_font = al_load_font(MENU_FONT, 40, 0);
    ALLEGRO_FONT* emoji = al_load_font(EMOJI_FONT, 40, 0);

    ALLEGRO_AUDIO_STREAM *music = al_load_audio_stream(WINN_SONG, 4, 2048);
    al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
    al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);

    bool running = true;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    while(running)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch (ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_ENTER:
                    *state = STATE_STARTGAME;
                    running = false;
                break;
            }
        }

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            al_draw_bitmap(background, 0,0,0);
		    al_draw_text(font, al_map_rgb(255,255,255), 180 - 40, 100, 0, "YOU WON THIS FASE!");
            al_draw_bitmap(winn, X_SCREEN/2 - 30, 290, 0);

            if(((int)(al_get_time() * 2)) % 2 == 0)
            {
                al_draw_text(emoji, al_map_rgb(255,255,255), 230 - 80, 600, 0, "MN");
                al_draw_text(sub_font, al_map_rgb(255,255,255), 230, 600, 0, "PRESS START TO CONTINUE");
                al_draw_text(emoji, al_map_rgb(255,255,255), 230 + 500, 600, 0, "MN");
            }
            
            al_flip_display();
        }
    }

    al_destroy_timer(timer);
    al_destroy_font(font);
    al_destroy_font(emoji);
    al_destroy_font(sub_font);
    al_destroy_bitmap(background);
    al_destroy_bitmap(winn);
    al_detach_audio_stream(music);
    al_destroy_audio_stream(music);
}

void DisplayWinnGame(GameState *state, ALLEGRO_EVENT_QUEUE *event_queue)
{
    ALLEGRO_BITMAP *background = al_load_bitmap(MENU_BACKGROUND);
    ALLEGRO_BITMAP *winn = al_load_bitmap(WINN_ELEMENT);
    ALLEGRO_BITMAP *pretinha = al_load_bitmap("assets/players/little0.png");
	ALLEGRO_FONT* font = al_load_font(MENU_FONT, 80, 0);
	ALLEGRO_FONT* sub_font = al_load_font(MENU_FONT, 40, 0);
    ALLEGRO_FONT* sub_sub_font = al_load_font(MENU_FONT, 20, 0);
    ALLEGRO_FONT* emoji = al_load_font(EMOJI_FONT, 20, 0);

    ALLEGRO_AUDIO_STREAM *music = al_load_audio_stream(WINN_SONG, 4, 2048);
    al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
    al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);

    bool running = true;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    while(running)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch (ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_ENTER:
                    *state = STATE_MENU;
                    running = false;
                break;
            }
        }

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(background, 0,0,0);
		    al_draw_text(font, al_map_rgb(255,255,255), 180 - 60, 100, 0, "YOU WON THE GAME!!!");
            al_draw_bitmap(winn, X_SCREEN/2 - 30, 290, 0);
            al_draw_text(sub_sub_font, al_map_rgb(0,0,0), 180 - 80, 600, 0, "Obrigado por jogar :)");
            al_draw_bitmap(pretinha, 180 + 180, 600, 0);

            if(((int)(al_get_time() * 2)) % 2 == 0)
            {
                al_draw_text(emoji, al_map_rgb(0,0,0), 180 - 80, 500, 0, "MN");
                al_draw_text(sub_font, al_map_rgb(0,0,0), 180, 500, 0, "PRESS START TO RETURN TO MENU");
                al_draw_text(emoji, al_map_rgb(0,0,0), 180 + 450, 500, 0, "MN");
            }
            
            al_flip_display();
        }
    }

    al_destroy_bitmap(winn);
    al_destroy_bitmap(pretinha);
    al_destroy_timer(timer);
    al_destroy_font(font);
    al_destroy_font(emoji);
    al_destroy_font(sub_font);
    al_destroy_font(sub_sub_font);
    al_destroy_bitmap(background);
    al_detach_audio_stream(music);
    al_destroy_audio_stream(music);
}

void PauseGame(GameState *state, struct Fase *level, ALLEGRO_EVENT_QUEUE *event_queue)
{
    ALLEGRO_BITMAP *background = al_load_bitmap(MENU_BACKGROUND);
	ALLEGRO_FONT* font = al_load_font(MENU_FONT, 70, 0);
	ALLEGRO_FONT* sub_font = al_load_font(MENU_FONT, 40, 0);

    bool pause = true;
    int selected = 0; 
    const char *options[] = { "Continuar jogo", "Menu Principal" };
    const int option_count = 2;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    while(pause)
    {
        al_draw_bitmap(background, 0,0,0);
		al_draw_text(font, al_map_rgb(255,255,255), X_SCREEN/2 - 200, 100, 0, "MENU PAUSE");
        for (int i = 0; i < option_count; i++)
        {
            ALLEGRO_COLOR color = (i == selected) ? al_map_rgb(255, 255, 0) : al_map_rgb(0, 0, 0);
            al_draw_text(sub_font, color, X_SCREEN / 2 - 150, 300 + i * 80, 0, options[i]);
        }

        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
       if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_UP:
                    selected = (selected + option_count - 1) % option_count;
                    break;
                case ALLEGRO_KEY_DOWN:
                    selected = (selected + 1) % option_count;
                    break;
                case ALLEGRO_KEY_ENTER:
                case ALLEGRO_KEY_SPACE:
                    *state = (selected == 0) ? STATE_STARTGAME : STATE_MENU;
                    pause = false;
                    break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            *state = STATE_EXIT;
            pause = false;
        }
    }

    al_destroy_timer(timer);
    al_destroy_font(font);
    al_destroy_font(sub_font);
    al_destroy_bitmap(background);
}

void InitGame(Database *local_db, GameState *state, ALLEGRO_EVENT_QUEUE *event_queue)
{ 
    int current_level = local_db->lavel;
    struct Fase *fase = NULL;

    bool running = true;
    bool createFase = false;
    while(running)
    {
        //Cria no inicio e mantém a mesma no pause
        if(!createFase)
        {
            fase = CreateFase(EASY, current_level, PLAYER_PRETINHA, "PRETINHA");
            if(!fase)
            {
                printf("[ERROR]: FAILURE IN LEVEL CREATION");
                *state = STATE_EXIT;
                return;
            }

            createFase = true;
        }

        printf("fase %s", fase->Name);
        FaseState result = PlayFase(fase, state, event_queue);
        printf("%d", (int)result);

        if(*state == STATE_PAUSE)
        {
            PauseGame(state, fase, event_queue);
            if(*state == STATE_MENU || *state == STATE_EXIT)
            {
                running = false;
                break;
            }
            continue;
        }

        switch (result)
        {
            case COMPLEATED:
                printf("WINNING FASE");
                DisplayWinnFase(state, fase, event_queue);

                if(*state == STATE_STARTGAME)
                {
                    DestroyFase(fase);
                    fase = NULL;    
                    createFase = false;
                }
                
                if(current_level == FASE_ENDGAME)
                {
                    DisplayWinnGame(state, event_queue);
                    if(*state == STATE_MENU)
                    {
                        running = false;
                        break;
                    }
                }

                current_level++;
                break;
        
            case LOST:
                printf("init lose screen");
                DisplayLostMenu(state, event_queue);
                if(*state == STATE_STARTGAME)
                {
                    if(fase)
                    {
                        DestroyFase(fase);
                        fase = NULL;
                        createFase = false;
                    }
                    continue;
                }
                else
                {
                    running = false;
                    break;
                }

            case CANCELLED:
                *state = STATE_EXIT;
                running = false;
                break;
        }
    }

    if(fase)
        DestroyFase(fase);
}

int main()
{
    srand(time(0));
    
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(1);
    al_install_keyboard();

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(X_SCREEN, Y_SCREEN);
        	
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_keyboard_event_source());

    GameState gameState = STATE_MENU;
    Database *local_db = ExistDataBase() ? GetDataBase() : CreateDataBase();
    if(!local_db)
        return EXIT_FAILURE;

    while(gameState != STATE_EXIT)
    {
        switch (gameState)
        {
            case STATE_MENU:
                DisplayMenu(&gameState, queue);
            break;

            case STATE_STARTGAME:
                InitGame(local_db, &gameState, queue);
            break;
        }
    }

    free(local_db);
    al_destroy_event_queue(queue);
    al_destroy_display(disp);

    return 0;
}





