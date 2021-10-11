#include <TChain.h>
#include <TString.h>
#include <TClonesArray.h>
#include <TH1.h>
#include <Rtypes.h>
R__ADD_INCLUDE_PATH($VMCWORKDIR)
//#include "macro/run/bmnloadlibs.C"
#include "bmnloadlibs.C"
        
void readFD(TString fileName = "bmnsim.root") {
    if (fileName == "") {
        cout << "File not specified!" << endl;
        return;
    }

    TChain* out = new TChain("bmndata");
    out->Add(fileName.Data());
    cout << "#recorded entries = " << out->GetEntries() << endl;

    //Creating histograms
    TH1I *histAmp = new TH1I("histAmp", "Amplitude distribution;Amplitude;Number", 100, 0, 0.01);
    TH1I *histAmpOld = new TH1I("histAmpOld", "Amplitude distribution;Amplitude;Number", 100, 0, 0.01);
    TH1I *histAmpDif = new TH1I("histAmpDif", "Amplitude distribution;Amplitude;Number", 100, 0, 0.01);
    TH1I *histMod = new TH1I("histMod", "Module distribution;Module;Number", 67, -2, 65);
    TH1I *histModOld = new TH1I("histModOld", "Module distribution;Module;Number", 67, -2, 65);
    TH1I *histModDif = new TH1I("histModDif", "Module distribution;Module;Number", 67, -2, 65);
    auto hStackMod = new THStack("hStackMod","Old and filtered digit modules");
    auto hStackAmp = new THStack("hStackAmp","Old and filtered digit amplitudes");
    TFile f("histo.root", "recreate");

    TClonesArray* fdDigits = nullptr;
    out->SetBranchAddress("FDDigit", &fdDigits);

    Int_t nEntries = 0;
    Int_t nEntriesOld = 0;

    histMod->SetLineColor(1);
    histAmp->SetLineColor(1);
    histModOld->SetLineColor(2);
    histAmpOld->SetLineColor(2);
    histModDif->SetLineColor(2);
    histAmpDif->SetLineColor(2);

    //for (Int_t iEv = 0; iEv < 150; iEv++) {
    for (Int_t iEv = 0; iEv < out->GetEntries(); iEv++) {
        out->GetEntry(iEv);
        
        for (Int_t iDigit = 0; iDigit < fdDigits->GetEntriesFast(); iDigit++) {
            BmnTrigDigit* digi = (BmnTrigDigit*) fdDigits->UncheckedAt(iDigit);
                        
            nEntriesOld++;
            histModOld->Fill(digi->GetMod());
            histAmpOld->Fill(digi->GetAmp());

            if( fdDigits->GetEntriesFast() > 2 ) {
                nEntries++;
                histMod->Fill(digi->GetMod());
                histAmp->Fill(digi->GetAmp());
            }
        
        }
    }
    /*
    //fffffffffffffffff
    Double_t vals[64];
    Double_t sims[64];
    Int_t colors[64];
    char lbls[64];
    for(Int_t i = 0; i < 64; i++) {
        vals[i] = histMod->GetBinContent(i);
        sims[i] = 1;
        lbls[i] = i;
        colors[i] = vals[i];
    }
    const char *a = lbls;
    TCanvas *cpie = new TCanvas("cpie","TPie test",700,700);
    TPie *pie = new TPie("pieChart", "Pie Chart for histogram", 64, sims);
    cpie->cd(1);
    pie->SetAngularOffset(89.6);
    pie->SetRadius(.35);
    pie->SetFillColors(colors);
    for(Int_t i = 0; i < 64; i++) {
        pie->GetSlice(i)->SetTitle("a");
    }
    pie->Draw();

    //fffffffffffffffff
    */

    histModDif->Add(histMod,histModOld, -1, 1);
    histAmpDif->Add(histAmp,histAmpOld, -1, 1);

    hStackAmp->Add(histAmp);
    hStackMod->Add(histMod);
    hStackMod->Add(histModDif);
    hStackAmp->Add(histAmpDif);

    hStackMod->Draw();
    hStackMod->GetXaxis()->SetTitle("Module");
    hStackMod->GetYaxis()->SetTitle("Number");

    hStackAmp->Draw();
    hStackAmp->GetXaxis()->SetTitle("Amplitude");
    hStackAmp->GetYaxis()->SetTitle("Number");
    
    histMod->SetEntries(nEntries); 
    histAmp->SetEntries(nEntries);
    histMod->Write();
    histAmp->Write();

    histModOld->Write();
    histModDif->Write();

    hStackMod->Write();
    hStackAmp->Write();

    //pie->Draw();

}
