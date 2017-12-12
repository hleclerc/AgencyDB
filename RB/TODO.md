* faire la création d'utilisateur
    * 
* faire le GenMap undo de rem_key_und
* undo, notamment dans les input

Curseurs avec read_cursor_ato(): on pourrait s'en passer pour tout (mouse_move, ...) mais pour le onFocus c'est complique.
* il faut qu'on fasse la différence

À prévoir
* cas ou 1 merge d'inode apparaît 2 fois, à des positions différentes

Problème du jour: aujourd'hui la sauvegarde des variables fait perdre des informations. On pourrait avoir deux versions:
* toutes les infos: utilisateur, patches, etc...
* ou uniquement ce qu'on aurait pour une copie
* Prop: la place que ça prend n'est donné que pour les types qui pourraient ne pas être connus de base (pour économiser en taille de message)


* interface avec coupures entres les machines (+ représentation des liens), ajout ok Aide machines...


Priorités:
- sécurité (authentification, droit sur objets)
- sauvegarde sur disque dur
- gestion des écrivains uniques (par exemple, entier atomique)
- envoi des requêtes pour potentielle exécution à distance
- gestion des types utilisateurs
- python
- JSON
- javascript
- positionnement automatique de la sauvegarde et des réplications
- démonstration de recovery
- gestion des grands nombre de lecteurs/écrivain (graphes de connectivité)
- streaming: requêtes adaptées, gestion de la sauvegarde des données non traitées
- répartition de charges
