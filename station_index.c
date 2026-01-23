#include "station_index.h"
#include <stdlib.h>
#include <stdio.h>

//Opérateur ternaire pour récupérer la hauteur, s'il ne trouve pas -1
static int h(const StationNode* n){ return n ? n->height : -1; }

//Mise à jour de h selon ses enfants
static void upd(StationNode* n) {
    //On va chercher la hauteur des enfants
    int hl = h(n->left), hr = h(n->right);
    //On prend la hauteyur la plus grande et on ajoute le Node actuel donc 1.
    n->height = (hl > hr ? hl : hr) + 1;
}

//Création d'un StationNode
static StationNode* mk(int id, StationInfo in){
    //On alloue de l'espace
    StationNode* n = (StationNode*) malloc(sizeof *n);
    //Si ça ne marche pas, on retourne 0
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
//---A3 Filtrage Mixte (Pré-filtres + Postfix)---

// appel d'une fonction externe provenant de rules.c qu'on va utiliser dans notre fonction filter_ids_with_rule (eval_rule_postfix)
extern int eval_rule_postfix(char* toks[], int n, StationInfo* info);

int filter_ids_with_rule(StationNode* r, char* toks[], int n, int* out, int cap, int min_power, int min_slots) {
    // Si l'arbre est vide, le tableau de sortie n'existe pas ou si c'est plein, on return 0
    if (!r || !out || cap <= 0) return 0;
    // On initialise count à 0
    int count = 0;
    
    // Parcours gauche en premier en in-order
    count += filter_ids_with_rule(r->left, toks, n, out + count, cap - count, min_power, min_slots);
    

    // Traitement du nœud actuel avec pré-filtres avant d'appliquer les filtres 
    if (count < cap && r->info.power_kW >= min_power && r->info.slots_free >= min_slots) {        
        if (eval_rule_postfix(toks, n, &r->info)) {
            out[count++] = r->station_id;  
        }
    }
    
     // Parcours de la branche droite (toujours après avoir traité le nœud courant)
    count += filter_ids_with_rule(r->right, toks, n, out + count, cap - count, min_power, min_slots);
    
    return count;
}
//---A5 Export Snapshot CSV---

int si_export_csv_rec(StationNode* r, FILE* f) {
    //Si le sous-arbre est vide, on renvoie 0
    if (!r) return 0;
    //On créé un count qui est le nombre de lignes
    int count = 0;
    //On parcourt à gauche
    count += si_export_csv_rec(r->left, f);
    //On écrit les infos de r
    fprintf(
        //Dans f, avec le format csv avec les propriétés de StationInfo
        f,"%d,%d,%d,%d,%d\n",
        r->station_id,
        r->info.power_kW,
        r->info.price_cents,
        r->info.slots_free,
        r->info.last_ts
    );
    //On a écrit une ligne donc +1
    count++;
    //Et on fait pareil pour la droite
    count += si_export_csv_rec(r->right, f);
    //On retourne le nombre de lignes ajoutées
    return count;
}

//Import de l'AVL vers un csv trié par station_id
//Sortie :
// > 0 : c'est bon
// -1 : Chemin invalide
// -2 : Problème dans l'ouverture du fichier
int si_export_csv(StationNode* r, const char* path) {
    //On lit le fichier
    if (!path) {
        printf("[!!!] si_export_csv: Le chenmin est null\n");
        return -1;
    }
    //On ouvre le fichier
    FILE* f = fopen(path, "w");
    //S'il y a un problème, on renvoie -2
    if(!f) {
        printf("[!!!] Impossible d'écrire sur %s\n", path);
        return -2;
    }
    //On write les headers séparé par une virgule
    fprintf(f, "id,power,price,slots,last_ts\n");
    //On write via la récursive en in order et on retourne le nombre de lignes
    int n = si_export_csv_rec(r, f);
    //On ferme le fichier et on retourne
    fclose(f);
    return n;
}
//----------------------------

//-----A2 Top-K par score------
// Fonction récursive pour remplir le min-heap
void si_top_k_by_score_rec(StationNode* r, int k, int* heap, int* ids, int* count, int alpha, int beta, int gamma){
    //Si Node null, on retourne
    if(!r) return;
    //On visite le Node gauche
    si_top_k_by_score_rec(r->left, k, heap, ids, count, alpha, beta, gamma);
    //On calcule le score
    int score = r->info.slots_free * alpha + r->info.power_kW * beta - r->info.price_cents * gamma;
    //Si on peut encore mettre, on met la station dedans
    if(*count < k){
        //On met à jour les tableaux
        heap[*count] = score;
        ids[*count]  = r->station_id;
        //On incrémente le count
        (*count)++;
    } else {
        // trouver le plus petit score dans la boîte
        int min_idx = 0;
        for(int i = 1; i < k; i++)
            if(heap[i] < heap[min_idx])
                min_idx=i;
        if(score > heap[min_idx]){
            //On remplace le plus petit score
            heap[min_idx] = score;
            ids[min_idx]  = r->station_id;
        }
    }

    //Et on visite le Node droit
    si_top_k_by_score_rec(r->right, k, heap, ids, count, alpha, beta, gamma);
}

int si_top_k_by_score(StationNode* r, int k, int* out_ids, int alpha, int beta, int gamma){
    if(!r || k <= 0) return 0;
    int heap[k]; //liste de scores
    int ids[k]; //liste d'id
    int count = 0; //nombre de stations
    si_top_k_by_score_rec(r, k, heap, ids, &count, alpha, beta, gamma);
    // copie finale dans out_ids
    for(int i = 0; i < count; i++)
        out_ids[i] = ids[i];
    return count;
}
//----------------------------

// --- A1 ----

int si_range_ids_rec(StationNode* r, int lo, int hi, int* out, int cap, int* count){
    if(!r || *count >= cap) return 0;

    if(r->station_id > lo)
        si_range_ids_rec(r->left, lo, hi, out, cap, count);

    if(r->station_id >= lo && r->station_id <= hi && *count < cap){
        out[*count] = r->station_id;
        (*count)++;
    }

    if(r->station_id < hi)
        si_range_ids_rec(r->right, lo, hi, out, cap, count);

    return *count;
}

int si_range_ids(StationNode* r, int lo, int hi, int* out, int cap){
    int count = 0;
    si_range_ids_rec(r, lo, hi, out, cap, &count);
    return count;
}


int si_count_ge_power(StationNode* r, int P){
    if(!r) return 0;
    int count = 0;

    if(r->left) count += si_count_ge_power(r->left, P);
    if(r->info.power_kW >= P) count++;
    if(r->right) count += si_count_ge_power(r->right, P);

    return count;
}

//----------------------------

