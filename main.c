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

void scenario_b1() {
    printf("SCENARIO_B1\n");
    StationIndex idx; si_init(&idx);
    int k = 5;
    int min_id = 1010, max_id = 1050;
    int range_ids[300];
    int top[k];
    Queue q; q_init(&q);

    printf("Appuyez sur Entrée pour continuer");
    getchar();
    printf("On load le csv\n");

    int load = ds_load_stations_from_csv("../izivia_tp_subset.csv", &idx);
    // int found = si_range_ids(&idx, min_id, max_id, range_ids, 300);
    // r = l'arbre
    // k = le top 3, top 5
    // out = tableau des k stations
    // alpha = poids des places libres
    // beta = poids de la puissance
    // gamma = poids du prix

    printf("Appuyez sur Entrée pour continuer");
    getchar();
    printf("On affiche le top score selon des critères\n");

    int k1 = si_top_k_by_score(idx.root, k, top, 20, 2, 1);
    printf("Top-%d stations avant événements : ", k1);
    for(int i = 0; i < k1; i++)
        printf("%d ", top[i]);
    printf("\n");

    printf("Appuyez sur Entrée pour continuer");
    getchar();
    printf("On affiche tout les ids\n");

    char* rule[] = { "slots","1",">=","power","50",">=","&&" };
    int filt[64];
    int f = filter_ids_with_rule(idx.root, rule, 7, filt, 64, 50, 1);

    printf("Candidats avant les évènements: ");
    for (int i = 0; i < f; i++) printf("%d ", filt[i]);
    printf("\n");

    printf("Appuyez sur Entrée pour continuer");
    getchar();
    printf("On sature une plage de ids, ici 1010 à 1050\n");

    //On sature artificiellement les ids entre 1010 et 1050
    int found = si_range_ids(idx.root, min_id, max_id, range_ids, 256);
    printf("Zone saturée [%d..%d] (%d stations)\n", min_id, max_id, found);

    for(int i = 0; i < found; i++){
        StationNode* s = si_find(idx.root, range_ids[i]);
        if(s){
            s->info.slots_free = 0;  // saturation
        }
    }

    // Pleins d'évènements
    for(int step = 0; step < 5000; step++) {
        for(int i = 0; i < DS_EVENTS_COUNT; i++) {
            q_enqueue(&q, DS_EVENTS[i]);
        }
    }

    printf("Appuyez sur Entrée pour continuer");
    getchar();
    printf("Mise à jour des évènements et affichage top stations\n");

    //Mise à jour des stations
    process_events(&q, &idx);

    int k2 = si_top_k_by_score(idx.root, k, top, 20, 2, 1);
    printf("Top-%d stations après événements : ", k2);
    for(int i = 0; i < k2; i++)
        printf("%d ", top[i]);
    printf("\n");

    int f2 = filter_ids_with_rule(idx.root, rule, 7, filt, 64, 50, 1);

    printf("Appuyez sur Entrée pour continuer");
    getchar();
    printf("On filtre les ids après les évènements et avec le filtre excluant\n");

    printf("Candidats après les évènements :");
    for (int i = 0; i < f2; i++) printf("%d ", filt[i]);
    printf("\n");

    printf("Appuyez sur Entrée pour continuer");
    getchar();
    printf("On export et on clear\n");

    int export = si_export_csv(idx.root, "snapshot.csv");
    si_clear(&idx);
    q_clear(&q);
    printf("Export CSV : %d lignes écrites\n", export);
}


int main(void){
    // for(int i=0;i<MAX_VEH;i++) ds_slist_init(&VEH_MRU[i]);
    //
    // StationIndex idx; si_init(&idx);
    // Queue q; q_init(&q);

    /* Load provided datasets (place files next to binary or run from project root) */
    // int c1 = ds_load_stations_from_csv("../izivia_tp_subset.csv", &idx);
    // printf("CSV loaded: %d stations\n", c1);
    // int c2 = ds_load_stations_from_json("../izivia_tp_min.json", &idx);
    // printf("JSON loaded: %d stations (optional)\n", c2);

    // for(int i = 0; i < DS_EVENTS_COUNT; i++) q_enqueue(&q, DS_EVENTS[i]);
    // process_events(&q, &idx);
    //
    // printf("\nAVL stations (sideways):\n");
    // si_print_sideways(idx.root);
    //
    // //---A3 Filtrage Mixte (Pré-filtres + Postfix)---
    // printf(" MODULE A3 : ");

    // char* rule[] = {"slots","1",">=","power","100",">=","&&"};
    // int results[300];
    // int count = filter_ids_with_rule(idx.root, rule, 7, results, 300, 100, 1);

    // printf("Pré-filtres: power>=50, slots>=1\n");
    // printf("Règle postfix: (slots>=1) && (power>=50)\n");
    // printf("Stations trouvées: %d\n", count);
    // printf("Exemples: ");
    // for (int i = 0; i < count && i < 10; i++) {
        // printf("%d ", results[i]);
    // }
    // if (count > 10) printf("+");
    // printf("\n");
    // si_clear(&idx);


    // //---A5 Export Snapshot CSV---
    // int n = si_export_csv(idx.root, "snapshot.csv");
    // if (n >= 0)
    //     printf("[csv] %d lignes écrites\n", n);
    // else
    //     printf("[csv] Export échoué\n");
    //
    // //----------------------------
    // //-----A2 Top-K par score------
    //
    // int top[5];
    // int k2 = si_top_k_by_score(idx.root, 5, top, 2, 1, 1);
    // printf("Top 5 des stations : ");
    // for(int i = 0; i < k2; i++) printf("%d ", top[i]);
    // printf("\n");

    //----------------------------

    //-----A1 Range Query (ID)----

    // Test Range Query
    //printf("\n--- A1: Range Query Test ---\n");

    //int range_ids[300];
    // int min_id = 1010;
    // int max_id = 1050;
    //
    // int found = si_range_ids(idx.root, min_id, max_id, range_ids, 300);
    //
    // printf("IDs dans [%d, %d] = %d stations trouvées\n", min_id, max_id, found);
    //
    // for(int i = 0; i < found; i++){
    //     printf("%d ", range_ids[i]);
    // }
    // printf("\n");

    //----------------------------
    scenario_b1();
    return 0;
}
