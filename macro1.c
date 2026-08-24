#include <iostream>
#include "TSystem.h"
#include "PReaction.h"

using namespace std;

void macro1() {
    gSystem->Load("$PLUTOLIBDIR/libPluto.so");
    PReaction *my_reaction = new PReaction(
        "1.2", "p", "p", 
        "p D+ [p pi0 [g g]]", 
        "macro1_out", 
        1, 0, 0, 0
    );

    my_reaction->Print();
    my_reaction->Loop(10000, 1);
    
    cout << "Simulation finished. File macro1_out.root was created" << endl;
}