#include <iostream>
#include "TSystem.h"
#include "PParticle.h"
#include "PChannel.h"
#include "PReaction.h"
#include "TMath.h"

using namespace std;

void macro5() {
    gSystem->Load("$PLUTOLIBDIR/libPluto.so");

    PParticle beamProton("p", 1.2);
    PParticle targetProton("p");
    PParticle composite = beamProton + targetProton;

    PParticle producedProton("p"), dPlus("D+");
    PParticle* productionParticles[] = {&composite, &producedProton, &dPlus};
    PChannel* productionChannel = new PChannel(productionParticles, 2);

    PParticle producedNeutron("n"), producedPionPlus("pi+");
    PParticle* decayParticles[] = {&dPlus, &producedNeutron, &producedPionPlus};
    PChannel* decayChannel = new PChannel(decayParticles, 2);

    PChannel* reactionChannels[] = {productionChannel, decayChannel};
    PReaction *reaction = new PReaction(reactionChannels, "macro5_out", 2, 1, 0, 0, 0);

    reaction->Do("th_p = [p]->Theta() * TMath::RadToDeg();");
    reaction->Do("th_pip = [pi+]->Theta() * TMath::RadToDeg();");

    reaction->Do("#accept = 1;");
    reaction->Do("if (th_p < 20. || th_p > 70.) #accept = 0;");
    reaction->Do("if (th_pip < 20. || th_pip > 70.) #accept = 0;");

    reaction->Print();
    reaction->Loop(100000, 1);

    cout << "Simulation finished. File macro5_out.root was created." << endl;
}
