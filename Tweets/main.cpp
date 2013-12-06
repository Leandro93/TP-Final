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

////////////////////VARIABLES COMPARTIDAS ENTRE THREADS////////////////////
BOOL _exit_=false;      //HABRIA QUE AGREGAR ESTAS VARIABLES EN UN ARCHIVO APARTE
char *search1_text;
char *search2_text;
char *search3_text;
char search1_ready=false;
char search2_ready=false;
char search3_ready=false;
//FALTAN LOS ARREGLOS DE CTWEETS, TODAVIA NO CONSIGO LA LIBRERIA

////////////////////PROTOTIPOS////////////////////
void *search1(void*);
void *search2(void*);
void *search3(void*);
void *main_loop(void* event_queue);
ALLEGRO_DISPLAY * init_allegro(void);
void shutdown_allegro(ALLEGRO_DISPLAY *display);
ALLEGRO_EVENT_QUEUE *create_event_queue(ALLEGRO_DISPLAY *display);

////////////////////FUNCIONES////////////////////
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

////////////////////THREADS////////////////////
void *search1(void*)    //SOLO BUSCAN Y COLOCAN EN EL ARREGLO LOS RESULTADOS Y EN EL READY EL NUMERO DE RESULTADOS ENCONTRADOS
{
    while(_exit_==false)
    {
        if(search1_ready=false)
        {
            
        }
       al_rest(1.0/FPS);
    }
}

void *search2(void*)
{
    while(_exit_==false)
    {
        if(search2_ready=false)
        {
            
        }
        al_rest(1.0/FPS);
    }
}

void *search3(void*)
{
    while(_exit_==false)
    {
        if(search3_ready=false)
        {
            
        }
        al_rest(1.0/FPS);
    }
}

void *main_loop(void *event_queue)      //THREAD QUE CONTIENE LAS FUNCIONES DE DIBUJO Y MANEJO DE LOS OTROS THREADS
{
    ALLEGRO_EVENT_QUEUE* queue=(ALLEGRO_EVENT_QUEUE*)event_queue;
    _exit_=true;
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

void shutdown_allegro(ALLEGRO_DISPLAY *display)
{
    al_uninstall_keyboard();
    al_uninstall_mouse();
    al_destroy_display(display);
    al_shutdown_primitives_addon();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
}

ALLEGRO_EVENT_QUEUE *create_event_queue(ALLEGRO_DISPLAY *display)
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
