/*
 hmwk2_mculberson1.c
 
 Matthew Culberson
 Account: mculberson1
 CSc 4821
 Homework 2
 Due date: October 3, 2019
 
 Description:
 
 Input:
 (State what the program inputs are.)
 Output:
 (State what the program outputs are.)
 */

// BEGIN code from Doug Thompson
// https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace:-Basic-
// game-structure

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

                        
void must_init(bool test, const char *description)
{
    if(test) return;
    
    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    
    must_init(al_install_mouse(), "mouse");
    
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");
    
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");
    
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    
    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    must_init(disp, "display");
    
    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");
    
    //Modified by Matthew Culberson
    //This loads in the Pounce graphic
    //and the graphic walls
    
    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* Pounce = al_load_bitmap("Pounce.png");
    must_init(Pounce, "Pounce");
    
    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* walls = al_load_bitmap("walls.png");
    must_init(walls, "walls");
    
    //End modification
    
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool done = false;    
    bool redraw = true;
    ALLEGRO_EVENT event;
    
    float x, y;
    x = 100;
    y = 100;
    
    float dx, dy;
    dx = 0;
    dy = 0;
    
    //Modifications made by Matthew
    //method added to create tile generation
    
    int wallsTileXPos = 0;
    int windowTileXPos[10];
    int windowTileYPos[10];
    
    int window_tile_map[20][15];
    for (int x = 0; x < 20; x++){
        for(int y = 0; y < 15 ;y++){
            window_tile_map[x][y] = 0;
        }
    }
    
    al_set_target_bitmap(walls);
    al_draw_bitmap(walls, 0,0,0);

    al_set_target_backbuffer(disp);
    
    for(int i = 0; i < 10 ;i++){
        int randXTile = rand() % 20;
        int randYTile = rand() % 15;
        
        while(window_tile_map[randXTile][randYTile] == 1) {
            randXTile = rand() % 20;
            randYTile = rand() % 15;
        }
        
        windowTileXPos[i] = randXTile*32;
        windowTileYPos[i] = randYTile*32;
        
        window_tile_map[randXTile][randYTile] = 1;
        
        wallsTileXPos = wallsTileXPos + 32;
    }
    
    //Modifications end
    
    al_grab_mouse(disp);
    al_hide_mouse_cursor(disp);
    
    #define KEY_SEEN     1
    #define KEY_RELEASED 2
    
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);
        
        switch(event.type)
        {
                
    //Modification made by Matthew Culberson
    //Changes made were to alter the amount of pixels to 4 with each press
                
            case ALLEGRO_EVENT_TIMER:
                if(key[ALLEGRO_KEY_UP])
                    y = y - 4;
                if(key[ALLEGRO_KEY_DOWN])
                    y = y + 4;
                if(key[ALLEGRO_KEY_LEFT])
                    x = x - 4;
                if(key[ALLEGRO_KEY_RIGHT])
                    x = x + 4;
                
    //End modification
                
                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;
                
                x += dx;
                y += dy;
                
                if(x < 0)
                {
                    x *= -1;
                    dx *= -1;
                }
                if(x > 605)
                {
                    x -= (x - 605) * 2;
                    dx *= -1;
                }
                if(y < 0)
                {
                    y *= -1;
                    dy *= -1;
                }
                if(y > 440)
                {
                    y -= (y - 440) * 2;
                    dy *= -1;
                }
                
                dx *= 0.9;
                dy *= 0.9;
                
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;
                
                redraw = true;
                break;
                
            case ALLEGRO_EVENT_MOUSE_AXES:
                dx += event.mouse.dx * 0.1;
                dy += event.mouse.dy * 0.1;
                al_set_mouse_xy(disp, 320, 240);
                break;
                
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;
                
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }
        
        if(done)
            break;
        
        if(redraw && al_is_event_queue_empty(queue))
        {
            
            int wallsTileXPos = 0;
            3(disp);
            al_clear_to_color(al_map_rgb(0, 100, 0));
            al_draw_textf(font, al_map_rgba(255, 255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", x, y);
            al_draw_bitmap(Pounce, x + 10, y + 10, 0);
            
            al_set_target_bitmap(walls);
            int alpha = 255;
            for (int i = 0; i < 10; i++){
                if(i == 9) {
                    alpha = 0;
                }
                
                al_clear_to_color(al_map_rgba(0,0,0, alpha));
                
                al_draw_bitmap_region(walls, wallsTileXPos, 0, 32, 32, windowTileXPos[i], windowTileYPos[i], 0);
                wallsTileXPos = wallsTileXPos + 32;
            }
            
            if((x <= (windowTileXPos[9] + 16) && x >= (windowTileXPos[9] - 16)) && ((y <= (windowTileYPos[9] + 16)) && (y >= (windowTileYPos[9] - 16))))
            {
                al_draw_text(font, al_map_rgb(255, 255, 255), 290, 240, 0, "You win!");
                done = true;
            }
        
            al_flip_display();
            redraw = false;
            
            if(done)
            {
                sleep(4);
                break;
            }
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_bitmap(Pounce);
    al_destroy_bitmap(walls);
    
    return 0;
}

// END code from Doug Thompson
// https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace:-Basic-
// game-structure

