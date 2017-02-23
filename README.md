# Benchmark
-----------
* Commentaires indispensables, décrire la fonction de la plupart des blocs, et ne pas attendre pour le faire
En sachant qu'on va utiliser Doxygen et dans les .h pour doxygen, et dans les .c pour les information relative au fonction
* Ne pas utiliser de valeurs à la volée dans le programme, utiliser uniquement des variables ou des constantes
* Tests sur les toutes les erreurs (pointeurs potentiellement nuls, longueurs dépassées, etc..)
* Nom des varaibles, classes et tout en anglais
* Tous les includes des classes/couples de fichiers seront dans les fichiers .h
* Indentation
* marquer balises TODO quand des choses ne sont pas faites
* Avant d'envoyer les fonctionnalités sur le Git du projet, il faut s'assurer que tout ce qu'on a fait fonctionne. Il faut faire des fichiers de test
* On ne touche pas aux fichiers des autres. On prévient uniquement les personnes des potentielles erreurs trouvées
* affichage à but informatif : cout ; affichage des erreurs : cerr
* Affichage standard des erreurs (en débug) : "[Nom de la classe] [Nom de la méthode] Descriptif de l'erreur"
* Afficher un message (en débug) lors de la création et de la destruction d'un objet/ d'une structure avec un cout
* factoriser les fonctions
* attributs d'instance avec un _ au debut, les static avec un s_ et les static const en majuscule
* le code en anglais et les commentaires en francais
* methode : references constante !!
* noter les horaires de travail 
* toute les execptions doivent etre attraper puis renvoyer avec le message proprie

OpenMP traitement parallele sur CPU
MPI transfert de message entre deux pc sur le meme programme
CUDA traitement parallele sur GPU
Gunrock3 generation de tres gros graphe
