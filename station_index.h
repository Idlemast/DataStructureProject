#ifndef DS_STATION_INDEX_H
#define DS_STATION_INDEX_H
#include <stdio.h>

typedef struct StationInfo {
    int power_kW;      /* puissance */
    int price_cents;   /* tarif en centimes */
    int slots_free;    /* places libres */
    int last_ts;       /* dernière mise à jour */
} StationInfo;

typedef struct StationNode {
    int station_id;
    StationInfo info;
    struct StationNode* left;
    struct StationNode* right;
    int height;
} StationNode;

typedef struct StationIndex {
    StationNode* root;
} StationIndex;


//  A3 Filtre les stations d’un AVL selon pré-filtres + règle postfix et retourne leurs IDs.
int filter_ids_with_rule(StationNode* r, char* toks[], int n_toks, int* out, int cap, int min_power, int min_slots);

void si_init(StationIndex* idx);
StationNode* si_find(StationNode* r, int id);           /* O(log n) */
void si_add(StationIndex* idx, int id, StationInfo in); /* AVL insert : O(log n) */
int  si_delete(StationIndex* idx, int id);              /* AVL delete : O(log n) */
int  si_to_array(StationNode* r, int* ids, int cap);    /* inorder fill : O(n)*/
void si_print_sideways(StationNode* r);                 /* debug : O(n)*/
void si_clear(StationIndex* idx);
int si_export_csv(StationNode* r, const char* path);
int si_export_csv_rec(StationNode* r, FILE* f);
int si_top_k_by_score(StationNode* r, int k, int* out_ids, int alpha, int beta, int gamma);
int si_range_ids(StationNode* r, int lo, int hi, int* out, int cap);
int si_count_ge_power(StationNode* r, int P);

#endif
