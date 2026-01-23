Projet ChargeCraft - Module : Structure de données

BDML 2
- Paul Lo
- William Wan
- Teddy Rakotoarivelo

Ce projet simule la gestion de stations de recharge pour véhicules électriques à l’aide de plusieurs structures de données. 
L’objectif est d’indexer, manipuler et filtrer des stations selon différents critères, tout en gérant des évènements simulés sur les stations (branchement / débranchement).

Données
- izivia_tp_subset.csv : données principales des stations (300 entrées)
- izivia_tp_min.json : données complémentaires (10 entrées)
- events.h : liste d’événements pour le scénario B1

Fonctionnalités implémentées
- A1 Range Query sur l’AVL : recherche de stations par intervalle d’IDs
- A2 Top-K : sélection des stations les mieux notées selon un score
- A3 Filtrage : pré-filtres + règles postfix
- A5 Export CSV : export snapshot des stations
- B1 Simulation d’événements : branchement/débranchement + MRU des véhicules

Exécution
Les modules A (A1, A2, A3, A5) sont exécutés avant le scénario B1, car ils travaillent sur les données statiques chargées depuis les fichiers CSV/JSON.
Le scénario B1 utilise ensuite ces données pour simuler des événements de branchement/débranchement et mettre à jour les stations, ce qui nécessite que la structure (AVL + infos) soit déjà initialisée.
