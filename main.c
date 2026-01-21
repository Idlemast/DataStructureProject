#include <stdio.h>
#include "events.h"
#include "queue.h"
#include "slist.h"
#include "station_index.h"
#include "csv_loader.h"
#include "json_loader.h"

#define MAX_VEH 100
#define MRU_CAP 5

SList VEH_MRU[MAX_VEH];

//Mise à jour de la dernière station visitée par un véhicule
void add_to_mru(int veh_id, int station_id){
    //On vérifie que veh_id est dans les limites
    if(veh_id < 0 || veh_id >= MAX_VEH) return;
    //Supprime la station de la liste
    ds_slist_remove_value(&VEH_MRU[veh_id], station_id);
    //Et la rajoute en tête de liste, pour éviter les doublons
    ds_slist_insert_head(&VEH_MRU[veh_id], station_id);
    int drop;
    int count = 0;
    SNode* c = VEH_MRU[veh_id].head;
    //On compte le nombre de valeurs
    while(c){ count++; c = c->next; }
    //Si ça dépasse la limite fixée, on supprime
    if(count > MRU_CAP) ds_slist_remove_tail(&VEH_MRU[veh_id], &drop);
}

void process_events(Queue* q, StationIndex* idx){
    Event e; //Pour stocker l'Event
    while(q_dequeue(q, &e)){
        add_to_mru(e.vehicle_id, e.station_id);
        //On va chercher un AVL par id
        StationNode* sn = si_find(idx->root, e.station_id);
        StationInfo info;
        //Si on trouve, on prend ses infos
        if(sn){ info=sn->info; }
        //On lui assigne des valeurs par défaut, s'il n'y a rien
        else {
            info.power_kW = 50;
            info.price_cents = 300;
            info.slots_free = 2;
            info.last_ts = 0;
        }
        //Si on branche
        if(e.action == 1) {
            if(info.slots_free > 0)
                info.slots_free--;
        }
        //Si on débranche
        if(e.action == 0) {
            info.slots_free++;
        }
        //On met à jour le timestamp
        info.last_ts = e.ts;
        //Et on met à jour l'AVL
        si_add(idx, e.station_id, info);
    }
}

int eval_rule_postfix(char* toks[], int n, StationInfo* info); /* from rules.c */

int main(void){
    for(int i=0;i<MAX_VEH;i++) ds_slist_init(&VEH_MRU[i]);

    StationIndex idx; si_init(&idx);
    Queue q; q_init(&q);

    /* Load provided datasets (place files next to binary or run from project root) */
    int c1 = ds_load_stations_from_csv("izivia_tp_subset.csv", &idx);
    printf("CSV loaded: %d stations\n", c1);
    int c2 = ds_load_stations_from_json("izivia_tp_min.json", &idx);
    printf("JSON loaded: %d stations (optional)\n", c2);

    for(int i = 0; i < DS_EVENTS_COUNT; i++) q_enqueue(&q, DS_EVENTS[i]);
    process_events(&q, &idx);

    printf("\nAVL stations (sideways):\n");
    si_print_sideways(idx.root);

    /* Simple rule: power>=50 && slots>=1 */
    char* rule[] = { "slots", "1", ">=", "power", "50", ">=", "&&" };
    int ids[64]; int k = si_to_array(idx.root, ids, 64);
    printf("\nStations satisfying rule: ");
    //Convertit l'AVL en tableau (in-order)
    for(int i = 0; i < k; i++){
        StationNode* s = si_find(idx.root, ids[i]);
        //Rechercher par id
        if(s && eval_rule_postfix(rule, 7, &s->info)) {
            printf("%d ", ids[i]);
        }
    }
    printf("\n");
    printf("MRU vehicle 3: "); ds_slist_print(&VEH_MRU[3]);
    //---A3 Filtrage Mixte (Pré-filtres + Postfix)---
    printf(" MODULE A3 : ");
    
    char* rule[] = {"slots","1",">=","power","100",">=","&&"};
    int results[300];
    int count = filter_ids_with_rule(idx.root, rule, 7, results, 300, 100, 1    
    );
    
    printf("Pré-filtres: power>=50, slots>=1\n");
    printf("Règle postfix: (slots>=1) && (power>=50)\n");
    printf("Stations trouvées: %d\n", count);
    printf("Exemples: ");
    for (int i = 0; i < count && i < 10; i++) {
        printf("%d ", results[i]);
    }
    if (count > 10) printf("+");
    printf("\n");
    // si_clear(&idx);
    //---A5 Export Snapshot CSV---
    int n = si_export_csv(idx.root, "snapshot.csv");
    if (n >= 0)
        printf("[csv] %d lignes écrites\n", n);
    else
        printf("[csv] Export échoué\n");

    //----------------------------
    //-----A2 Top-K par score------

    int top[5];
    int k2 = si_top_k_by_score(idx.root, 5, top, 2, 1, 1);
    printf("Top 5 des stations : ");
    for(int i = 0; i < k2; i++) printf("%d ", top[i]);
    printf("\n");

    //----------------------------
    return 0;
}
