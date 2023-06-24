#!/bin/bash
# Petit script qui lance le build des fichiers
# Si param build - le fichier reconstruit les makfiles (appel à CMAKE)
# Sinon relance les makefiles précédemment créé
# Un travail d'artiste qui réjouria les assistants d'INFO-F-201
# ~ @

# if [[ $1 == "clean" ]]; then
#     echo "Makefiles cleaned"
#     rm Makefile*
# elif [[ ! -e "Makefile_serv" && ! -e "Makefile_serv" ]]; then
    rm CMakeCache.txt 

    echo "Build client"
    cmake . -DCMAKE_BUILD_TYPE=RELEASE
    # cp Makefile Makefile_client
    make -j8
    

    [ ! $? -eq "0" ] && echo "Erreur de compilation" && exit 1
    rm CMakeCache.txt

    echo "Build server"
    cmake . -DSERVER=1 -DCMAKE_BUILD_TYPE=RELEASE
    # cp Makefile Makefile_serv
    make -j8


    [ ! $? -eq "0" ] && echo "Erreur de compilation" && exit 1

    echo "Fin"
# else
#     echo "Lancement des Makefiles"
#     echo "Build du serveur"
#     make -j4 --file Makefile_serv
#     echo "Build du client"
#     make -j4 --file Makefile_client
# fi