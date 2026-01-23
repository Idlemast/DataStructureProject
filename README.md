Projet ChargeCraft - Module : Structure de données

BDML 2
- Paul Lo
- William Wan
- Teddy Rakotoarivelo

Description générale
Ce projet simule le fonctionnement d’un réseau de stations de recharge pour véhicules électriques. 
Il combine des structures de données et des algorithmes afin de charger des données réelles, 
organiser les stations dans une structure équilibrée, réaliser des requêtes de sélection et de filtrage, 
simuler des événements dynamiques sur le réseau, puis exporter des résultats pour analyse.

Objectifs techniques
- Charger des données depuis des fichiers externes (CSV et JSON)
- Stocker efficacement les stations via une structure en AVL
- Permettre des requêtes rapides sur les stations (range queries, filtrage, top-K)
- Mettre à jour dynamiquement l’état des stations à travers des événements simulés
- Exporter un snapshot de l’état final du réseau

Données utilisées
Le projet utilise trois types de fichiers :
- izivia_tp_subset.csv : dataset principal contenant environ 300 stations
- izivia_tp_min.json : dataset complémentaire contenant 10 stations supplémentaires
- events.h : liste d’événements simulés représentant les branchements et débranchements de véhicules

Chaque station possède plusieurs attributs, tels que : id de station, puissance en kW, nombre de slots libres, prix, opérateur, horodatage, etc.

Structures de données
Plusieurs structures sont utilisées pour assurer performances et modularité :
1. AVL Tree (arbre binaire équilibré)
   Utilisé pour indexer les stations par station_id. Il permet :
   - insertion en O(log n)
   - recherche en O(log n)
   - parcours trié implicite (in-order)
   - extraction top-K
   - filtrage

2. File FIFO (Queue)
   Utilisée pour stocker les événements dynamiques avant leur traitement.

3. Liste chaînée (SList)
   Utilisée pour maintenir des listes MRU (Most Recently Used) pour suivre les dernières stations visitées par un véhicule.

Modules A (analyse statique)
Les modules A opèrent sur un réseau statique, c’est-à-dire uniquement basé sur les données chargées, sans simulation d’événements.
Ils permettent un traitement analytique du réseau.

Module A1 : Range Query
Ce module permet de sélectionner rapidement les stations dont l’identifiant est dans un intervalle donné (exemple : [1010..1050]). 
Il est basé sur un parcours borné de l’AVL, ce qui réduit les coûts en ne visitant pas les branches inutiles.

Module A2 : Top-K
Ce module permet de calculer un classement des stations selon un score composite basé généralement sur :
- nombre de slots libres
- puissance
- prix
Ce score permet de classer les stations avant/après simulation et d’observer des changements de comportement.

Module A3 : Filtrage avancé
Ce module combine deux niveaux de filtrage :
- des pré-filtres (ex : puissance >= 50, slots >= 1)
- une règle postfix fournie sous forme de tokens, permettant une expression booléenne plus complexe
Exemple de règle postfix : slots 1 >= power 50 >= &&

Module A5 : Export CSV
Ce module exporte l’état actuel de l’AVL dans un fichier snapshot.csv. 
Ce snapshot permet d’archiver l’état du réseau après analyse ou après la simulation.

Module B (simulation dynamique)
Contrairement aux modules A, le module B travaille sur un réseau dynamique. 
Le scénario principal, appelé B1, simule le fonctionnement d’un réseau en conditions quasi réelles.
Des événements successifs modifient les états des stations :
- branchements : réduction du nombre de slots libres
- débranchements : augmentation de slots libres
- mise à jour de timestamps
- suivi MRU des véhicules

Organisation et enchaînement des modules dans B1
Le scénario B1 s’exécute en plusieurs étapes :
1. Chargement des données (CSV)
2. Calcul du Top-K initial (A2)
3. Filtrage initial (A3)
4. Range Query et saturation d’une zone (A1)
5. Simulation via traitement massif d’événements (B1)
6. Re-calcul des métriques après simulation (A2/A3)
7. Export final du snapshot (A5)

Intérêt de la combinaison des modules
L'intérêt de cette organisation est d’illustrer :
- l’état initial du réseau (statique)
- l’effet de contraintes locales (saturation d’une zone via A1)
- l’évolution du réseau dans le temps (simulation dynamique B1)
- les impacts sur le classement (A2) et le filtrage (A3)
- la possibilité de conserver l’état futur via export (A5)

Résultats visibles lors de l'exécution
L'exécution produit sur console :
- un Top-K initial
- un Top-K après simulation
- un filtrage avant/après
- un intervalle saturé via A1
- un export snapshot.csv écrit

Conclusion
Ce projet met en œuvre des concepts d’algorithmique, de structures de données et de simulation.
Le scénario B1 a été enrichi avec les modules A afin de fournir une simulation cohérente, où :
- A1 sert à cibler une zone critique
- A2 et A3 permettent de comparer des métriques avant/après
- A5 archive l’état final
- B1 fournit la dynamique
Ce choix illustre le comportement d’un réseau de bornes sous contraintes, proche d'un système réel de recharge.
