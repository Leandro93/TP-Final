#include <stdio.h>
#include <pthread.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

typedef char BOOL;

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

BOOL _exit_=false;

////////////////////PROTOTIPOS////////////////////
void *search1(void*);
void *search2(void*);
void *search3(void*);
void *main_loop(void* event_queue);
ALLEGRO_DISPLAY * init_allegro(void);
void shutdown_allegro(ALLEGRO_DISPLAY *display);
ALLEGRO_EVENT_QUEUE *create_event_queue(ALLEGRO_DISPLAY *display);

int main(void)
{
    ALLEGRO_DISPLAY *display=init_allegro();
    ALLEGRO_EVENT_QUEUE *event_queue=create_event_queue(display);
    pthread_t search1_thread,search2_thread,search3_thread,main_loop_thread;
    pthread_create(&search1_thread,NULL,&search1,NULL);
    pthread_create(&search2_thread,NULL,&search2,NULL);
    pthread_create(&search3_thread,NULL,&search3,NULL);
    pthread_create(&main_loop_thread,NULL,&main_loop,(void*)&event_queue);
    pthread_join(search1_thread,NULL);
    pthread_join(search2_thread,NULL);
    pthread_join(search3_thread,NULL);
    pthread_join(main_loop_thread,NULL);
    return 0;
}

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
    al_install_mouse();
    if(!al_install_keyboard())
    {
        fprintf(stderr,"al_install_keyboard failed\n");
        exit(3);
        al_shutdown_primitives_addon();
    }
    ALLEGRO_DISPLAY *display=al_create_display(WIDTH,HEIGHT);
    if(!display)
    {
        fprintf(stderr,"al_create_display failed\n");
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        exit(4);
    }
    al_set_window_position(display,0,0);
    al_init_font_addon();
    al_init_ttf_addon();
    return display;
}

void shutdown_allegro(ALLEGRO_DISPLAY *display)
{
    al_destroy_display(display);
    al_uninstall_keyboard();
    al_shutdown_primitives_addon();
    al_shutdown_ttf_addon();
    al_uninstall_mouse();
}

ALLEGRO_EVENT_QUEUE *create_event_queue(ALLEGRO_DISPLAY *display)
{
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr,"al_create_event_queue failed\n");
        shutdown_allegro(display);
        exit(5);
    }
    al_register_event_source(event_queue,al_get_keyboard_event_source());
    al_register_event_source(event_queue,al_get_mouse_event_source());
    al_register_event_source(event_queue,al_get_display_event_source(display));
    return event_queue;
}

////////////////////THREADS////////////////////
void *search1(void*)
{
    while(_exit_==false)
    {
       al_rest(1.0/FPS);
    }
}

void *search2(void*)
{
    while(_exit_==false)
    {
        al_rest(1.0/FPS);
    }
}

void *search3(void*)
{
    while(_exit_==false)
    {
        al_rest(1.0/FPS);
    }
}

void *main_loop(void *event_queue)
{
    ALLEGRO_EVENT_QUEUE* queue=(ALLEGRO_EVENT_QUEUE*)event_queue;
    fprintf(stdout,"exit"); 
    _exit_=true;
}