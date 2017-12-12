Dans le cas où la valeur est un objet qui est destiné à être changé, 
  Pb: on a un formalisme différent en symbolique, où 
  
On passe une liste, c'est bien cette dernière qui est modifiée
    lst -> [a]
    <<  -> [a b] (puisqu'on garde le même pointeur)
    Pb: on se retrouve avec une copie dans le Executed.
        Sol 1: `value` est moved
        
Objectif: 
* ne pas deleter l'argument de set_lazy_val
* ne pas mettre le résultat dans le lazyfier quand c'est ce qu'on remplace

Rq: si on fait dir[...] avec dir symbolique et qu'on propose une valeur pour dir, il faut bien faire un remplacement


Attention: pb si on retourne un autre rp lors d'un self_op
    
Sécurité:
* Quand on démarre une instance, on est anonymous
* On peut ensuite (prétendre) être quelqu'un d'autre en donnant le login et le mot de passe ou la clé publique
    * il faut aussi dire qui a le droit de sauvegarder les données
* Pb quand on crée deux fois le même fichier (/test par exemple): ils vont avoir des inodes différents, et il faudra merger les inodes
    -> il faudra décider qui a raison
    -> il faudrait pouvoir différencier la création d'objet et la récupération d'objets extérieurs
        Si avec le même nom (/test par exemple) un objet est crée, et l'autre et récupéré, pour la version récupérée ça marchera si /test existe déjà dans le répértoire (et que l'inode )

Gestion des droits:
- on peut avoir 1 flag par type de patch
- autre proposition: une liste de droit + des méthodes 

Problème des droits dans les Map:
- a priori Map ne connait que add_key et rem_key comme droit spécifique.
- les droits pour les key et les val sont stockés dans les objets eux-même
- comment faire pour avoir des droits globaux ?
    - Rq 1: on pourrait laisser les devs se charger de modifier les droits de chaque item
    - mais par exemple, si on ajoute un fichier dans un répertoire, ça serait bien que ça soit avec des droits sympas par défaut. Prop: on utilise des types de droit pour dire ce qu'il se passe quand création
    - quand un utilisateur est ajouté dans un map, il faudrait que ce même utilisateur 
    - idéalement, il faudrait stocker

    
Rq pour les map: c'est con de stocker toutes les données des patches dans chaque clé et chaque valeur. L'idéal serait d'avoir
- un PatchMerger
- un get_changes qui donne uniquement les changements depuis le get_changes

Il reste quand même
+ la gestion des droits
+ les callbacks ? A priori, on peut les laisser dans les objets
+ les droits
- AutoPtr<Patch>       last_patch;
- mutable MapExtStates ext_states;     ///< data sent to other machine, known by other machines
- SetOfPatchId         patch_ids;      ///<

Proposition 1 pour les clé, valeur, ...:
- on garde tous les attributs, mais on ne les remplit pas (on pourra faire un pointeur qui rassemble le tout si on veut économiser de l'espace)
- on a une interface get_last_changes dans PatchCreator
- + un new_patch_merger pour faire les undo
    -> ça fait un pointeur en plus à stocker pour les phases de merger

Pourquoi on a ref et sym dans Lazyfier ?
- a priori, ref suffirait... sauf qu'on veut avoir une trace des opérations sur ref. Sym permet de le faire.

Pourquoi a-t-on besoin de symbol->lazifier ?
- on en a besoin dans set_lazy_val pour savoir ce que devient le symbol
- si on le veut 

Problème:
- on a s qui doit être utilisé dans un lazyfier, mais qui est changé pas un self_op. Rq: ça se repère avec le own qui est égal à 0


Pb si on a plusieurs Db,
- on peut avoir des dev_id identiques qui représentent des objets différents
- un même objet devrait avoir des patchs identiques d'une Db à l'autre... mais à ce moment là, il faut que get_changes_for et compagnie sachent pour quelle Db le message est

