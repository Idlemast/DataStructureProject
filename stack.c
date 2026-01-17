#include "stack.h"
#include <stdlib.h>

//On initialise une pile vide
void st_init(Stack* s){ s->top = 0; }

int st_push(Stack* s, int v){
    //On alloue l'espace
    StNode* n = (StNode*) malloc(sizeof*n);
    //Si y'a pas d'espace, on retourne 0
    if(!n) return 0;
    //Stocke la valeur
    n->v = v;
    //Pointe vers l'ancien sommet
    n->next = s->top;
    //Mise à jour du sommet
    s->top = n;
    //Et c'est bon
    return 1;
}

int st_pop(Stack* s, int* out){
    //On récupère le top
    StNode* t = s->top;
    //Si ça ne marche pas, on retourne 0
    if(!t) return 0;
    //On prend la valeur
    if(out) *out = t->v;
    //On met à jour le top
    s->top = t->next;
    //On free l'ancien top
    free(t);
    //Et c'est bon
    return 1;
}

int st_is_empty(Stack* s){ return s->top == 0; }
void st_clear(Stack* s){ int tmp; while(st_pop(s, &tmp)){} }
