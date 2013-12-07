#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include "CTweets.h"
#include "threads_intercommunication.h"

#define TEST

#define FOREVER true
#define SLEEP 0.2

#define WIDTH 800
#define HEIGHT 600
#define FPS 60.0

#define BLACK 0,0,0
#define WHITE 255,255,255
#define RED 255,0,0
#define GREEN 0,255,0
#define BLUE 0,0,255
#define YELLOW 255,255,0
#define CYAN 0,255,255
#define VIOLET 255,0,255
#define UBUNTU_TERMINAL 48,10,36
#define COLOR(x) al_map_rgb(x)
#define RGB(r,g,b) al_map_rgb(r,g,b)

////////////////////PROTOTIPOS////////////////////
void start_threads(ALLEGRO_EVENT_QUEUE*event_queue,ALLEGRO_DISPLAY*display);
void *search(void*);
void *main_loop(void*event_queue);
void init_CTweets(void);
ALLEGRO_DISPLAY * init_allegro(void);
void shutdown_allegro(ALLEGRO_DISPLAY*display);
ALLEGRO_EVENT_QUEUE *create_event_queue(ALLEGRO_DISPLAY*display);

////////////////////MAIN////////////////////
int main(void)
{
    init_CTweets();
    ALLEGRO_DISPLAY*display=init_allegro();
    ALLEGRO_EVENT_QUEUE*event_queue=create_event_queue(display);
    start_threads(event_queue,display);
    shutdown_allegro(display);
    return 0;
}

////////////////////THREADS////////////////////
void start_threads(ALLEGRO_EVENT_QUEUE*event_queue,ALLEGRO_DISPLAY*display)
{
    pthread_t search_thread,main_loop_thread;
    if(pthread_create(&search_thread,NULL,&search,NULL))
    {
        shutdown_allegro(display);
        fprintf(stderr,"pthread_create failed\n");
        
        exit(9);
    }
    if(pthread_create(&main_loop_thread,NULL,&main_loop,(void*)&event_queue))
    {
        shutdown_allegro(display);
        fprintf(stderr,"pthread_create failed\n");
        exit(9);
    }
    pthread_join(main_loop_thread,NULL);
}

void *search(void*)
{
    char*err;
    Uint count;
    CTweet*search1_buffer,*search2_buffer,*search3_buffer;
    while(!get_exit_state())
    {
        if(!get_search_ready(1)&&get_search_text(1)[0])
        {
            count=BUFFER_SIZE;
            search1_buffer=CT_search(get_search_text(1),&count,err);
            if((err==NULL)&&(search1_buffer!=NULL)&&count)
            {
                write_search_array(1,search1_buffer,count);
                write_search_ready(1,count);
            }
        }
        if(!get_search_ready(2)&&get_search_text(2)[0])
        {
            count=BUFFER_SIZE;
            search2_buffer=CT_search(get_search_text(2),&count,err);
            if((err==NULL)&&(search2_buffer!=NULL)&&count)
            {
                write_search_array(2,search2_buffer,count);
                write_search_ready(2,count);
            }
        }
        if(!get_search_ready(3)&&get_search_text(3)[0])
        {
            count=BUFFER_SIZE;
            search3_buffer=CT_search(get_search_text(3),&count,err);
            if((err==NULL)&&(search3_buffer!=NULL)&&count)
            {
                write_search_array(3,search3_buffer,count);
                write_search_ready(3,count);
            }
        }
        sleep(SLEEP);
    }
}

