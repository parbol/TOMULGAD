# TOMULGAD


-------------------------------------------------------------
1.- Install GEANT4 from source and compile
-------------------------------------------------------------

1.1 Install pre-requisites

+ Make sure to have at least GCC11 or greater.

+ CMake 3.16 or higher.

In any case, check: 

https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/gettingstarted.html


1.2 Get the GEANT4 source code 

Get the latest release from the GEANT4 page. We recommend 4.11.1.2.

wget https://gitlab.cern.ch/geant4/geant4/-/archive/v11.1.2/geant4-v11.1.2.tar.gz


1.3 Setup installation and compile

cd $HOME

wget https://gitlab.cern.ch/geant4/geant4/-/archive/v11.1.2/geant4-v11.1.2.tar.gz

gunzip geant4-v11.1.2.tar.gz

tar -xvf geant4-v11.1.2.tar

rm geant4-v11.1.2.tar

mkdir geant4-v11.1.2-build

cd geant4-v11.1.2-build

cmake -DCMAKE_INSTALL_PREFIX=../geant4-v11.1.2-install -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_SYSTEM_EXPAT=OFF ../geant4-v11.1.2

make -j8

make install


-------------------------------------------------------------
2.- Install ROOT from source and compile
-------------------------------------------------------------

2.1 Install pre-requisites

In general cmake will ask for dependencies, but some of the most normal missing dependencies can be installed with:

sudo apt-get install libxpm-dev libxpm4 libxft-dev libxft2 libxext-dev libxext6


2.2 Get the ROOT source code

wget https://root.cern/download/root_v6.28.04.source.tar.gz


2.3 Setup installation and compile 

cd $HOME

wget https://root.cern/download/root_v6.28.04.source.tar.gz

gunzip root_v6.28.04.source.tar.gz

tar -xvf root_v6.28.04.source.tar

rm root_v6.28.04.source.tar

mkdir root_v6.28.04-build root_v6.28.04-install

cd root_v6.28.04-build

cmake -DCMAKE_INSTALL_PREFIX=../root_v6.28.04-install ../root-6.28.04

cmake --build . --target install

echo "source $HOME/root_v6.28.04-install/bin/thisroot.sh" >> $HOME/.bashrc


-------------------------------------------------------------
3.- Install JSON library
-------------------------------------------------------------

You can do this by just copy-pasting these lines:

#Make sure you are outside of MTP release area
cd $HOME/

git clone https://github.com/open-source-parsers/jsoncpp jsoncpp

mkdir jsoncpp-build

cd jsoncpp-build

cmake -DJSONCPP_WITH_PKGCONFIG_SUPPORT=OFF -DJSONCPP_WITH_TESTS=OFF ../jsoncpp/

More instructions can be found at:

http://tudat.tudelft.nl/projects/tudat/wiki/JSONCPP

You also need to have installed geant4 and ROOT.


-------------------------------------------------------------
4.- Install TOMULGAD
-------------------------------------------------------------

TOMULGAD can be obtained from github:

git clone https://github.com/parbol/TOMULGAD.git

Take into account that TOMULGAD comes with a version of the CRY generator that has to be compiled
indepedently from the GEANT4 TOMULGAD program.

4.1 Compile CRY libraries


4.2 Compile TOMULGAD





