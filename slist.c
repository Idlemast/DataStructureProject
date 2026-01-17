#include "slist.h"
#include <stdlib.h>
#include <stdio.h>

//On pointe vers l et on dit que head est null
void ds_slist_init(SList* l){ l->head = 0; }

//Pour ajouter une valeur en début de liste
int ds_slist_insert_head(SList* l, int v){
    //On alloue de la mémoire pour le nouveau SNode
    SNode* n = (SNode*) malloc(sizeof *n);
    //Si il n'y arrive pas, on retourne 0
    if(!n) return 0;
    //On stocke la valeur
    n->value = v;
    //On attache le nouveau head
    n->next = l->head;
    //On dit que le nouveau head c'est le nouveau SNode
    l->head = n;
    //Et on dit que c'est ok
    return 1;
}

//On supprime la première occurrence de v
int ds_slist_remove_value(SList* l, int v){
    //Pointeur vers le SNode précédent, au début null
    SNode* p = 0;
    //Pointeur vers le SNode actuel, ici head
    SNode* c = l->head;
    //Tant qu'on n'est pas à la fin de SList et que la valeur n'a pas été trouvée
    while(c && c->value != v) {
        //Le précédent devient le courant
        p = c;
        //Et le courant devient le suivant
        c = c->next;
    }
    //Si on a pas trouvé, on return 0
    if(!c) return 0;
    //Si p est null
    if(!p) {
        //On enlève le 1er SNode de SList
        l->head = c->next;
    } else {
        //On replug SList sans c, on le saute
        p->next = c->next;
    }
    //On libère c et c'est ok
    free(c);
    return 1;
}

//On supprime le dernier SNode de SList
int ds_slist_remove_tail(SList* l, int* out){
    //Si c'est vide, on retourne 0
    if(!l->head) return 0;
    //Si la liste ne contient qu'un SNode
    if(!l->head->next) {
        //On garde la valeur
        if(out) *out = l->head->value;
        //On libère le SNode
        free(l->head);
        //On dit que la liste est vide
        l->head = 0;
        //Et on est bon
        return 1;
    }
    //Pointeur qui est sur le précédent
    SNode* p = 0;
    //Pointeur qui est sur le 1er SNode, pour l'instant sur head
    SNode* c = l->head;
    //Tant qu'il y a un suivant
    while(c->next) {
        //On bouge les 2 pointeurs
        p = c;
        c=c->next;
    }
    //On prend la valeur du dernier
    if(out) *out = c->value;
    //Le précédent est donc le dernier
    p->next = 0;
    //On free c
    free(c);
    //Et c'est ok
    return 1;
}

void ds_slist_print(const SList* l){
    SNode* c = l->head;
    printf("[");
    while(c) {
        printf("%d", c->value);
        if(c->next) printf(" -> ");
        c = c->next;
    }
    printf("]\n");
}

void ds_slist_clear(SList* l){
    SNode* c = l->head;
    while(c) {
        SNode* n = c->next;
        free(c);
        c = n;
    }
    l->head = 0;
}