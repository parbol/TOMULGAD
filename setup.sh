#!/bin/bash

if [ $HOSTNAME == "caronte" ]; then

    echo "Setting up environment in Caronte"
    export G4INSTALLDIR=/home/pablo/Documentos/TOMULGAD/geant4-v11.1.2-install
    export G4WORKDIR=/home/pablo/Documentos/TOMULGAD/TOMULGAD/
    export JSONCPPDIR=/home/pablo/Documentos/TOMULGAD/jsoncpp/
    source $G4INSTALLDIR/bin/geant4.sh
fi

if [ $HOSTNAME == "euler" ]; then

    echo "Setting up environment in Euler"
    export G4INSTALLDIR=/home/pablo/Documentos/Pyramid/geant4-v11.2.1-install
    export G4WORKDIR=/home/pablo/Documentos/Pyramid/TOMULGAD
    export JSONCPPDIR=/home/pablo/jsoncpp/
    source $G4INSTALLDIR/bin/geant4.sh
fi

if [ $HOSTNAME == "galois" ]; then

    echo "Setting up environment in Galois"
    export G4INSTALLDIR=/home/pablo/Documentos/Pyramid/geant4-v11.2.1-install
    export G4WORKDIR=/home/pablo/Documentos/Pyramid/TOMULGAD/
    export JSONCPPDIR=/home/pablo/jsoncpp/
    source $G4INSTALLDIR/bin/geant4.sh
fi

