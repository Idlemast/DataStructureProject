#include "queue.h"
#include <stdlib.h>

void q_init(Queue* q) {
    //On met head et tail en null
    q->head = q->tail = 0;
}

int q_is_empty(const Queue* q) {
    return q->head == 0;
}

int q_enqueue(Queue* q, Event e){
    //On malloc l'espace d'un QNode
    QNode* n = (QNode*) malloc(sizeof *n);
    //Si le malloc échoue, alors on retourne 0
    if(!n) return 0;
    //On assigne l'évènement e au nouveau QNode
    n->e = e;
    //On assigne le next en null puisque le QNode est le dernier
    n->next = 0;
    //Si la file est vide
    if(!q->tail) {
        //Le nouveau QNode devient le head et le tail
        q->head = q->tail = n;
    } else { //Sinon
        //L'ancien dernier QNode pointe vers le nouveau QNode
        q->tail->next = n;
        //On met à jour tail pour qu'elle pointe vers le nouveau QNode
        q->tail = n;
    }
    //Et on retourne 1 pour indiquer que l'ajout est ok
    return 1;
}

//Pour retirer le 1er élément de la file
int q_dequeue(Queue* q, Event* out){
    //On pointe sur le 1er QNode
    QNode* h = q->head;
    //Si c'est vide, on retourne 0
    if(!h) return 0;
    //Si on peut récupérer le pointeur de l'Event
    if(out) *out = h->e;
        //head = 2nd QNode
        q->head = h->next;
    //Si juste après la file est vide, on remet tail à null
    if(!q->head) q->tail=0;
    //On libère la mémoire de l'ancien QNode et on retourne 1
    free(h);
    return 1;
}

void q_clear(Queue* q) {
    Event dump;
    //Tant qu'on peut utiliser q_dequeue, on le fait
    while(q_dequeue(q, &dump)){}
}
