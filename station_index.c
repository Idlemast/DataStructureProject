#include "station_index.h"
#include <stdlib.h>
#include <stdio.h>

//Opérateur ternaire pour récupérer la hauteur, s'il ne trouve pas -1
static int h(StationNode* n){ return n ? n->height : -1; }

//Mise à jour de h selon ses enfants
static void upd(StationNode* n) {
    //On la chercher la hauteur des enfants
    int hl = h(n->left), hr = h(n->right);
    //On prend la hauteyur la plus grande et on ajoute le noeud actuel donc 1
    n->height = (hl > hr ? hl : hr) + 1;
}

//Création d'un StationNode
static StationNode* mk(int id, StationInfo in){
    //On alloue de l'espace
    StationNode* n = (StationNode*) malloc(sizeof *n);
    //Si ça ne marche pas on retourne 0
    if(!n) return 0;
    //On assigne id et in
    n->station_id = id;
    n->info = in;
    //Le reste est à 0 par défaut
    n->left = n->right = 0;
    n->height = 0;
    //Et on renvoie StationNode
    return n;
}

//Rééquilibrer un arbre sur la gauche
static StationNode* rotL(StationNode* x) {
    StationNode* y = x->right;
    //Sous-arbre qu'on va bouger
    StationNode* t2 = y->left;
    //On change les left et right
    y->left = x;
    x->right = t2;
    //On met à jour les hauteurs h
    upd(x);
    upd(y);
    //Et on retourne le nouveau sous-arbre
    return y;
}

//Rééquilibrer un arbre sur la droite
static StationNode* rotR(StationNode* y) {
    StationNode* x = y->left;
    StationNode* t2 = x->right;
    x->right = y;
    y->left = t2;
    upd(y);
    upd(x);
    return x;
}

static StationNode* rebalance(StationNode* n){
    //Mise à jour de h
    upd(n);
    //On calcule la diff de h entre left et right
    int bf = h(n->left) - h(n->right);
    //Si l'arbre n'est pas équilibré, alors rotation
    if(bf > 1) {
        if(h(n->left->right) > h(n->left->left))
            n->left=rotL(n->left);
        return rotR(n);
    }
    if(bf < -1) {
        if(h(n->right->left) > h(n->right->right))
            n->right = rotR(n->right);
        return rotL(n);
    }
    return n;
}

//Initialise l'AVL
void si_init(StationIndex* idx){ idx->root = 0; }

//Chercher un id
StationNode* si_find(StationNode* r, int id){
    while(r) {
        //Si r est inférieur à l'enfant de gauche
        if(id < r->station_id)
            r = r->left;
        //Si r est inférieur à l'enfant de droite
        else if(id > r->station_id)
            r = r->right;
        else return r;
    } return 0; //Si il ne trouve pas
}

//Insertion récursive de l'AVL
static StationNode* insert_rec(StationNode* r, int id, StationInfo in){
    //Si le Node est vide, on retourne l'insert
    if(!r) return mk(id, in);
    //On parcourt l'AVL
    if(id < r->station_id)
        r->left = insert_rec(r->left, id, in);
    else if(id > r->station_id)
        r->right = insert_rec(r->right, id, in);
    else {
        r->info = in;
        return r;
    } /* update */
    //On rééquilibre l'arbre
    return rebalance(r);
}

void si_add(StationIndex* idx, int id, StationInfo in) {
    idx->root = insert_rec(idx->root, id, in);
}

//Renvoie le plus petit Node, donc tout à gauche
static StationNode* min_node(StationNode* r) {
    StationNode* c = r;
    while(c && c->left)
        c=c->left;
    return c;
}

//Suppression récursive
static StationNode* delete_rec(StationNode* r, int id, int* found){
    //Si vide, retourne 0
    if(!r) return 0;
    //Parcourt à gauche et à droite
    if(id < r->station_id)
        r->left = delete_rec(r->left, id, found);
    else if(id > r->station_id)
        r->right = delete_rec(r->right, id, found);
    //Si c'est le bon
    else {
        *found = 1;
        //Si aucun enfant, alors free
        if(!r->left && !r->right) {
            free(r);
            return 0;
        //S'il n'y a pas d'enfant à gauche
        } else if(!r->left) {
            StationNode* t = r->right;
            //On free le bon Node
            free(r);
            //On retourne le Node en dessous
            return t;
        //S'il n'y a pas d'enfant à droite
        } else if(!r->right) {
            StationNode* t = r->left;
            //On free le bon Node
            free(r);
            //On retourne le Node en dessous
            return t;
        //S'il a 2 enfants
        } else {
            //Va chercher la valeur la plus petite de l'enfant à droite
            StationNode* s = min_node(r->right);
            //On prend ses infos
            r->station_id = s->station_id;
            r->info = s->info;
            //On supprime tout les enfants
            r->right = delete_rec(r->right, s->station_id, found);
        }
    }
    return rebalance(r);
}

//Pour supprimer un id, depuis la racine
int si_delete(StationIndex* idx, int id) {
    int f = 0;
    idx->root = delete_rec(idx->root, id, &f);
    return f;
}

//Parcours in-order pour remplir un tableau d'id (+)
int si_to_array(StationNode* r, int* ids, int cap){
    if(!r || cap <= 0) return 0;
    int w = 0, wl = si_to_array(r->left, ids, cap);
    w += wl;
    if(w < cap) {
        ids[w++] = r->station_id;
    }
    if(w < cap) {
        int wr = si_to_array(r->right, ids + w, cap - w);
        w += wr;
    }
    return w;
}

//free post-order
static void free_post(StationNode* r) {
    if(!r) return;
    free_post(r->left);
    free_post(r->right);
    free(r);
}

//Pour nettoyer tout l'arbre
void si_clear(StationIndex* idx) {
    free_post(idx->root);
    idx->root = 0;
}

void si_print_sideways(StationNode* r){
    static int depth = 0;
    if(!r) return;
    depth++;
    si_print_sideways(r->right);
    for(int i = 1; i < depth; i++)
        printf("    ");
    printf("%d(h=%d) P=%dKW Price=%dc Slots=%d\n", r->station_id, r->height, r->info.power_kW, r->info.price_cents, r->info.slots_free);
    si_print_sideways(r->left);
    depth--;
}
