#ifndef THREADS_INTERCOMMUNICATION_H
#define	THREADS_INTERCOMMUNICATION_H
#include "CTweets.h"

typedef char BOOL;
typedef unsigned int Uint;

//Devuelve true solamente despues de usar exit_threads().
BOOL get_exit_state(void);
//exit=true
void exit_threads(void);
//Devuelve NULL si no existe search asociado a search_num.
char* get_search_text(char search_num);
//Devuelve 0 si fue correcto, 1 si no existe search asociado a search_num.
BOOL new_search_text(char search_num,char*new_text);
//Siempre devuelve false si no existe search asociado a search_num
Uint get_search_ready(char search_num);
//Devuelve 1 si no exite search asociado a search_num, sino devuelve 0.
BOOL write_search_ready(char search_num,BOOL new_state);
//Devuelve NULL si no existe search asociado a search_num.
pCT get_search_array(char search_num);
//Devuelve 1 si no exite search asociado a search_num, sino devuelve 0.
BOOL write_search_array(char search_num,pCT new_array,Uint items_num);

#endif

