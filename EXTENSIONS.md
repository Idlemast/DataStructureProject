EXTENSIONS — Modules avancés (A1, A2, A3, A5)

Nous avons implémenté 4 extensions avancées : A1, A2, A3 et A5.
Elles sont utilisées dans un ordre précis dans notre scénario (B1) afin d’obtenir un traitement cohérent du réseau.

Ordre d’exécution dans le scénario

Les extensions sont utilisées dans l’ordre suivant :

A2 — Top-K avant simulation
A3 — Filtrage avant simulation
A1 — Sélection d’une zone critique

Simulation B1

A2 — Top-K après simulation
A3 — Filtrage après simulation
A5 — Export final

Chaque extension sert un but bien précis dans la chaîne de traitement.

Détail des extensions
✔ A1 — Range Query (zones d’IDs)

Rôle dans le scénario :
Sélectionne une zone d’IDs (ex : [1010..1050]) afin de simuler une saturation locale.

Complexité :
Temps : O(log n + k)
Espace : O(k)
Utilité : Créer un cas réaliste (zone saturée → impact sur Top-K & filtrage).

✔ A2 — Top-K par score

Rôle dans le scénario :
Mesure la qualité des stations avant et après simulation.

Score : α * slots + β * puissance - γ * prix

Complexité :
Temps : O(n log n)
Espace : O(n)
Utilité : Observer l’évolution du “classement” des stations.

✔ A3 — Filtrage mixte (pré-filtres + postfix)

Rôle dans le scénario :
Isoler les stations valides avant et après simulation selon des règles comme :
(slots >= 1) && (power >= 50)

Complexité :
Temps : O(n)
Espace : O(k)
Utilité :
Analyser les changements de disponibilité liés à la simulation.

✔ A5 — Export CSV (snapshot)

Rôle dans le scénario :
Exporter l’état final dans snapshot.csv.

Complexité :
Temps : O(n)
Espace : O(1)
Utilité :
Garder une trace exploitable pour validation / analyse.

Conclusion
Les quatre extensions A1, A2, A3 et A5 ont été intégrées dans un scénario dynamique (B1) permettant :
- analyse statique initiale (A2 + A3)
- création d’une zone critique (A1)
- simulation réaliste d’activité (B1)
- comparaison avant/après (A2 + A3)
- archivage final (A5)

Cette organisation démontre l’intérêt pratique des extensions lorsqu’elles sont combinées.