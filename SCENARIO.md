SCENARIO B1 — Simulation dynamique

Description du scénario

Le scénario B1 simule l’activité d’un réseau de bornes de recharge pour véhicules électriques.
Il utilise une liste d’événements (issue de events.h) représentant :
- le branchement d’un véhicule (slot occupé)
- le débranchement d’un véhicule (slot libéré)

Chaque événement modifie l’état d’une station (slots libres, timestamp, etc.), ce qui permet de reproduire l’évolution d’un réseau dans le temps.

Données manipulées
- Le réseau est initialisé à partir de données statiques (CSV/JSON).
- Chaque station possède au minimum : un identifiant, une puissance, un nombre de slots libres et un dernier timestamp d’activité.

Déroulement du scénario

Le scénario suit trois étapes principales :

Initialisation
Chargement des stations dans une structure adaptée (AVL).

Simulation
Un grand nombre d’événements est injecté dans une file FIFO puis traité, l’état des stations est mis à jour au fur et à mesure.

État final
Après simulation, le réseau reflète un usage réel : certaines stations deviennent saturées, d’autres restent disponibles.

Intérêt du scénario

B1 permet d’observer un réseau dynamique plutôt qu’un état statique.
Il met en évidence :
- la saturation possible de certaines zones
- la charge variable des stations
- la nécessité de structures efficaces pour mettre à jour les données

Conclusion

B1 modélise un usage réaliste d’un réseau de recharge. 
Le système doit suivre les mises à jour en temps réel pour conserver une vision cohérente du réseau, 
ce qui justifie l’emploi de structures adaptées (AVL, queue d’événements, etc.).