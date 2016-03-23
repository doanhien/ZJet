#ifndef EnergyRegression_h_
#define EnergyRegression_h_

#include <TMath.h>
#include <TMVA/Reader.h>

#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooConstVar.h>
#include <RooWorkspace.h>


//use GBR method for pp
RooRealVar* invar[2][99]; // [EB or EE][varnum]
RooAbsReal* mvaMean[2];
RooAbsReal* mvaSigma[2];
RooAbsReal* mvaAlphaL[2];
RooAbsReal* mvaAlphaR[2];
RooAbsReal* mvaPowerL[2];
RooAbsReal* mvaPowerR[2];

//call this function once at the beginning of event loop 
void initRegressionGBR(){

  TString weightfile = "weights/gbrmva_pp.root";

  TFile f(weightfile);
  if (f.IsZombie()) FATAL("TFile::Open() failed");

  for (int iBE = 0; iBE < 2; iBE++) { // barrel vs endcaps
    const char* det = (iBE == 0 ? "EB" : "EE");

    TString wsname = TString::Format("ws_mva_%s", det);

    RooWorkspace* ws = dynamic_cast<RooWorkspace*>(f.Get(wsname));
    if (!ws) FATAL("TFile::Get() failed");

    invar[iBE][1] = ws->var("eleSCEta");
    invar[iBE][2] = ws->var("eleSCPhi");
    invar[iBE][3] = ws->var("eleSigmaIEtaIEta_2012");
    invar[iBE][4] = ws->var("eleSigmaIPhiIPhi");
    invar[iBE][5] = ws->var("eleSCEtaWidth");
    invar[iBE][6] = ws->var("eleSCPhiWidth");
    invar[iBE][7] = ws->var("eleHoverE");
    invar[iBE][8] = ws->var("eleR9");
    invar[iBE][9] = ws->var("eleE3x3");
    invar[iBE][10] = ws->var("eleE5x5");
    invar[iBE][11] = ws->var("eleR9Full5x5");
    invar[iBE][12] = ws->var("eleE3x3Full5x5");
    invar[iBE][13] = ws->var("eleE5x5Full5x5");
    invar[iBE][15] = ws->var("eleSeedEnToRawEn");
    invar[iBE][16] = ws->var("eleSeedEta");
    invar[iBE][17] = ws->var("eleSeedPhi");

    mvaMean[iBE] = ws->function("limMean");
    mvaSigma[iBE] = ws->function("limSigma");
    mvaAlphaL[iBE] = ws->function("limAlphaL");
    mvaAlphaR[iBE] = ws->function("limAlphaR");
    mvaPowerL[iBE] = ws->function("limPowerL");
    mvaPowerR[iBE] = ws->function("limPowerR");

  }
}

//call this function for every electron 
//multiply return value with electron energy or electron pt (eg. elePt*ElectronRegressionGBR(i))
float ElectronRegressionGBR(int i) {

  float mean;

  int iBE = (fabs(eleSCEta[i]) < 1.4442) ? 0 : 1;

  *invar[iBE][1] = eleSCEta[i];
  *invar[iBE][2] = eleSigmaIEtaIEta_2012[i];
  *invar[iBE][3] = eleSCPhi[i];
  *invar[iBE][4] = eleSigmaIPhiIPhi[i];
  *invar[iBE][5] = eleSCEtaWidth[i];
  *invar[iBE][6] = eleSCPhiWidth[i];
  *invar[iBE][7] = eleHoverE[i];
  *invar[iBE][8] = eleR9[i];
  *invar[iBE][9] = eleE3x3[i];
  *invar[iBE][10] = eleE5x5[i];
  *invar[iBE][11] = eleR9Full5x5[i];
  *invar[iBE][12] = eleE3x3Full5x5[i];
  *invar[iBE][13] = eleE5x5Full5x5[i];
  *invar[iBE][15] = eleSeedEn[i]/eleSCRawEn[i];
  *invar[iBE][16] = eleSeedEta[i];
  *invar[iBE][17] = eleSeedPhi[i];

  mean = mvaMean[iBE]->getVal();

  return mean;
}


#endif
