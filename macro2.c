#include <iostream>
#include "TSystem.h"
#include "PParticle.h"
#include "PChannel.h"
#include "PReaction.h"

using namespace std;

void macro2() {
    gSystem->Load("$PLUTOLIBDIR/libPluto.so");

    PParticle beamProton("p", 1.2);
    PParticle targetProton("p");
    PParticle composite = beamProton + targetProton;

    PParticle producedProton("p");
    PParticle dPlus("D+");
    PParticle* productionParticles[] = {&composite, &producedProton, &dPlus};
    PChannel* productionChannel = new PChannel(productionParticles, 2);

    PParticle producedNeutron("n");
    PParticle producedPionPlus("pi+");
    PParticle* decayParticles[] = {&dPlus, &producedNeutron, &producedPionPlus};
    PChannel* decayChannel = new PChannel(decayParticles, 2);

    PChannel* reactionChannels[] = {productionChannel, decayChannel};
    PReaction* reaction = new PReaction(
        reactionChannels, "macro2_out", 2, 1, 0, 0, 0
    );

    reaction->Print();
    reaction->Loop(10000, 1);

    cout << "Simulation finished. File macro2_out.root was created." << endl;
}