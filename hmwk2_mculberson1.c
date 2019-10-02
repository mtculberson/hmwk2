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

#define xOffset 1
#define yOffset 2
#define KEY_SEEN     1
#define KEY_RELEASED 2

struct Point
{
    int x, y, tile_number;
};
                        
void must_init(bool test, const char *description)
{
    if(test) return;
    
    printf("couldn't initialize %s\n", description);
    exit(1);
}

/*int Get_Correction(int xCoord, int barrierX) {
    barrierX = barrierX * 32;
    
    if((xCoord < barrierX + 32 && xCoord > barrierX)
   || (xCoord + 32 < barrierX + 32 && xCoord + 32 > barrierX))
    {
       if(xCoord < barrierX + 32 && xCoord > barrierX)
       {
           return (barrierX + 32) - xCoord;
       }
       else
       {
           return barrierX - (xCoord + 32);
       }
    }
    return xCoord;
}*/

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
    
    
    int window_tile_map[20][15];
    for (int x = 0; x < 20; x++){
        for(int y = 0; y < 15 ;y++){
            window_tile_map[x][y] = 0;
        }
    }
    
    
    int arraySize = 59;
    struct Point points[arraySize];
    int counter = 0;
    
    for(int i = 0; i < 15; i++) {
        points[i].x = i + xOffset;
        points[i].y = yOffset;
        points[i].tile_number = 0;
        counter++;
    }
    
    for(int i = 0; i < 14 + xOffset; i++) {
        points[counter].x = i + xOffset;;
        points[counter].y = 10 + yOffset;
        points[counter].tile_number = 1;
        counter++;
    }
    
    for(int i = 0; i < 10 + xOffset; i++) {
        points[counter].x = xOffset;
        points[counter].y = i + yOffset;
        points[counter].tile_number = 2;
        counter++;
    }
    
    for(int i = 0; i < 2 + yOffset; i++) {
        points[counter].x = 14 + xOffset;
        points[counter].y = i + yOffset;
        points[counter].tile_number = 3;
        counter++;
    }
    
    for(int i = 0; i < 4 + yOffset; i++) {
        points[counter].x = 14 + xOffset;
        points[counter].y = i + yOffset + 5;
        points[counter].tile_number = 3;
        counter++;
    }
    
    points[counter].x = 5 + xOffset;
    points[counter].y = 6 + yOffset;
    points[counter].tile_number = 4;
    counter++;
    
    points[counter].x = 12 + xOffset;
    points[counter].y = 2 + yOffset;
    points[counter].tile_number = 4;
    counter++;
    
    points[counter].x = 12 + xOffset;
    points[counter].y = 7 + yOffset;
    points[counter].tile_number = 5;
    counter++;
    
    points[counter].x = 7 + xOffset;
    points[counter].y = (rand() % 8) + yOffset;
    points[counter].tile_number = 6;
    counter++;
    
    points[counter].x = 13 + xOffset;
    points[counter].y = 3 + yOffset;
    points[counter].tile_number = 7;
    counter++;
    
    points[counter].x = 11 + xOffset;
    points[counter].y = 4 + yOffset;
    points[counter].tile_number = 7;
    counter++;
    
    points[counter].x = 5 + xOffset;
    points[counter].y = 4 + yOffset;
    points[counter].tile_number = 8;
    counter++;
    
    points[counter].x = 15 + xOffset;
    points[counter].y = yOffset + 4;
    points[counter].tile_number = 9;
    counter++;
    
    //Modifications end
    
    al_grab_mouse(disp);
    al_hide_mouse_cursor(disp);
    
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
            
            
            al_set_target_backbuffer(al_get_current_display());
            al_clear_to_color(al_map_rgb(0, 100, 0));
            al_draw_textf(font, al_map_rgba(255, 255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", x, y);
            al_draw_bitmap(Pounce, x + 10, y + 10, 0);
            
           

            size_t arraySize = sizeof(points)/sizeof(points[0]);
            //printf("%d\n", arraySize);
            for (int i = 0; i < arraySize; i++){
                if(points[i].tile_number == 9) {
                    al_draw_bitmap_region(walls, points[i].tile_number * 32, 0, 32, 32, points[i].x * 32, points[i].y * 32, 0);
                } else {
                    al_draw_tinted_bitmap_region(walls, al_map_rgba(0, 0, 0, 0), points[i].tile_number * 32, 0, 32, 32, points[i].x * 32, points[i].y * 32, 0);
                }
            }

            if((x <= (points[arraySize - 1].x * 32 + 16) && x >= (points[arraySize - 1].x * 32 - 16)) &&
               ((y <= (points[arraySize - 1].y * 32 + 16)) && (y >= (points[arraySize - 1].y * 32 - 16))))
            {
                al_draw_text(font, al_map_rgb(255, 255, 255), 290, 240, 0, "You win!");
                done = true;
            }
            
            for(int i = 0; i < arraySize; i++) {
                if(points[i].tile_number != 9) {
                    if((x <= (points[i].x * 32 + 30) && x >= (points[i].x * 32 - 31)) &&
                       ((y <= (points[i].y * 32 + 30)) && (y >= (points[i].y * 32 - 31))))
                    {
                        //x = Get_Correction(x, points[i].x);
                        //y = Get_Correction(y, points[i].y);
                        
                        al_draw_tinted_bitmap_region(walls, al_map_rgba(255, 255, 0, 255), points[i].tile_number * 32, 0, 32, 32, points[i].x * 32, points[i].y * 32, 0);
                        
                        al_draw_text(font, al_map_rgb(255, 255, 255), 290, 450, 0, "Barrier Hit!");
                    }
                }
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

    al_destroy_bitmap(Pounce);
    al_destroy_bitmap(walls);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    
    return 0;
}
// END code from Doug Thompson
// https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace:-Basic-
// game-structure