void *main_loop(void*event_queue)
{
    #ifdef TEST
    new_search_text(1,(char*)"love");
    new_search_text(2,(char*)"xmas");
    new_search_text(3,(char*)"happy");
    #endif
    ALLEGRO_EVENT_QUEUE* queue=(ALLEGRO_EVENT_QUEUE*)event_queue;
    Uint i;
    CTweet* search_array;
    while(FOREVER)
    {
        if(get_search_ready(1))
        {
            search_array=get_search_array(1);
            for(i=0;i<get_search_ready(1);i++)
            {
                #ifndef TEST
                if(search_array[i].loc.latitude<9000)
                #endif
                {
                    #ifdef TEST
                    printf("user: %s\n"     , search_array[i].user);
                    printf("userName: %s\n" , search_array[i].userName);
                    printf("text: %s\n"     , search_array[i].text);
                    printf("lang: %s\n"     , search_array[i].lang);
                    printf("lat:  %f\n"     , search_array[i].loc.latitude );
                    printf("long: %f\n\n"   , search_array[i].loc.longitude);
                    fflush(stdout);
                    sleep(SLEEP);
                    #endif
                }
            }
            write_search_ready(1,false);
        }
        if(get_search_ready(2))
        {
            search_array=get_search_array(2);
            for(i=0;i<get_search_ready(2);i++)
            {
                #ifndef TEST
                if(search_array[i].loc.latitude<9000)
                #endif
                {
                    #ifdef TEST
                    printf("user: %s\n"     , search_array[i].user);
                    printf("userName: %s\n" , search_array[i].userName);
                    printf("text: %s\n"     , search_array[i].text);
                    printf("lang: %s\n"     , search_array[i].lang);
                    printf("lat:  %f\n"     , search_array[i].loc.latitude );
                    printf("long: %f\n\n"   , search_array[i].loc.longitude);
                    fflush(stdout);
                    sleep(SLEEP);
                    #endif
                }
            }
            write_search_ready(2,false);
        }
        if(get_search_ready(3))
        {
            search_array=get_search_array(3);
            for(i=0;i<get_search_ready(3);i++)
            {
                #ifndef TEST
                if(search_array[i].loc.latitude<9000)
                #endif
                {
                    #ifdef TEST
                    printf("user: %s\n"     , search_array[i].user);
                    printf("userName: %s\n" , search_array[i].userName);
                    printf("text: %s\n"     , search_array[i].text);
                    printf("lang: %s\n"     , search_array[i].lang);
                    printf("lat:  %f\n"     , search_array[i].loc.latitude );
                    printf("long: %f\n\n"   , search_array[i].loc.longitude);
                    fflush(stdout);
                    sleep(SLEEP);
                    #endif
                }
            }
            write_search_ready(3,false);
        }
    }
    exit_threads();
}

////////////////////CTweets////////////////////
void init_CTweets(void)
{
    char*err;
    if(CT_init(err)==NOT_OK)
    {
        fprintf(stderr,"%s\n",err);
        exit(8);
    }
}

////////////////////ALLEGRO////////////////////
ALLEGRO_DISPLAY * init_allegro(void)
{
    if(!al_init())
    {
        fprintf(stderr,"Could not init Allegro.\n");
        exit(1);
    }
    if(!al_init_primitives_addon())
    {
        fprintf(stderr,"al_init_primitives_addon failed.\n");
        exit(2);
    }
    if(!al_install_mouse())
    {
        fprintf(stderr,"al_install_mouse failed\n");
        
        al_shutdown_primitives_addon();
        exit(3);
    }
    if(!al_install_keyboard())
    {
        fprintf(stderr,"al_install_keyboard failed\n");
        al_uninstall_mouse();
        al_shutdown_primitives_addon();
        exit(4);
    }
    ALLEGRO_DISPLAY *display=al_create_display(WIDTH,HEIGHT);
    if(!display)
    {
        fprintf(stderr,"al_create_display failed\n");
        al_uninstall_keyboard();
        al_uninstall_mouse();
        al_shutdown_primitives_addon();
        exit(5);
    }
    al_set_window_position(display,0,0);
    al_init_font_addon();
    if(!al_init_ttf_addon())
    {
        fprintf(stderr,"al_init_ttf_addon failed\n");
        al_uninstall_keyboard();
        al_uninstall_mouse();
        al_shutdown_primitives_addon();
        al_destroy_display(display);
        al_shutdown_font_addon();
        exit(6);
    }
    return display;
}

void shutdown_allegro(ALLEGRO_DISPLAY*display)
{
    al_uninstall_keyboard();
    al_uninstall_mouse();
    al_destroy_display(display);
    al_shutdown_primitives_addon();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
}

ALLEGRO_EVENT_QUEUE *create_event_queue(ALLEGRO_DISPLAY*display)
{
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr,"al_create_event_queue failed\n");
        shutdown_allegro(display);
        exit(7);
    }
    al_register_event_source(event_queue,al_get_keyboard_event_source());
    al_register_event_source(event_queue,al_get_mouse_event_source());
    al_register_event_source(event_queue,al_get_display_event_source(display));
    return event_queue;
}