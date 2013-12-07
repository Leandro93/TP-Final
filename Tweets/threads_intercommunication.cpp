#include "threads_intercommunication.h"

#define TWEET_SIZE 140*3
#define ERROR 1

////////////////////VARIABLES COMPARTIDAS ENTRE THREADS////////////////////
BOOL Exit=false;
char s1_text[TWEET_SIZE+1];
char s2_text[TWEET_SIZE+1];
char s3_text[TWEET_SIZE+1];
Uint s1_ready=false;
Uint s2_ready=false;
Uint s3_ready=false;
CTweet s1_array[BUFFER_SIZE];
CTweet s2_array[BUFFER_SIZE];
CTweet s3_array[BUFFER_SIZE];

////////////////////FUNCIONES DE INTERFAZ////////////////////
BOOL get_exit_state(void)
{
    return Exit;
}

void exit_threads(void)
{
    Exit=true;
}

char* get_search_text(char search_num)
{
    switch(search_num)
    {
        case 1:
            return s1_text;
        case 2:
            return s2_text;
        case 3:
            return s3_text;
        default:
            return NULL;
    }
}

BOOL new_search_text(char search_num,char*new_text)
{
    Uint i;
    char*search=get_search_text(search_num);
    if(search==NULL) return ERROR;
    for(i=0;new_text[i];i++)
        search[i]=new_text[i];
    return 0;
}

Uint get_search_ready(char search_num)
{
    switch(search_num)
    {
        case 1:
            return s1_ready;
        case 2:
            return s2_ready;
        case 3:
            return s3_ready;
        default:
            return false;
    }
}

BOOL write_search_ready(char search_num,BOOL new_state)
{
    switch(search_num)
    {
        case 1:
            s1_ready=new_state;
            break;
        case 2:
            s2_ready=new_state;
            break;
        case 3:
            s3_ready=new_state;
            break;
        default:
            return ERROR;
    }
    return 0;
}

CTweet* get_search_array(char search_num)
{
    switch(search_num)
    {
        case 1:
            return s1_array;
        case 2:
            return s2_array;
        case 3:
            return s3_array;
        default:
            return NULL;
    }
}

BOOL write_search_array(char search_num,CTweet* new_array,Uint items_num)
{
    CTweet*array=get_search_array(search_num);
    if(array==NULL) return ERROR;
    Uint i;
    for(i=0;i<items_num;i++)
        array[i]=new_array[i];
    return 0;
}