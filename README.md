Projet ChargeCraft - Module : Structure de données

BDML 2
- Paul Lo
- William Wan
- Teddy Rakotoarivelo


Ce projet simule le fonctionnement d’un réseau de stations de recharge pour véhicules électriques.
Il combine des structures de données et des algorithmes afin de :
- charger des données réelles depuis CSV/JSON,
- organiser les stations dans un AVL,
- réaliser des requêtes (range queries, filtrage, top-K),
- simuler des événements dynamiques (branchement/débranchement),
- exporter des snapshots d’état.


Données utilisées: 
- izivia_tp_subset.csv → dataset principal (≈300 stations)
- izivia_tp_min.json → dataset complémentaire (10 stations)
- events.h → liste d’événements simulés (scénario)

Les attributs d’une station incluent :
puissance, slots libres, prix, horodatage, id, opérateur…


Structures de données:
- AVL (Arbre binaire équilibré) : index station_id → StationInfo
   → permet : insertion, recherche, top-K, filtrage
- Liste chaînée (SList) : MRU des véhicules (dernières stations)
- Queue (FIFO) : file d’événements à traiter

Le scénario B1 regroupe tous les modules dans un cycle avant → simulation → après :

1. Chargement des données
ds_load_stations_from_csv(...)

2. A2 — Top-K avant simulation
Permet de voir le "meilleur classement" initial.

3. A3 — Filtrage avant simulation
Application pré-filtres + règle postfix.

4. A1 — Range Query pour saturation
On sélectionne un intervalle d'IDs (ex: [1010..1050])
puis on force slots_free = 0 → zone saturée

5. Simulation B1
On exécute des milliers d’événements :
- process_events(&q, &idx);
- Les slots et timestamps sont mis à jour.

6. A2 / A3 — Après simulation
- On vérifie l'impact de la simulation :
-le classement change (Top-K dynamique)
- les candidats filtrés changent

7. A5 — Export final CSV
Export de l’AVL en snapshot.csv.

Intérêt de la combinaison
Cette intégration permet de :
- observer l’évolution dynamique du réseau
- évaluer l’impact de la saturation
- voir si les meilleures stations changent
- comparer filtrage avant / après
- avoir une trace finale exportée

Ce n’est pas juste de l’algorithme, c’est un scénario réaliste.

Exécution

Le programme affiche :
-Top-K avant / après
-Stations filtrées avant / après
-Intervalle saturé
-Export CSV final
 
Conclusion
-Le scénario B1 a été volontairement enrichi avec les modules A afin de créer une simulation cohérente :
- A1 pour cibler une zone critique,
- A2/A3 pour comparer l’état du réseau avant/après,
- A5 pour garder une trace,
- B1 pour la dynamique.

Ce choix montre l’évolution du réseau sous contraintes, reproduisant des mécanismes proches d’un système réel.
