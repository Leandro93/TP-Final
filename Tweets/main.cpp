#include <stdio.h>
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
#define BACKGROUND COLOR(WHITE)
#define SHOW() al_flip_display()

#define FOREVER true
#define FPS 60.0
#define SLEEP 1000000/FPS
#define CONTINENTS 7
#define XBOX 10
#define YBOX -25
#define NO_SEARCH 0
#define URLDIF 12
#define BACKSPACE 8
#define RADIUS 20.0
#define MIN_RADIUS 2.0
#define FADE 1
#define LIFE_SPAN (int)(FADE*FPS)
#define CNI 31 //caracter no imprimible

////////////////////PROTOTIPOS////////////////////
void start_thread(pthread_t*thread,void*(*func)(void*));
void *search(void*);
void main_loop(ALLEGRO_DISPLAY*display,ALLEGRO_BITMAP*world,ALLEGRO_FONT*font,ALLEGRO_EVENT_QUEUE*event_queue);
void init_CTweets(void);
void init_allegro(ALLEGRO_DISPLAY**display,ALLEGRO_BITMAP**world,ALLEGRO_FONT**font,ALLEGRO_EVENT_QUEUE**event_queue);
void shutdown_allegro(ALLEGRO_DISPLAY*display,ALLEGRO_BITMAP*world,ALLEGRO_FONT*font);
void get_continents(int search_ready,CTweet*search_array,Uint*continents,Uint*total);
void draw(ALLEGRO_BITMAP*world,ALLEGRO_FONT*font,char*text1,char*text2,char*text3,Uint*continentes1,Uint*continentes2,Uint*continentes3,Uint*total1,Uint*total2,Uint*total3,dot*puntos1,dot*puntos2,dot*puntos3,BOOL live);

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
    CT_searchReset();
    while(!get_exit_state())
    {
        if(!get_search_ready(1)&&get_search_text(1)[0])
        {
            set_minId(get_minID(1));
            count=1;
            search1_buffer=CT_search(get_search_text(1),&count,err);
            if((err==NULL)&&(search1_buffer!=NULL)&&count)
            {
                write_search_array(1,search1_buffer,count);
                write_search_ready(1,count);
            }
            set_minID(1,get_minId());
        }
        if(!get_search_ready(2)&&get_search_text(2)[0])
        {
            set_minId(get_minID(2));
            count=1;
            search2_buffer=CT_search(get_search_text(2),&count,err);
            if((err==NULL)&&(search2_buffer!=NULL)&&count)
            {
                write_search_array(2,search2_buffer,count);
                write_search_ready(2,count);
            }
            set_minID(2,get_minId());
        }
        if(!get_search_ready(3)&&get_search_text(3)[0])
        {
            set_minId(get_minID(3));
            count=1;
            search3_buffer=CT_search(get_search_text(3),&count,err);
            if((err==NULL)&&(search3_buffer!=NULL)&&count)
            {
                write_search_array(3,search3_buffer,count);
                write_search_ready(3,count);
            }
            set_minID(3,get_minId());
        }
        usleep(SLEEP);
    }
}

