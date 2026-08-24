#include <iostream>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TMath.h"
#include "PParticle.h"

using namespace std;

void formatHistogram(TH1D* histogram, const char* xAxisTitle) {
    histogram->GetXaxis()->SetTitle(xAxisTitle);
    histogram->GetXaxis()->CenterTitle(true);
    histogram->GetXaxis()->SetTitleOffset(1.2);

    histogram->GetYaxis()->SetTitle("Counts");
    histogram->GetYaxis()->CenterTitle(true);
    histogram->GetYaxis()->SetTitleOffset(1.6);

    histogram->SetLineColor(kBlack);
    histogram->SetLineWidth(2);
    histogram->SetFillColorAlpha(kBlue-7, 0.3);
}

void macro6() {
    gSystem->Load("$PLUTOLIBDIR/libPluto.so");

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetLabelSize(0.05, "XY");
    gStyle->SetTitleSize(0.05, "XY");
    gStyle->SetTextFont(42);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.16);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.08);

    TFile *inputFile = new TFile("macro5_out.root");
    if (!inputFile || inputFile->IsZombie()) return;

    TTree *dataTree = (TTree*)inputFile->Get("data");
    TClonesArray *particles = new TClonesArray("PParticle", 10);
    dataTree->SetBranchAddress("Particles", &particles);

    TH1D *protonThetaHistogram = new TH1D("protonThetaHistogram", "", 90, 0, 90);
    TH1D *pionThetaHistogram = new TH1D("pionThetaHistogram", "", 90, 0, 90);

    formatHistogram(protonThetaHistogram, "Proton emission angle #theta_{p} [deg]");
    formatHistogram(pionThetaHistogram, "Pion emission angle #theta_{#pi^{+}} [deg]");

    int eventCount = dataTree->GetEntries();
    for (int eventIndex = 0; eventIndex < eventCount; eventIndex++) {
        dataTree->GetEntry(eventIndex);
        for (int particleIndex = 0; particleIndex < particles->GetEntries(); particleIndex++) {
            PParticle *particle = (PParticle*)particles->At(particleIndex);

            if (particle->ID() == 14 && particle->GetParentId() != 0) {
                protonThetaHistogram->Fill(particle->Theta() * TMath::RadToDeg());
            }
            if (particle->ID() == 8) {
                pionThetaHistogram->Fill(particle->Theta() * TMath::RadToDeg());
            }
        }
    }

    TCanvas *canvas = new TCanvas("canvas", "Angular distributions", 1200, 600);
    canvas->Divide(2, 1);

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.06);
    latex.SetTextFont(42);

    canvas->cd(1);
    protonThetaHistogram->Draw("HIST");
    latex.DrawLatex(0.2, 0.93, "Protons");

    canvas->cd(2);
    pionThetaHistogram->Draw("HIST");
    latex.DrawLatex(0.2, 0.93, "Pions #pi^{+}");
}
