#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "threads_intercommunication.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include "CTweets.h"

typedef Location dot;

#define WIDTH 900
#define HEIGHT 500
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

#define FOREVER true
#define SLEEP 0.2
#define CONTINENTS 6
#define XBOX 10
#define YBOX -25
#define NO_SEARCH 0
#define URLDIF 12
#define BACKSPACE 8
#define RADIO 4
#define FADE 1
#define CNI 31 //caracter no imprimible

////////////////////PROTOTIPOS////////////////////
void start_thread(pthread_t*thread,void*(*func)(void*));
void *search(void*);
void main_loop(ALLEGRO_DISPLAY*display,ALLEGRO_BITMAP*world,ALLEGRO_FONT*font,ALLEGRO_EVENT_QUEUE*event_queue);
void init_CTweets(void);
void init_allegro(ALLEGRO_DISPLAY**display,ALLEGRO_BITMAP**world,ALLEGRO_FONT**font,ALLEGRO_EVENT_QUEUE**event_queue);
void shutdown_allegro(ALLEGRO_DISPLAY*display,ALLEGRO_BITMAP*world,ALLEGRO_FONT*font);
void get_continents(int search_ready,CTweet*search_array,Uint*continents,Location*puntos,Uint i);

////////////////////MAIN////////////////////
int main(void)
{
    init_CTweets();
    ALLEGRO_DISPLAY*display;
    ALLEGRO_BITMAP*world;
    ALLEGRO_FONT*font;
    ALLEGRO_EVENT_QUEUE*event_queue;
    init_allegro(&display,&world,&font,&event_queue);
    pthread_t search_thread;
    start_thread(&search_thread,&search);
    main_loop(display,world,font,event_queue);
    pthread_join(search_thread,NULL);
    return 0;
}

////////////////////THREADS////////////////////
void start_thread(pthread_t*thread,void*(*func)(void*))
{
    if(pthread_create(thread,NULL,func,NULL))
    {
        fprintf(stderr,"pthread_create failed\n");
        exit(12);
    }
}

