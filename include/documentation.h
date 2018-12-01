/** \mainpage Application Spoutnik

\section intro Introduction

Ce logiciel a été proposé pour l'UE LIFPROJET, Université Claude Bernard, Lyon 1.
Il a été créé par :
    - ALLAM Mourad (n° étudiant : 11509098), contact : mourad.allam@etu.univ-lyon1.fr
    - SEQUEIRA Gabriel (n° étudiant : 11508289), contact : gabriel.sequeira@etu.univ-lyon1.fr
    - EUVRARD-BLANC Nicolas (n° étudiant : 11508330), contact : nicolas-gilles.euvrard-blanc@etu.univ-lyon1.frma

Adresse du commanditaire : 23 avenue Pierre de Coubertin, 69100 Villeurbanne - Bâtiment Nautibus, campus La DOUA

Adresse de dépôt forge : https://forge.univ-lyon1.fr/p1508330/Spoutnik.git

<br>

\section compil Compilation

Ce programme se compile de manière automatique, vous n'avez qu'une simple commande à faire :

sur LINUX :
    Ouvrez un terminal (ctrl+alt+T)
    Déplacez-vous dans le dossier adéquat
    Exécutez la commande $ qmake
    Exécutez la commande $ make
    lancez le serveur avec $ ./bin/Spoutnik serveur
    lancez un client avec $ ./bin/Spoutnik client

sur macOS :
    identique à Linux avec les commandes associées

Dépendances :
 - Qt 5.8: https://www.qt.io/download/

 <br>

 \section Using Utilisation
 Ce logiciel permet de récupérer vos emails et de les déchiffrer en cas de réception chiffrée.
Il peut également effectuer l'inverse en chiffrant un message pour une personne à partir du moment 
où vous disposez de son adresse mail !

\section doc Générer la documentation
Dans le cadre du projet, nous fournissons une documentation développée du logiciel :
    Ouvrez un terminal
    executez la commande : firefox ~/Documents/Spoutnik/doc/html/index.html &

La documentation a été réalisée à l'aide de l'outil Doxygene, disponible ici : http://www.stack.nl/~dimitri/doxygen/

Les modules commentés sont les suivant :
    - Chiffrement
    - Dechiffrement
    - Outils
    - Socket
    - PrivateKeyGenerator
    - Client

<br>

\section lol Organisation de l'archive

Le logiciel se découpe en plusieurs dossiers :<br>
        bin     => Contient les executables<br>
        data    => Contient les fichiers multimédias<br>
        doc     => Contient toute la documentation du logiciel<br>
        include => Contient les en-têtes des codes sources.<br>
        obj     => Contient les différents fichiers objets (.o)<br>
        src     => Contient les codes sources c++<br>

        Makefile est un fichier permettant de compiler de manière regroupée les différents fichiers.<br>

*/
