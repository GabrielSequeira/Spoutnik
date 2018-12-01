############################# PRESENTATION #############################

Ce logiciel a été proposé pour l'UE LIFPROJET, Université Claude Bernard, Lyon 1.
Il a été créé par :
    - Mr X
    - ALLAM Mourad (n° étudiant : 11509098), contact : mourad.allam@etu.univ-lyon1.fr
    - SEQUEIRA Gabriel (n° étudiant : 11508289), contact : gabriel.sequeira@etu.univ-lyon1.fr

Adresse de dépôt forge : https://forge.univ-lyon1.fr/p1508330/Spoutnik.git

############################# DEVELOPPEMENT #############################

Ce logiciel a été développé en C++ à l'aide de différentes librairies :
    - GMP : https://gmplib.org/
    - openSSL : https://www.openssl.org/
    - Qt : https://www.qt.io/
    - Curl : https://curl.haxx.se/libcurl/

Outils :
    - Doxygen : http://www.stack.nl/~dimitri/doxygen/

Nous l'avons testé sous masOS, Ubuntu (>16.04) et Fedora

############################# COMPILATION #############################

Linux et macOS :
    -> Créer Makefile: $ qmake
    -> Compiler: $ make

############################# UTILISATION #############################

Ce logiciel est composé de 2 parties:
- le serveur (PKG)
- le client mail

Lancer le serveur:
    -> $ ./bin/Spoutnik serveur

Lancer le client:
    -> $ ./bin/Spoutnik client

Preuve que IMAP via Curl fonctionne:
    -> $ ./bin/Spoutnik test

############################# DOCUMENTATION #############################

La documentation a été réalisée à l'aide de l'outil Doxygen: 
    -> $ doxygen doc/image.doxy
    -> $ firefox ./doc/html/index.html &

Les modules commentés sont les suivant:
    - Chiffrement
    - Dechiffrement
    - Mail
    - Outils
    - PrivateKeyGenerator
    - Socket
    - Utilisateur
    - FenClient
    - FenConnexion
    - FormMail

############################# L'ARCHIVE #############################

bin/     => Contient les executables

data/    => Contient les données de l'application
    clients/ => contient un repertoire par utilisateur
        exemple@exemple.fr/
            mail_recu.txt => mail récupéré via CURL
            mails.txt     => tous les mails envoyés
            mpk.txt       => Master Public Key
            skid.txt      => Clé secréte
    pkg/     => données du PKG
        clients.txt => ses clients
        mpk.txt     => Master Public Key
        msk.txt     => Master Secret Key
    utilisateurs.txt => utilisateurs de l'application

doc/     => Contient toute la documentation du logiciel
    cahier_des_charges.txt => de début de projet
    html/         => dossier généré par Doxygen
    image.doxy    => utiliser pour générer le documentation Doxygen
    presentation/ => tous les fichiers liés au rendu
    recherche/    => recherches effectuées en début de projet
    sujet.pdf     => sujet du projet 

include/ => Contient les en-têtes

obj/     => Contient les fichiers objets

src/     => Contient les codes sources

exemple_mail_imap.txt   => mail récupéré via IMAP

MakefileTXT => Makefile utilisé lors du développement en CLI

qt.pro  => configuration du projet Qt

README.md => ici même

############################# REMARQUES #############################

Il se peut que le déchiffrement ne fonctionne pas comme prévu même si
les chances que cela arrive sont faibles. Si c'est le cas, supprimer
le dossier clients et pkg du répertoire data.