void *search(void*)
{
    char*err;
    Uint count;
    pCT search1_buffer,search2_buffer,search3_buffer;
    while(!get_exit_state())
    {
        if(!get_search_ready(1)&&get_search_text(1)[0])
        {
            count=1;
            search1_buffer=CT_search(get_search_text(1),&count,err);
            if((err==NULL)&&(search1_buffer!=NULL)&&count)
            {
                write_search_array(1,search1_buffer,count);
                write_search_ready(1,count);
            }
        }
        if(!get_search_ready(2)&&get_search_text(2)[0])
        {
            count=1;
            search2_buffer=CT_search(get_search_text(2),&count,err);
            if((err==NULL)&&(search2_buffer!=NULL)&&count)
            {
                write_search_array(2,search2_buffer,count);
                write_search_ready(2,count);
            }
        }
        if(!get_search_ready(3)&&get_search_text(3)[0])
        {
            count=1;
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

void main_loop(ALLEGRO_DISPLAY*display,ALLEGRO_BITMAP*world,ALLEGRO_FONT*font,ALLEGRO_EVENT_QUEUE*event_queue)
{
    char live=false;
    Uint i,j,k,URLi;
    Uint continentes1[6]={0,0,0,0,0,0};
    Uint continentes2[6]={0,0,0,0,0,0};
    Uint continentes3[6]={0,0,0,0,0,0};
    ALLEGRO_EVENT event;
    char text1[141]={"Click here to start typing, then press enter to start searching."};
    char text2[141]={0};
    char text3[141]={0};
    char search1_text[140*3+1];
    char search2_text[140*3+1];
    char search3_text[140*3+1];
    dot puntos1[(int)(FADE/SLEEP)];
    dot puntos2[(int)(FADE/SLEEP)];
    dot puntos3[(int)(FADE/SLEEP)];
    char search_ready=NO_SEARCH;
    for(i=0;i<(int)(FADE/SLEEP);i++)
    {
        puntos1[i].latitude=10000;
        puntos1[i].longitude=10000;
        puntos2[i].latitude=10000;
        puntos2[i].longitude=10000;
        puntos3[i].latitude=10000;
        puntos3[i].longitude=10000;
    }
    while(FOREVER)
    {
        if(!al_is_event_queue_empty(event_queue))
        {
            al_get_next_event(event_queue,&event);
            switch(event.type)
            {
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    if((event.mouse.x>20)&&(event.mouse.x<880))
                    {
                        if(event.mouse.y<25)
                        {
                            search_ready=1;
                            text1[0]=0;
                            search1_text[0]=0;
                            i=0;
                            URLi=0;
                        }
                        else if(event.mouse.y<50)
                        {
                            search_ready=2;
                            text2[0]=0;
                            search2_text[0]=0;
                            i=0;
                            URLi=0;
                        }
                        else if(event.mouse.y<75)
                        {
                            search_ready=3;
                            text3[0]=0;
                            search3_text[0]=0;
                            i=0;
                            URLi=0;
                        }
                        else
                        {
                            switch(search_ready)
                            {
                                case 1:
                                    new_search_text(1,search1_text);
                                    write_search_ready(1,false);
                                    for(k=0;k<6;k++)continentes1[k]=0;
                                    break;
                                case 2:
                                    new_search_text(2,search2_text);
                                    write_search_ready(2,false);
                                    for(k=0;k<6;k++)continentes2[k]=0;
                                    break;
                                case 3:
                                    new_search_text(3,search3_text);
                                    write_search_ready(3,false);
                                    for(k=0;k<6;k++)continentes3[k]=0;
                                    break;
                            }
                            search_ready=NO_SEARCH;
                        }
                    }
                    if((event.mouse.x>27)&&(event.mouse.x<150)&&(event.mouse.y>370)&&(event.mouse.y<410))
                        live=!live;
                    break;
                case ALLEGRO_EVENT_KEY_CHAR:
                    if(event.keyboard.unichar>CNI)
                    {
                        switch(search_ready)
                        {
                            case 1:
                                text1[i++]=event.keyboard.unichar;
                                text1[i]=0;
                                if(((event.keyboard.unichar>=' ')&&(event.keyboard.unichar<='/'))||((event.keyboard.unichar>=':')&&(event.keyboard.unichar<='@'))||((event.keyboard.unichar>='[')
                                  &&(event.keyboard.unichar<='`'))||((event.keyboard.unichar>='{')&&(event.keyboard.unichar<=255)))
                                {
                                    search1_text[URLi++]='%';
                                    search1_text[URLi++]=(char)((event.keyboard.unichar-URLDIF)/10)+'0';
                                    search1_text[URLi++]=((event.keyboard.unichar-URLDIF)%10)+'0';
                                }
                                else search1_text[URLi++]=event.keyboard.unichar;
                                search1_text[URLi]=0;
                                break;
                            case 2:
                                text2[i++]=event.keyboard.unichar;
                                text2[i]=0;
                                if(((event.keyboard.unichar>=' ')&&(event.keyboard.unichar<='/'))||((event.keyboard.unichar>=':')&&(event.keyboard.unichar<='@'))||((event.keyboard.unichar>='[')
                                  &&(event.keyboard.unichar<='`'))||((event.keyboard.unichar>='{')&&(event.keyboard.unichar<=255)))
                                {
                                    search2_text[URLi++]='%';
                                    search2_text[URLi++]=(char)((event.keyboard.unichar-URLDIF)/10)+'0';
                                    search2_text[URLi++]=((event.keyboard.unichar-URLDIF)%10)+'0';
                                }
                                else search2_text[URLi++]=event.keyboard.unichar;
                                search2_text[URLi]=0;
                                break;
                            case 3:
                                text3[i++]=event.keyboard.unichar;
                                text3[i]=0;
                                if(((event.keyboard.unichar>=' ')&&(event.keyboard.unichar<='/'))||((event.keyboard.unichar>=':')&&(event.keyboard.unichar<='@'))||((event.keyboard.unichar>='[')
                                  &&(event.keyboard.unichar<='`'))||((event.keyboard.unichar>='{')&&(event.keyboard.unichar<=255)))
                                {
                                    search3_text[URLi++]='%';
                                    search3_text[URLi++]=(char)((event.keyboard.unichar-URLDIF)/10)+'0';
                                    search3_text[URLi++]=((event.keyboard.unichar-URLDIF)%10)+'0';
                                }
                                else search3_text[URLi++]=event.keyboard.unichar;
                                search3_text[URLi]=0;
                                break;
                        }
                    }
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                    switch(event.keyboard.keycode)
                    {
                        case ALLEGRO_KEY_ESCAPE:
                            shutdown_allegro(display,world,font);
                            exit_threads();
                            return;
                        case ALLEGRO_KEY_ENTER:
                            switch(search_ready)
                            {
                                case 1:
                                    new_search_text(1,search1_text);
                                    write_search_ready(1,false);
                                    for(k=0;k<6;k++)continentes1[k]=0;
                                    break;
                                case 2:
                                    new_search_text(2,search2_text);
                                    write_search_ready(2,false);
                                    for(k=0;k<6;k++)continentes2[k]=0;
                                    break;
                                case 3:
                                    new_search_text(3,search3_text);
                                    write_search_ready(3,false);
                                    for(k=0;k<6;k++)continentes3[k]=0;
                                    break;
                            }
                            search_ready=NO_SEARCH;
                            break;
                        case ALLEGRO_KEY_BACKSPACE:
                            if(i)
                                switch(search_ready)
                                {
                                    case 1:
                                        text1[--i]=0;
                                        search1_text[--URLi]=0;
                                        if(search1_text[URLi-1]=='%')
                                        {
                                            search1_text[--URLi]=0;
                                            search1_text[--URLi]=0;
                                        }
                                        break;
                                    case 2:
                                        text2[--i]=0;
                                        search2_text[--URLi]=0;
                                        if(search2_text[URLi-1]=='%')
                                        {
                                            search2_text[--URLi]=0;
                                            search2_text[--URLi]=0;
                                        }
                                        break;
                                    case 3:
                                        text3[--i]=0;
                                        search3_text[--URLi]=0;
                                        if(search3_text[URLi-1]=='%')
                                        {
                                            search3_text[--URLi]=0;
                                            search3_text[--URLi]=0;
                                        }
                                        break;
                                }
                            break;
                    }
                    break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    shutdown_allegro(display,world,font);
                    exit_threads();
                    return;
            }
        }
        //DRAWING
        al_clear_to_color(COLOR(WHITE));
        al_draw_bitmap(world,0,50,0);
        al_draw_filled_rounded_rectangle(XBOX,YBOX+50,XBOX+880,YBOX+100,25,25,COLOR(GREEN));
        al_draw_filled_rounded_rectangle(XBOX,YBOX+25,XBOX+880,YBOX+75,25,25,COLOR(BLUE));
        al_draw_filled_rounded_rectangle(XBOX,YBOX,XBOX+880,YBOX+50,25,25,COLOR(RED));
        al_draw_text(font,al_map_rgb(255,255,255),XBOX+20,YBOX+25,ALLEGRO_ALIGN_LEFT,text1);
        al_draw_text(font,al_map_rgb(255,255,255),XBOX+20,YBOX+75,ALLEGRO_ALIGN_LEFT,text3);
        al_draw_text(font,al_map_rgb(255,255,255),XBOX+20,YBOX+50,ALLEGRO_ALIGN_LEFT,text2);
        if(get_search_ready(1))
            get_continents(get_search_ready(1),get_search_array(1),continentes1,puntos1,j);
        if(get_search_ready(2))
            get_continents(get_search_ready(2),get_search_array(2),continentes2,puntos2,j);
        if(get_search_ready(3))
            get_continents(get_search_ready(3),get_search_array(3),continentes3,puntos3,j);
        if((++j)==(int)(FADE/SLEEP))j=0;
        //AMERICA
        al_draw_filled_circle(150,170,RADIO*continentes1[0],COLOR(RED));
        al_draw_filled_circle(200,170,RADIO*continentes2[0],COLOR(BLUE));
        al_draw_filled_circle(250,170,RADIO*continentes3[0],COLOR(GREEN));
        //EUROPA
        al_draw_filled_circle(428,176,RADIO*continentes1[1],COLOR(RED));
        al_draw_filled_circle(450,150,RADIO*continentes2[1],COLOR(BLUE));
        al_draw_filled_circle(470,120,RADIO*continentes3[1],COLOR(GREEN));
        //ANTARTIDA
        al_draw_filled_circle(450,470,RADIO*continentes1[5],COLOR(RED));
        al_draw_filled_circle(400,470,RADIO*continentes2[5],COLOR(BLUE));
        al_draw_filled_circle(470,470,RADIO*continentes3[5],COLOR(GREEN));
        //AFRICA
        al_draw_filled_circle(490,370,RADIO*continentes1[2],COLOR(RED));
        al_draw_filled_circle(500,250,RADIO*continentes2[2],COLOR(BLUE));
        al_draw_filled_circle(500,300,RADIO*continentes3[2],COLOR(GREEN));
        //ASIA
        al_draw_filled_circle(728,176,RADIO*continentes1[3],COLOR(RED));
        al_draw_filled_circle(750,150,RADIO*continentes2[3],COLOR(BLUE));
        al_draw_filled_circle(670,180,RADIO*continentes3[3],COLOR(GREEN));
        //OCEANIA
        al_draw_filled_circle(728,376,RADIO*continentes1[4],COLOR(RED));
        al_draw_filled_circle(750,350,RADIO*continentes2[4],COLOR(BLUE));
        al_draw_filled_circle(770,380,RADIO*continentes3[4],COLOR(GREEN));
        //LIVE DOTS
        al_draw_filled_rounded_rectangle(25,370,150,410,5,5,COLOR(BLACK));
        al_draw_text(font,COLOR(WHITE),32,380,ALLEGRO_ALIGN_LEFT,"GO LIVE!");
        if(!live)al_draw_filled_circle(130,390,7,COLOR(WHITE));
        else
        {
            al_draw_filled_circle(130,390,7,COLOR(RED));
            for(k=0;k<(int)(FADE/SLEEP);k++)
                if(puntos1[k].longitude<9000)al_draw_filled_circle((((puntos1[k].longitude)/180)*(WIDTH/2))+(WIDTH/2),-(((puntos1[k].latitude)/90)*((HEIGHT-50)/2))+50+((HEIGHT-50)/2),2,COLOR(RED));
            for(k=0;k<(int)(FADE/SLEEP);k++)
                if(puntos2[k].longitude<9000)al_draw_filled_circle((((puntos2[k].longitude)/180)*(WIDTH/2))+(WIDTH/2),-(((puntos2[k].latitude)/90)*((HEIGHT-50)/2))+50+((HEIGHT-50)/2),2,COLOR(BLUE));
            for(k=0;k<(int)(FADE/SLEEP);k++)
                if(puntos3[k].longitude<9000)al_draw_filled_circle((((puntos3[k].longitude)/180)*(WIDTH/2))+(WIDTH/2),-(((puntos3[k].latitude)/90)*((HEIGHT-50)/2))+50+((HEIGHT-50)/2),2,COLOR(GREEN));
        }
        al_flip_display();
        //DEBUG
        for(k=0;k<get_search_ready(1);k++)
        {
            printf("user: %s\n"     , get_search_array(1)[k].user);
            printf("userName: %s\n" , get_search_array(1)[k].userName);
            printf("text: %s\n"     , get_search_array(1)[k].text);
            printf("lang: %s\n"     , get_search_array(1)[k].lang);
            printf("lat:  %f\n"     , get_search_array(1)[k].loc.latitude );
            printf("long: %f\n\n"   , get_search_array(1)[k].loc.longitude);
            fflush(stdout);
        }
        for(k=0;k<get_search_ready(2);k++)
        {
            printf("user: %s\n"     , get_search_array(2)[k].user);
            printf("userName: %s\n" , get_search_array(2)[k].userName);
            printf("text: %s\n"     , get_search_array(2)[k].text);
            printf("lang: %s\n"     , get_search_array(2)[k].lang);
            printf("lat:  %f\n"     , get_search_array(2)[k].loc.latitude );
            printf("long: %f\n\n"   , get_search_array(2)[k].loc.longitude);
            fflush(stdout);
        }
        for(k=0;k<get_search_ready(3);k++)
        {
            printf("user: %s\n"     , get_search_array(3)[k].user);
            printf("userName: %s\n" , get_search_array(3)[k].userName);
            printf("text: %s\n"     , get_search_array(3)[k].text);
            printf("lang: %s\n"     , get_search_array(3)[k].lang);
            printf("lat:  %f\n"     , get_search_array(3)[k].loc.latitude );
            printf("long: %f\n\n"   , get_search_array(3)[k].loc.longitude);
            fflush(stdout);
        }
        write_search_ready(1,false);
        write_search_ready(2,false);
        write_search_ready(3,false);
        sleep(SLEEP);
    }
}

////////////////////FUNCIONES////////////////////
void get_continents(int search_ready,CTweet*search_array,Uint*continents,dot*puntos,Uint i)
{
    Uint k;
    for(k=0;k<search_ready;k++)
    {
        if((search_array[k].loc.longitude<-30)&&(search_array[k].loc.longitude>=-175)&&(search_array[k].loc.latitude<90)&&(search_array[k].loc.latitude>=-60))
            continents[0]++;
        if((search_array[k].loc.longitude<36)&&(search_array[k].loc.longitude>=-30)&&(search_array[k].loc.latitude>=36)&&(search_array[k].loc.latitude<90))
            continents[1]++;
        if((search_array[k].loc.longitude<36)&&(search_array[k].loc.longitude>=-30)&&(search_array[k].loc.latitude<36)&&(search_array[k].loc.latitude>-40))
            continents[2]++;
        if((search_array[k].loc.longitude>=36)&&(search_array[k].loc.longitude<180)&&(search_array[k].loc.latitude<90)&&(search_array[k].loc.latitude>-10))
            continents[3]++;
        if((search_array[k].loc.longitude>90)&&(search_array[k].loc.latitude<=-10)&&(search_array[k].loc.latitude>=-60))
            continents[4]++;
        if((search_array[k].loc.latitude<-60))
            continents[5]++;
        puntos[i].latitude=search_array[k].loc.latitude;
        puntos[i].longitude=search_array[k].loc.longitude;
    }
}

////////////////////CTWEETS////////////////////
void init_CTweets(void)
{
    char*err;
    if(CT_init(err)==NOT_OK)
    {
        fprintf(stderr,"%s\n",err);
        exit(11);
    }
}

////////////////////ALLEGRO////////////////////
void init_allegro(ALLEGRO_DISPLAY**display,ALLEGRO_BITMAP**world,ALLEGRO_FONT**font,ALLEGRO_EVENT_QUEUE**event_queue)
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
    *display=al_create_display(WIDTH,HEIGHT);
    if(!*display)
    {
        fprintf(stderr,"al_create_display failed\n");
        al_uninstall_keyboard();
        al_uninstall_mouse();
        al_shutdown_primitives_addon();
        exit(5);
    }
    al_set_window_position(*display,0,0);
    al_set_window_title(*display,"Tweets map.");
    al_clear_to_color(COLOR(WHITE));
    al_init_font_addon();
    if(!al_init_ttf_addon())
    {
        fprintf(stderr,"al_init_ttf_addon failed\n");
        al_uninstall_keyboard();
        al_uninstall_mouse();
        al_shutdown_primitives_addon();
        al_destroy_display(*display);
        al_shutdown_font_addon();
        exit(6);
    }
    if(!al_init_image_addon())
    {
        fprintf(stderr,"al_init_image_addon failed\n");
        al_uninstall_keyboard();
        al_uninstall_mouse();
        al_shutdown_primitives_addon();
        al_destroy_display(*display);
        al_shutdown_font_addon();
        al_shutdown_ttf_addon();
        exit(7);
    }
    *world=al_load_bitmap("map.png");
    if(*world==NULL)
    {
        fprintf(stderr,"al_load_bitmap failed\n");
        shutdown_allegro(*display,*world,NULL);
        exit(8);
    }
    al_draw_bitmap(*world,0,50,0);
    al_flip_display();
    *font=al_load_ttf_font("stocky.ttf",20,0);
    if (!*font)
    {
        fprintf(stderr,"al_load_ttf_font failed\n");
        shutdown_allegro(*display,*world,*font);
        exit(9);
    }
    *event_queue=al_create_event_queue();
    if (!*event_queue)
    {
        fprintf(stderr,"al_create_event_queue failed\n");
        shutdown_allegro(*display,*world,*font);
        exit(10);
    }
    al_register_event_source(*event_queue,al_get_keyboard_event_source());
    al_register_event_source(*event_queue,al_get_mouse_event_source());
    al_register_event_source(*event_queue,al_get_display_event_source(*display));
}

void shutdown_allegro(ALLEGRO_DISPLAY*display,ALLEGRO_BITMAP*world,ALLEGRO_FONT*font)
{
    al_uninstall_keyboard();
    al_uninstall_mouse();
    al_destroy_bitmap(world);
    al_destroy_display(display);
    al_shutdown_primitives_addon();
    al_destroy_font(font);
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_image_addon();
}