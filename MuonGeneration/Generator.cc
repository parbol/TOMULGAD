//--------------------------------------------------------------------------------------//
// _________   ___   ____    ____ _____  _____ _____      ______       _      ______    //
//|  _   _  |.'   `.|_   \  /   _|_   _||_   _|_   _|   .' ___  |     / \    |_   _ `.  //
//|_/ | | \_/  .-.  \ |   \/   |   | |    | |   | |    / .'   \_|    / _ \     | | `. \ //
//    | |   | |   | | | |\  /| |   | '    ' |   | |   _| |   ____   / ___ \    | |  | | //
//   _| |_  \  `-'  /_| |_\/_| |_   \ \__/ /   _| |__/ \ `.___]  |_/ /   \ \_ _| |_.' / //
//  |_____|  `.___.'|_____||_____|   `.__.'   |________|`._____.'|____| |____|______.'  //
//                                                                                      //
//--------------------------------------------------------------------------------------//
// A software project by: Pablo Martinez Ruiz del Árbol                                 //
//--------------------------------------------------------------------------------------//


#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "ConfigurationGeometry.hh"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/kd.h>


//--------------------------------------------------------------------------------------//
// Methods defined in this file                                                         //
//--------------------------------------------------------------------------------------//

//Get options from the command line
bool getOptions(int , char **, G4String &, G4String &, G4String &, G4int &, G4long &, G4double &, G4String &);

//Simply shows a cool propaganda banner
void showBanner();

//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//