void main_loop(ALLEGRO_DISPLAY*display,ALLEGRO_BITMAP*world,ALLEGRO_FONT*font,ALLEGRO_EVENT_QUEUE*event_queue)
{
    BOOL live=false;
    Uint i,k,URLi,total1=0,total2=0,total3=0;
    Uint continentes1[CONTINENTS]={0,0,0,0,0,0,0};
    Uint continentes2[CONTINENTS]={0,0,0,0,0,0,0};
    Uint continentes3[CONTINENTS]={0,0,0,0,0,0,0};
    ALLEGRO_EVENT event;
    char text1[141]={"Click any of these bars to start typing, then press enter to start searching."};
    char text2[141]={"Press the \"GO LIVE!\" button to briefly show dots where tweets come from."};
    char text3[141]={"Circles next to the continents indicate the percentage of it in the world."};
    char search1_text[140*3+1];
    char search2_text[140*3+1];
    char search3_text[140*3+1];
    dot puntos1[LIFE_SPAN];
    dot puntos2[LIFE_SPAN];
    dot puntos3[LIFE_SPAN];
    char search_ready=NO_SEARCH;
    for(i=0;i<LIFE_SPAN;i++)
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
                    switch(search_ready)
                    {
                        case 1:
                            new_search_text(1,search1_text);
                            write_search_ready(1,false);
                            for(k=0;k<CONTINENTS;k++)continentes1[k]=0;
                            total1=0;
                            set_minID(1,"0");
                            break;
                        case 2:
                            new_search_text(2,search2_text);
                            write_search_ready(2,false);
                            for(k=0;k<CONTINENTS;k++)continentes2[k]=0;
                            total2=0;
                            set_minID(2,"0");
                            break;
                        case 3:
                            new_search_text(3,search3_text);
                            write_search_ready(3,false);
                            for(k=0;k<CONTINENTS;k++)continentes3[k]=0;
                            total3=0;
                            set_minID(3,"0");
                            break;
                    }
                    if((event.mouse.x>20)&&(event.mouse.x<880))
                    {
                        if(event.mouse.y<25)
                        {
                            text1[0]=0;
                            search1_text[0]=0;
                            i=0;
                            URLi=0;
                            search_ready=1;
                        }
                        else if(event.mouse.y<50)
                        {
                            text2[0]=0;
                            search2_text[0]=0;
                            i=0;
                            URLi=0;
                            search_ready=2;
                        }
                        else if(event.mouse.y<75)
                        {
                            text3[0]=0;
                            search3_text[0]=0;
                            i=0;
                            URLi=0;
                            search_ready=3;
                        }
                        else search_ready=NO_SEARCH;
                    }
                    else search_ready=NO_SEARCH;
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
                                    for(k=0;k<CONTINENTS;k++)continentes1[k]=0;
                                    total1=0;
                                    set_minID(1,"0");
                                    break;
                                case 2:
                                    new_search_text(2,search2_text);
                                    write_search_ready(2,false);
                                    for(k=0;k<CONTINENTS;k++)continentes2[k]=0;
                                    total2=0;
                                    set_minID(2,"0");
                                    break;
                                case 3:
                                    new_search_text(3,search3_text);
                                    write_search_ready(3,false);
                                    for(k=0;k<CONTINENTS;k++)continentes3[k]=0;
                                    total3=0;
                                    set_minID(3,"0");
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
                                        if((URLi>2)&&(search1_text[URLi-2]=='%'))
                                        {
                                            search1_text[--URLi]=0;
                                            search1_text[--URLi]=0;
                                        }
                                        break;
                                    case 2:
                                        text2[--i]=0;
                                        search2_text[--URLi]=0;
                                        if((URLi>2)&&(search2_text[URLi-2]=='%'))
                                        {
                                            search2_text[--URLi]=0;
                                            search2_text[--URLi]=0;
                                        }
                                        break;
                                    case 3:
                                        text3[--i]=0;
                                        search3_text[--URLi]=0;
                                        if((URLi>2)&&(search3_text[URLi-2]=='%'))
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
        draw(world,font,text1,text2,text3,continentes1,continentes2,continentes3,&total1,&total2,&total3,puntos1,puntos2,puntos3,live);
        write_search_ready(1,false);
        write_search_ready(2,false);
        write_search_ready(3,false);
        usleep(SLEEP);
    }
}

////////////////////FUNCIONES////////////////////
void get_continents(int search_ready,CTweet*search_array,Uint*continents,Uint*total)
{
    Uint k;
    for(k=0;k<search_ready;k++)
    {
        if((search_array[k].loc.longitude<-25)&&(search_array[k].loc.longitude>=-175)&&(search_array[k].loc.latitude<90)&&(search_array[k].loc.latitude>=15))
            continents[0]++;
        else if((search_array[k].loc.longitude<36)&&(search_array[k].loc.longitude>=-30)&&(search_array[k].loc.latitude>=36)&&(search_array[k].loc.latitude<90))
            continents[1]++;
        else if((search_array[k].loc.longitude<36)&&(search_array[k].loc.longitude>=-30)&&(search_array[k].loc.latitude<36)&&(search_array[k].loc.latitude>-40))
            continents[2]++;
        else if((search_array[k].loc.longitude>=36)&&(search_array[k].loc.longitude<180)&&(search_array[k].loc.latitude<90)&&(search_array[k].loc.latitude>-10))
            continents[3]++;
        else if((search_array[k].loc.longitude>90)&&(search_array[k].loc.latitude<=-10)&&(search_array[k].loc.latitude>=-60))
            continents[4]++;
        else if((search_array[k].loc.latitude<-60))
            continents[5]++;
        else if((search_array[k].loc.longitude<-25)&&(search_array[k].loc.longitude>=-100)&&(search_array[k].loc.latitude>=-60)&&(search_array[k].loc.latitude<15))
            continents[6]++;
        if(search_array[k].loc.latitude<=90)(*total)++;
    }
}

