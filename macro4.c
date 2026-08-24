#include <iostream>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "PParticle.h"

using namespace std;

void macro4() {
    gSystem->Load("$PLUTOLIBDIR/libPluto.so");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetLegendBorderSize(0);
    gStyle->SetLegendTextSize(0.045);
    gStyle->SetLabelSize(0.05, "XY");
    gStyle->SetTitleSize(0.05, "XY");
    gStyle->SetTextFont(42);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.16);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.08);
    gStyle->SetNdivisions(505, "XY");

    TFile *inputFile = new TFile("macro1_out.root");
    if (!inputFile || inputFile->IsZombie()) {
        cout << "File macro1_out.root not found" << endl;
        return;
    }

    TTree *dataTree = (TTree*)inputFile->Get("data");
    TClonesArray *particles = new TClonesArray("PParticle", 10);
    dataTree->SetBranchAddress("Particles", &particles);

    TCanvas *canvas = new TCanvas("canvas", "Pi0 analysis", 800, 600);

    TH1D *massHistogram = new TH1D("massHistogram", "", 100, 0.05, 0.25);

    massHistogram->GetXaxis()->SetTitle("Rest mass M_{#gamma#gamma} [GeV/c^{2}]");
    massHistogram->GetXaxis()->CenterTitle(true);
    massHistogram->GetXaxis()->SetTitleOffset(1.2);

    massHistogram->GetYaxis()->SetTitle("Counts");
    massHistogram->GetYaxis()->CenterTitle(true);
    massHistogram->GetYaxis()->SetTitleOffset(1.6);

    massHistogram->SetLineColor(kBlack);
    massHistogram->SetLineWidth(2);

    int eventCount = dataTree->GetEntries();
    for (int eventIndex = 0; eventIndex < eventCount; eventIndex++) {
        dataTree->GetEntry(eventIndex);
        PParticle *photon1 = nullptr, *photon2 = nullptr;

        for (int particleIndex = 0; particleIndex < particles->GetEntries(); particleIndex++) {
            PParticle *particle = (PParticle*)particles->At(particleIndex);
            if (particle->ID() == 1) {
                if (!photon1) photon1 = particle;
                else if (!photon2) photon2 = particle;
            }
        }

        if (photon1 && photon2) {
            PParticle reconstructedPi0 = *photon1 + *photon2;
            massHistogram->Fill(reconstructedPi0.M());
        }
    }

    massHistogram->Draw();

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.05);
    latex.SetTextFont(42);
    latex.SetTextAlign(22);
    latex.DrawLatex(0.5, 0.95, "Pi0 reconstruction #pi^{0} #rightarrow #gamma #gamma");

    canvas->SaveAs("pi0_reconstruction.png");
}