//--------------------------------------------------------------------------------------//
// Start of the program                                                                 //
//--------------------------------------------------------------------------------------//
int main(int argc,char** argv) {

    showBanner();

    G4String nameOfInputFile;
    G4String nameOfOutputFile;
    G4String nameOfCRYFile;
    G4int    numberOfEvents;
    G4long   randomSeed = 0;
    G4double pt = 0;
    G4String visual="";
        
    if(!getOptions(argc, argv, nameOfInputFile, nameOfOutputFile, nameOfCRYFile, numberOfEvents, randomSeed, pt, visual)) {
        G4cerr << "\033[1;31m" << "Usage: ./Generator --input NameOfGeometry.json --cry cryfile.txt --output outputfile --number numberOfEvents --seed seed --pt pt --vis visualManager"  << "\033[0m" << G4endl;
        return -1;
    }

    //Number of events all right?
    if(numberOfEvents < 1) {
        G4cerr << "\033[1;31m" << "The number of events must be a positive integer greater than 0" << "\033[0m" << G4endl;
        return -1;
    }

    ConfigurationGeometry *geomConf = new ConfigurationGeometry(nameOfInputFile);
    if(!geomConf->isGood()) {
        G4cerr << "\033[1;31m" << "Problems in the configuration geometry file" << "\033[0m" << G4endl;
        return -1;
    }

    //Initializing runManager
    G4RunManager* runManager = new G4RunManager;

    runManager->SetVerboseLevel(4);   

    DetectorConstruction *myDetector = new DetectorConstruction(geomConf);
    if(myDetector == NULL) {
        G4cerr << "\033[1;31m" << "Problems in the construction of the detector" << "\033[0m" << G4endl;
        return -1;
    }

    runManager->SetUserInitialization(myDetector);

    PhysicsList *myPhysicsList = new PhysicsList();
    if(myPhysicsList == NULL) {
        G4cerr << "\033[1;31m" << "Problems in the physics list" << "\033[0m" << G4endl;
        return -1;
    }

    runManager->SetUserInitialization(myPhysicsList);

    PrimaryGeneratorAction *myPrimaryGeneratorAction = new PrimaryGeneratorAction(geomConf, nameOfCRYFile, randomSeed, pt);
    if(myPrimaryGeneratorAction == NULL) {
        G4cerr << "\033[1;31m" << "Problems in PrimaryGeneratorAction" << "\033[0m" << G4endl;
        return -1;
    }

    runManager->Initialize();

    runManager->SetUserAction(myPrimaryGeneratorAction);

    RunAction *myRunAction = new RunAction(nameOfOutputFile, geomConf);
    if(myRunAction == NULL) {
        G4cerr << "\033[1;31m" << "Problems in RunAction" << "\033[0m" << G4endl;
        return -1;
    }

    runManager->SetUserAction(myRunAction);

    EventAction *myEventAction = new EventAction(geomConf);
    myEventAction->SetNumberOfEvents(numberOfEvents);
    if(myEventAction == NULL) {
        G4cerr << "\033[1;31m" << "Problems in EventAction" << "\033[0m" << G4endl;
        return -1;
    }

    runManager->SetUserAction(myEventAction);


    #ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    //visManager->RegisterGraphicsSystem (new G4HepRep);
    #endif

    if(visual == "heprep"){

        G4UImanager* UI = G4UImanager::GetUIpointer();
        UI->ApplyCommand("/run/verbose 2");
        UI->ApplyCommand("/vis/scene/create A01Output.heprep");
        UI->ApplyCommand("/vis/open HepRepXML");
        UI->ApplyCommand("/vis/drawVolume world");
        UI->ApplyCommand("/vis/scene/add/volume");
        //UI->ApplyCommand("");
        //UI->ApplyCommand("");
        UI->ApplyCommand("/vis/viewer/flush");
        //UI->ApplyCommand("/vis/verbose");
        //UI->ApplyCommand("/vis/heprep/setFileName archivo.hep");
    
        //   runManager->BeamOn(numberOfEvents);
    
    } else if(visual == "opengl") {
        G4UImanager* UI = G4UImanager::GetUIpointer();
        G4UIExecutive* ui_e = new G4UIExecutive(argc, argv);
    
        UI->ApplyCommand("/run/verbose 2");
        UI->ApplyCommand("/vis/scene/create A01Output.heprep");


        UI->ApplyCommand("/vis/open OGL 600x600-0+0");

        UI->ApplyCommand("/vis/drawVolume world");

        UI->ApplyCommand("/vis/scene/add/volume");
        UI->ApplyCommand("/vis/scene/add/trajectories");
        UI->ApplyCommand("/vis/scene/endOfEventAction accumulate");
        //UI->ApplyCommand("");
        UI->ApplyCommand("/vis/viewer/flush");
        UI->ApplyCommand("/vis/viewer/set/style surface");

        
        ui_e->SessionStart();
        delete ui_e;
    }

    
    runManager->BeamOn(numberOfEvents);


    #ifdef G4VIS_USE
    delete visManager;
    #endif

    delete runManager;
    delete geomConf;
    G4cout << "The program finished successfully" << std::endl;
    return 0;

}
//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
// This method will put the command line input in the variables                         //
//--------------------------------------------------------------------------------------//
bool getOptions(int argc, char **argv, G4String &nameOfInputFile, G4String &nameOfOutputFile, G4String &nameOfCRYFile, G4int &numberOfEvents, G4long &randomSeed, G4double &pt, G4String &visual) {

    int option_iterator;
    int option_counter = 0;
    bool moreoptions = true;

    while (moreoptions) {
        static struct option long_options[] = {
            /* These options set a flag. */
            {"input",     required_argument, 0, 'i'},
            {"output",    required_argument, 0, 'o'},
            {"seed",    required_argument, 0, 's'},
            {"number",    required_argument, 0, 'n'},
            {"cry",    required_argument, 0, 'c'},
            {"pt",    required_argument, 0, 'p'},
            {"vis",    required_argument, 0, 'v'},
            {0, 0, 0, 0}
        };
        int option_index = 0;
        option_iterator = getopt_long(argc, argv, "d:", long_options, &option_index);
        if (option_iterator == -1) {
            moreoptions = false;
        } else {
            option_counter++;
            switch (option_iterator) {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;
                if (optarg)
                    break;
            case 'i':
                nameOfInputFile = (G4String) optarg;
                break;
            case 'o':
                nameOfOutputFile = (G4String) optarg;
                break;
            case 'n':
                numberOfEvents = (G4int) atoi(optarg);
                break;
            case 'c':
                nameOfCRYFile = (G4String) atoi(optarg);
                break;
            case 's':
                randomSeed = (G4long) atoi(optarg);
                break;
            case 'p':
                pt = (G4double) atof(optarg);
                break;
            case 'v':
                visual = (G4String) optarg;
                break;
            case '?':
                return false;
                break;
            default:
                return false;
            }
        }
    }

    if (option_counter == 0) {
        return false;
    }
    return true;

}
//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//



//--------------------------------------------------------------------------------------//
// Just showing the banner                                                              //
//--------------------------------------------------------------------------------------//
void showBanner() {

    G4cout << "\033[1;34m "
           << " _________   ___   ____    ____ _____  _____ _____      ______       _      ______    "        <<  G4endl
           << "|  _   _  |.'   `.|_   \\  /   _|_   _||_   _|_   _|   .' ___  |     / \\    |_   _ `.  "      <<  G4endl
           << "|_/ | | \\_/  .-.  \\ |   \\/   |   | |    | |   | |    / .'   \\_|    / _ \\     | | `. \\ "  <<  G4endl
           << "    | |   | |   | | | |\\  /| |   | '    ' |   | |   _| |   ____   / ___ \\    | |  | | "      <<  G4endl
           << "   _| |_  \\  `-'  /_| |_\\/_| |_   \\ \\__/ /   _| |__/ \\ `.___]  |_/ /   \\ \\_ _| |_.' / " <<  G4endl
           << "  |_____|  `.___.'|_____||_____|   `.__.'   |________|`._____.'|____| |____|______.'  "        <<  G4endl
           << "                                                                                      "        <<  G4endl
           << "\033[0m"                                                                                       << G4endl;

}
//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//

