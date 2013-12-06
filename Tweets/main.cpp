#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include "CTweets.h"

typedef char BOOL;
typedef unsigned int Uint;

#define FOREVER true
#define BUFFER_SIZE 1

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

////////////////////VARIABLES COMPARTIDAS ENTRE THREADS////////////////////
BOOL Exit=false;      //HABRIA QUE AGREGAR ESTAS VARIABLES EN UN ARCHIVO APARTE
char *search1_text=(char*)"xmas";
char *search2_text=(char*)"happy";
char *search3_text=(char*)"love";
Uint search1_ready=false;
Uint search2_ready=false;
Uint search3_ready=false;
CTweet *search1_buffer=NULL;
CTweet *search2_buffer=NULL;
CTweet *search3_buffer=NULL;

////////////////////PROTOTIPOS////////////////////
void start_threads(ALLEGRO_EVENT_QUEUE*event_queue,ALLEGRO_DISPLAY*display);
void *search1(void*);
void *search2(void*);
void *search3(void*);
void *main_loop(void*event_queue);
void init_CTweets(void);
ALLEGRO_DISPLAY * init_allegro(void);
void shutdown_allegro(ALLEGRO_DISPLAY*display);
ALLEGRO_EVENT_QUEUE *create_event_queue(ALLEGRO_DISPLAY*display);

////////////////////MAIN////////////////////
int main(void)
{
    init_CTweets();
    CT_searchReset();
    ALLEGRO_DISPLAY*display=init_allegro();
    ALLEGRO_EVENT_QUEUE*event_queue=create_event_queue(display);
    start_threads(event_queue,display);
    shutdown_allegro(display);
    return 0;
}

////////////////////THREADS////////////////////
void start_threads(ALLEGRO_EVENT_QUEUE*event_queue,ALLEGRO_DISPLAY*display)
{
    pthread_t search1_thread,search2_thread,search3_thread,main_loop_thread;
    if(pthread_create(&search1_thread,NULL,&search1,NULL))
    {
        shutdown_allegro(display);
        fprintf(stderr,"pthread_create failed\n");
        exit(9);
    }
    if(pthread_create(&search2_thread,NULL,&search2,NULL))
    {
        shutdown_allegro(display);
        fprintf(stderr,"pthread_create failed\n");
        exit(9);
    }
    if(pthread_create(&search3_thread,NULL,&search3,NULL))
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

void *search1(void*)    //SOLO BUSCAN Y COLOCAN EN EL ARREGLO LOS RESULTADOS Y EN EL READY EL NUMERO DE RESULTADOS ENCONTRADOS
{
    char*err=NULL;
    Uint count;
    while(!Exit)
    {
        if(!search1_ready&&search1_text!=NULL)
        {
            count=BUFFER_SIZE;
            search1_buffer=CT_search(search1_text,&count,err);
            if((err==NULL)&&(search1_buffer!=NULL))
                search1_ready=count;
        }
        if(count>2) sleep(1.0/count);
        else sleep(0.5);
    }
}

void *search2(void*)
{
    char*err;
    Uint count;
    while(!Exit)
    {
        if(!search2_ready&&search2_text!=NULL)
        {
            count=BUFFER_SIZE;
            search2_buffer=CT_search(search2_text,&count,err);
            if((err==NULL)&&(search2_buffer!=NULL))
                search2_ready=count;
        }
        if(count>2) sleep(1.0/count);
        else sleep(0.5);
    }
}

void *search3(void*)
{
    char*err;
    Uint count;
    while(!Exit)
    {
        if(!search3_ready&&search3_text!=NULL)
        {
            count=BUFFER_SIZE;
            search3_buffer=CT_search(search3_text,&count,err);
            if((err==NULL)&&(search3_buffer!=NULL))
                search3_ready=count;
        }
        if(count>2) sleep(1.0/count);
        else sleep(0.5);
    }
}

//AL REALIZAR MAS DE UNA BUSQUEDA SIMULTANEA EVENTUALMENTE COLAPSA
void *main_loop(void*event_queue)      //THREAD QUE CONTIENE LAS FUNCIONES DE DIBUJO Y MANEJO DE LOS OTROS THREADS
{
    ALLEGRO_EVENT_QUEUE* queue=(ALLEGRO_EVENT_QUEUE*)event_queue;
    Uint i;
    while(FOREVER)
    {
        if(search1_ready)
        {
            for(i=0;i<search1_ready;i++)
            {
                //if(search1_buffer[i].loc.latitude||search1_buffer[i].loc.longitude)
                {
                    printf("user: %s\n"     , search1_buffer[i].user);
                    printf("userName: %s\n" , search1_buffer[i].userName);
                    printf("text: %s\n"     , search1_buffer[i].text);
                    printf("lang: %s\n"     , search1_buffer[i].lang);
                    printf("lat:  %f\n"     , search1_buffer[i].loc.latitude );
                    printf("long: %f\n\n"   , search1_buffer[i].loc.longitude);
                    fflush(stdout);
                    sleep(0.2);
                }
            }
            search1_ready=false;
        }
        if(search2_ready)
        {
            for(i=0;i<search2_ready;i++)
            {
                //if(search2_buffer[i].loc.latitude||search2_buffer[i].loc.longitude)
                {
                    printf("user: %s\n"     , search2_buffer[i].user);
                    printf("userName: %s\n" , search2_buffer[i].userName);
                    printf("text: %s\n"     , search2_buffer[i].text);
                    printf("lang: %s\n"     , search2_buffer[i].lang);
                    printf("lat:  %f\n"     , search2_buffer[i].loc.latitude );
                    printf("long: %f\n\n"   , search2_buffer[i].loc.longitude);
                    fflush(stdout);
                    sleep(0.2);
                }
            }
            search2_ready=false;
        }
        if(search3_ready)
        {
            for(i=0;i<search3_ready;i++)
            {
                //if(search3_buffer[i].loc.latitude||search3_buffer[i].loc.longitude)
                {
                    printf("user: %s\n"     , search3_buffer[i].user);
                    printf("userName: %s\n" , search3_buffer[i].userName);
                    printf("text: %s\n"     , search3_buffer[i].text);
                    printf("lang: %s\n"     , search3_buffer[i].lang);
                    printf("lat:  %f\n"     , search3_buffer[i].loc.latitude );
                    printf("long: %f\n\n"   , search3_buffer[i].loc.longitude);
                    fflush(stdout);
                    sleep(0.2);
                }
            }
            search3_ready=false;
        }
        sleep(0.5);
    }
    Exit=true;
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