void draw(ALLEGRO_BITMAP*world,ALLEGRO_FONT*font,char*text1,char*text2,char*text3,Uint*continentes1,Uint*continentes2,Uint*continentes3,Uint*total1,Uint*total2,Uint*total3,dot*puntos1,dot*puntos2,dot*puntos3,BOOL live)
{
    static Uint j=0,k=0;
    al_clear_to_color(BACKGROUND);
    al_draw_bitmap(world,0,50,0);
    al_draw_filled_rounded_rectangle(XBOX,YBOX+50,XBOX+880,YBOX+100,25,25,COLOR(GREEN));
    al_draw_filled_rounded_rectangle(XBOX,YBOX+25,XBOX+880,YBOX+75,25,25,COLOR(BLUE));
    al_draw_filled_rounded_rectangle(XBOX,YBOX,XBOX+880,YBOX+50,25,25,COLOR(RED));
    al_draw_text(font,COLOR(WHITE),XBOX+10,YBOX+25,ALLEGRO_ALIGN_LEFT,text1);
    al_draw_text(font,COLOR(WHITE),XBOX+10,YBOX+75,ALLEGRO_ALIGN_LEFT,text3);
    al_draw_text(font,COLOR(WHITE),XBOX+10,YBOX+50,ALLEGRO_ALIGN_LEFT,text2);
    if(get_search_ready(1))
    {
        get_continents(get_search_ready(1),get_search_array(1),continentes1,total1);
        puntos1[j].latitude=get_search_array(1)[0].loc.latitude;
        puntos1[j].longitude=get_search_array(1)[0].loc.longitude;
    }
    else
    {
        puntos1[j].latitude=10000;
        puntos1[j].longitude=10000;
    }
    if(get_search_ready(2))
    {
        get_continents(get_search_ready(2),get_search_array(2),continentes2,total2);
        puntos2[j].latitude=get_search_array(2)[0].loc.latitude;
        puntos2[j].longitude=get_search_array(2)[0].loc.longitude;
    }
    else
    {
        puntos2[j].latitude=10000;
        puntos2[j].longitude=10000;
    }
    if(get_search_ready(3))
    {
        get_continents(get_search_ready(3),get_search_array(3),continentes3,total3);
        puntos3[j].latitude=get_search_array(3)[0].loc.latitude;
        puntos3[j].longitude=get_search_array(3)[0].loc.longitude;
    }
    else
    {
        puntos3[j].latitude=10000;
        puntos3[j].longitude=10000;
    }
    if((j+1)<LIFE_SPAN)j++;else j=0;
    //AMERICA DEL NORTE
    if(total1&&continentes1[0])
        if(((RADIUS*continentes1[0])/(*total1))>MIN_RADIUS)
            if(((RADIUS*continentes1[0])/(*total1))<RADIUS)
                al_draw_filled_circle(90,170,(RADIUS*continentes1[0])/(*total1),COLOR(RED));
            else
                al_draw_filled_circle(90,170,RADIUS,COLOR(RED));
        else
            al_draw_filled_circle(90,170,MIN_RADIUS,COLOR(RED));
    if(total2&&continentes2[0])
        if(((RADIUS*continentes2[0])/(*total2))>MIN_RADIUS)
            if(((RADIUS*continentes2[0])/(*total2))<RADIUS)
                al_draw_filled_circle(118.28,198.28,(RADIUS*continentes2[0])/(*total2),COLOR(BLUE));
            else
                al_draw_filled_circle(118.28,198.28,RADIUS,COLOR(BLUE));
        else
            al_draw_filled_circle(118.28,198.28,MIN_RADIUS,COLOR(BLUE));
    if(total3&&continentes3[0])
        if(((RADIUS*continentes3[0])/(*total3))>MIN_RADIUS)
            if(((RADIUS*continentes3[0])/(*total3))<RADIUS)
                al_draw_filled_circle(146.57,226.57,(RADIUS*continentes3[0])/(*total3),COLOR(GREEN));
            else
                al_draw_filled_circle(146.57,226.57,RADIUS,COLOR(GREEN));
        else
            al_draw_filled_circle(146.57,226.57,MIN_RADIUS,COLOR(GREEN));
    //AMERICA DEL SUR
    if(total1&&continentes1[6])
        if(((RADIUS*continentes1[6])/(*total1))>MIN_RADIUS)
            if(((RADIUS*continentes1[6])/(*total1))<RADIUS)
                al_draw_filled_circle(230,310,(RADIUS*continentes1[6])/(*total1),COLOR(RED));
            else
                al_draw_filled_circle(230,310,RADIUS,COLOR(RED));
        else
            al_draw_filled_circle(230,310,MIN_RADIUS,COLOR(RED));
    if(total2&&continentes2[6])
        if(((RADIUS*continentes2[6])/(*total2))>MIN_RADIUS)
            if(((RADIUS*continentes2[6])/(*total2))<RADIUS)
                al_draw_filled_circle(230,350,(RADIUS*continentes2[6])/(*total2),COLOR(BLUE));
            else
                al_draw_filled_circle(230,350,RADIUS,COLOR(BLUE));
        else
            al_draw_filled_circle(230,350,MIN_RADIUS,COLOR(BLUE));
    if(total3&&continentes3[6])
        if(((RADIUS*continentes3[6])/(*total3))>MIN_RADIUS)
            if(((RADIUS*continentes3[6])/(*total3))<RADIUS)
                al_draw_filled_circle(230,390,(RADIUS*continentes3[6])/(*total3),COLOR(GREEN));
            else
                al_draw_filled_circle(230,390,RADIUS,COLOR(GREEN));
        else
            al_draw_filled_circle(230,390,MIN_RADIUS,COLOR(GREEN));
    //EUROPA
    if(total1&&continentes1[1])
        if(((RADIUS*continentes1[1])/(*total1))>MIN_RADIUS)
            if(((RADIUS*continentes1[1])/(*total1))<RADIUS)
                al_draw_filled_circle(400,140,(RADIUS*continentes1[1])/(*total1),COLOR(RED));
            else
                al_draw_filled_circle(400,140,RADIUS,COLOR(RED));
        else
            al_draw_filled_circle(400,140,MIN_RADIUS,COLOR(RED));
    if(total2&&continentes2[1])
        if(((RADIUS*continentes2[1])/(*total2))>MIN_RADIUS)
            if(((RADIUS*continentes2[1])/(*total2))<RADIUS)
                al_draw_filled_circle(400,180,(RADIUS*continentes2[1])/(*total2),COLOR(BLUE));
            else
                al_draw_filled_circle(400,180,RADIUS,COLOR(BLUE));
        else
            al_draw_filled_circle(400,180,MIN_RADIUS,COLOR(BLUE));
    if(total3&&continentes3[1])
        if(((RADIUS*continentes3[1])/(*total3))>MIN_RADIUS)
            if(((RADIUS*continentes3[1])/(*total3))<RADIUS)
                al_draw_filled_circle(365.36,160,(RADIUS*continentes3[1])/(*total3),COLOR(GREEN));
            else
                al_draw_filled_circle(365.36,160,RADIUS,COLOR(GREEN));
        else
            al_draw_filled_circle(365.36,160,MIN_RADIUS,COLOR(GREEN));
    //ANTARTIDA
    if(total1&&continentes1[5])
        if(((RADIUS*continentes1[5])/(*total1))>MIN_RADIUS)
            if(((RADIUS*continentes1[5])/(*total1))<RADIUS)
                al_draw_filled_circle(410,420,(RADIUS*continentes1[5])/(*total1),COLOR(RED));
            else
                al_draw_filled_circle(410,420,RADIUS,COLOR(RED));
        else
            al_draw_filled_circle(410,420,MIN_RADIUS,COLOR(RED));
    if(total2&&continentes2[5])
        if(((RADIUS*continentes2[5])/(*total2))>MIN_RADIUS)
            if(((RADIUS*continentes2[5])/(*total2))<RADIUS)
                al_draw_filled_circle(450,420,(RADIUS*continentes2[5])/(*total2),COLOR(BLUE));
            else
                al_draw_filled_circle(450,420,RADIUS,COLOR(BLUE));
        else
            al_draw_filled_circle(450,420,MIN_RADIUS,COLOR(BLUE));
    if(total3&&continentes3[5])
        if(((RADIUS*continentes3[5])/(*total3))>MIN_RADIUS)
            if(((RADIUS*continentes3[5])/(*total3))<RADIUS)
                al_draw_filled_circle(490,420,(RADIUS*continentes3[5])/(*total3),COLOR(GREEN));
            else
                al_draw_filled_circle(490,420,RADIUS,COLOR(GREEN));
        else
            al_draw_filled_circle(490,420,MIN_RADIUS,COLOR(GREEN));
    //AFRICA
    if(total1&&continentes1[2])
        if(((RADIUS*continentes1[2])/(*total1))>MIN_RADIUS)
            if(((RADIUS*continentes1[2])/(*total1))<RADIUS)
                al_draw_filled_circle(450,285,(RADIUS*continentes1[2])/(*total1),COLOR(RED));
            else
                al_draw_filled_circle(450,285,RADIUS,COLOR(RED));
        else
            al_draw_filled_circle(450,285,MIN_RADIUS,COLOR(RED));
    if(total2&&continentes2[2])
        if(((RADIUS*continentes2[2])/(*total2))>MIN_RADIUS)
            if(((RADIUS*continentes2[2])/(*total2))<RADIUS)
                al_draw_filled_circle(450,325,(RADIUS*continentes2[2])/(*total2),COLOR(BLUE));
            else
                al_draw_filled_circle(450,325,RADIUS,COLOR(BLUE));
        else
            al_draw_filled_circle(450,325,MIN_RADIUS,COLOR(BLUE));
    if(total3&&continentes3[2])
        if(((RADIUS*continentes3[2])/(*total3))>MIN_RADIUS)
            if(((RADIUS*continentes3[2])/(*total3))<RADIUS)
                al_draw_filled_circle(415.36,305,(RADIUS*continentes3[2])/(*total3),COLOR(GREEN));
            else
                al_draw_filled_circle(415.36,305,RADIUS,COLOR(GREEN));
        else
            al_draw_filled_circle(415.36,305,MIN_RADIUS,COLOR(GREEN));
    //ASIA
    if(total1&&continentes1[3])
        if(((RADIUS*continentes1[3])/(*total1))>MIN_RADIUS)
            if(((RADIUS*continentes1[3])/(*total1))<RADIUS)
                al_draw_filled_circle(820,220,(RADIUS*continentes1[3])/(*total1),COLOR(RED));
            else
                al_draw_filled_circle(820,220,RADIUS,COLOR(RED));
        else
            al_draw_filled_circle(820,220,MIN_RADIUS,COLOR(RED));
    if(total2&&continentes2[3])
        if(((RADIUS*continentes2[3])/(*total2))>MIN_RADIUS)
            if(((RADIUS*continentes2[3])/(*total2))<RADIUS)
                al_draw_filled_circle(840,185.36,(RADIUS*continentes2[3])/(*total2),COLOR(BLUE));
            else
                al_draw_filled_circle(840,185.36,RADIUS,COLOR(BLUE));
        else
            al_draw_filled_circle(840,185.36,MIN_RADIUS,COLOR(BLUE));
    if(total3&&continentes3[3])
        if(((RADIUS*continentes3[3])/(*total3))>MIN_RADIUS)
            if(((RADIUS*continentes3[3])/(*total3))<RADIUS)
                al_draw_filled_circle(860,220,(RADIUS*continentes3[3])/(*total3),COLOR(GREEN));
            else
                al_draw_filled_circle(860,220,RADIUS,COLOR(GREEN));
        else
            al_draw_filled_circle(860,220,MIN_RADIUS,COLOR(GREEN));
    //OCEANIA
    if(total1&&continentes1[4])
        if(((RADIUS*continentes1[4])/(*total1))>MIN_RADIUS)
            if(((RADIUS*continentes1[4])/(*total1))<RADIUS)
                al_draw_filled_circle(710,350,(RADIUS*continentes1[4])/(*total1),COLOR(RED));
            else
                al_draw_filled_circle(710,350,RADIUS,COLOR(RED));
        else
            al_draw_filled_circle(710,350,MIN_RADIUS,COLOR(RED));
    if(total2&&continentes2[4])
        if(((RADIUS*continentes2[4])/(*total2))>MIN_RADIUS)
            if(((RADIUS*continentes2[4])/(*total2))<RADIUS)
                al_draw_filled_circle(710,390,(RADIUS*continentes2[4])/(*total2),COLOR(BLUE));
            else
                al_draw_filled_circle(710,390,RADIUS,COLOR(BLUE));
        else
            al_draw_filled_circle(710,390,MIN_RADIUS,COLOR(BLUE));
    if(total3&&continentes3[4])
        if(((RADIUS*continentes3[4])/(*total3))>MIN_RADIUS)
            if(((RADIUS*continentes3[4])/(*total3))<RADIUS)
                al_draw_filled_circle(750,390,(RADIUS*continentes3[4])/(*total3),COLOR(GREEN));
            else
                al_draw_filled_circle(750,390,RADIUS,COLOR(GREEN));
        else
            al_draw_filled_circle(750,390,MIN_RADIUS,COLOR(GREEN));
    //TOTAL TWEETS
    al_draw_filled_rounded_rectangle(25,265,162,300,5,5,COLOR(RED));
    al_draw_filled_rounded_rectangle(25,290,162,325,5,5,COLOR(BLUE));
    al_draw_filled_rounded_rectangle(25,315,162,350,5,5,COLOR(GREEN));
    al_draw_text(font,COLOR(BLACK),25,248,ALLEGRO_ALIGN_LEFT,"Total tweets");
    al_draw_textf(font,COLOR(WHITE),30,270,ALLEGRO_ALIGN_LEFT,"%u",*total1);
    al_draw_textf(font,COLOR(WHITE),30,295,ALLEGRO_ALIGN_LEFT,"%u",*total2);
    al_draw_textf(font,COLOR(WHITE),30,320,ALLEGRO_ALIGN_LEFT,"%u",*total3);
    //LIVE LIFE_SPAN
    if(!live)al_draw_filled_rounded_rectangle(25,370,150,410,5,5,COLOR(RED));
    else
    {
        al_draw_filled_rounded_rectangle(25,370,150,410,5,5,COLOR(GREEN));
        for(k=0;k<LIFE_SPAN;k++)
            if(puntos1[k].longitude<9000)al_draw_filled_circle((((puntos1[k].longitude)/180)*(WIDTH/2))+(WIDTH/2),-(((puntos1[k].latitude)/90)*((HEIGHT-50)/2))+50+((HEIGHT-50)/2),2,COLOR(RED));
        for(k=0;k<LIFE_SPAN;k++)
            if(puntos2[k].longitude<9000)al_draw_filled_circle((((puntos2[k].longitude)/180)*(WIDTH/2))+(WIDTH/2),-(((puntos2[k].latitude)/90)*((HEIGHT-50)/2))+50+((HEIGHT-50)/2),2,COLOR(BLUE));
        for(k=0;k<LIFE_SPAN;k++)
            if(puntos3[k].longitude<9000)al_draw_filled_circle((((puntos3[k].longitude)/180)*(WIDTH/2))+(WIDTH/2),-(((puntos3[k].latitude)/90)*((HEIGHT-50)/2))+50+((HEIGHT-50)/2),2,COLOR(GREEN));
    }
    al_draw_text(font,COLOR(WHITE),45,380,ALLEGRO_ALIGN_LEFT,"GO LIVE!");
    SHOW();
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
    al_clear_to_color(BACKGROUND);
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
    SHOW();
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