#include <iostream>
#include "TSystem.h"
#include "PParticle.h"
#include "PDecayManager.h"
#include "PDecayChannel.h"

using namespace std;

void macro3() {
    gSystem->Load("$PLUTOLIBDIR/libPluto.so");

    PParticle beamProton("p", 1.2);
    PParticle targetProton("p");
    PParticle composite = beamProton + targetProton;

    PDecayManager* decayManager = new PDecayManager();
    decayManager->SetVerbose(1);

    PDecayChannel* decayChannel = new PDecayChannel();
    decayChannel->AddChannel(0.245, "p", "D+");
    decayChannel->AddChannel(0.735, "n", "D++");
    decayChannel->AddChannel(0.020, "p", "p", "pi0");

    decayManager->InitReaction(&composite, decayChannel);
    decayManager->PrintReactionList();

    decayManager->loop(10000, 0, "macro3_out", 1, 0, 0, 0, 0);

    cout << "Simulation finished. File macro3_out.root was created." << endl;
}
