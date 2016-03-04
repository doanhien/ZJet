#ifndef EnergyRegression_h_
#define EnergyRegression_h_

#include <TMath.h>
#include <TMVA/Reader.h>

#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooConstVar.h>
#include <RooWorkspace.h>

//#include <RooRevCBExp.h>
//#include <RooGausDoubleExp.h>
//#include <RooDoubleCBFast.h>
//#include <HybridGBRForest.h>
//#include <RooHybridBDTAutoPdf.h>


float EleSCEta, EleSCPhi, EleSigmaIEtaIEta;
float EleSigmaIPhiIPhi, EleSCEtaWidth, EleSCPhiWidth;
float EleHoverE;

static TMVA::Reader* tmvaReader[2] = {NULL, NULL};

//call this function once at the beginning of event loop
void initreader() {


  for (int iBE = 0; iBE < 2; iBE++) {

    tmvaReader[iBE] = new TMVA::Reader("!Color:Silent");

    //add classification variables, variable name have to be the same in weight files
    tmvaReader[iBE]->AddVariable("EleSCPhi", &EleSCPhi);
    tmvaReader[iBE]->AddVariable("EleSCEta", &EleSCEta);
    tmvaReader[iBE]->AddVariable("EleSigmaIEtaIEta", &EleSigmaIEtaIEta);
    tmvaReader[iBE]->AddVariable("EleSigmaIPhiIPhi", &EleSigmaIPhiIPhi);
    tmvaReader[iBE]->AddVariable("EleSCEtaWidth", &EleSCEtaWidth);
    tmvaReader[iBE]->AddVariable("EleSCPhiWidth", &EleSCPhiWidth);
    tmvaReader[iBE]->AddVariable("EleHoverE", &EleHoverE);

    //book method, read weight files
    if (iBE == 0)
      tmvaReader[0]->BookMVA("BDTG", "weights/BDTG_EB_PbPb.weights.xml");
    else
      tmvaReader[1]->BookMVA("BDTG", "weights/BDTG_EE_PbPb.weights.xml");
  }
}

//call this function for every electron
//multiply return value with electron energy or electron pt (eg. elePt*ElectronRegressionTMVA(i))
float ElectronRegressionTMVA(int i) {

  int iBE = (fabs(eleSCEta[i]) < 1.4442) ? 0 : 1;

  //set variable
  EleSCPhi = eleSCPhi[i];
  EleSCEta = eleSCEta[i];
  EleSigmaIEtaIEta = eleSigmaIEtaIEta_2012[i];
  EleSigmaIPhiIPhi = eleSigmaIPhiIPhi[i];
  EleSCEtaWidth = eleSCEtaWidth[i];
  EleSCPhiWidth = eleSCPhiWidth[i];
  EleHoverE = eleHoverE[i];

  //get regression
  return (tmvaReader[iBE]->EvaluateRegression("BDTG"))[0];

}

//for GBR

RooRealVar* invar[2][99]; // [EB or EE][varnum]
RooAbsReal* mvaMean[2];
RooAbsReal* mvaSigma[2];
RooAbsReal* mvaAlphaL[2];
RooAbsReal* mvaAlphaR[2];
RooAbsReal* mvaPowerL[2];
RooAbsReal* mvaPowerR[2];

void initRegressionGBR(){

  TString weightfile = "weights/gbrmva_PbPb.root";

  TFile f(weightfile);
  if (f.IsZombie()) FATAL("TFile::Open() failed");

  for (int iBE = 0; iBE < 2; iBE++) { // barrel vs endcaps
    const char* det = (iBE == 0 ? "EB" : "EE");

    TString wsname = TString::Format("ws_mva_%s", det);

    RooWorkspace* ws = dynamic_cast<RooWorkspace*>(f.Get(wsname));
    if (!ws) FATAL("TFile::Get() failed");

    invar[iBE][1] = ws->var("var2");
    invar[iBE][2] = ws->var("var3");
    invar[iBE][3] = ws->var("var4");
    invar[iBE][4] = ws->var("var5");
    invar[iBE][5] = ws->var("var6");
    invar[iBE][6] = ws->var("var7");
    invar[iBE][7] = ws->var("var8");

    mvaMean[iBE] = ws->function("limMean");
    mvaSigma[iBE] = ws->function("limSigma");
    mvaAlphaL[iBE] = ws->function("limAlphaL");
    mvaAlphaR[iBE] = ws->function("limAlphaR");
    mvaPowerL[iBE] = ws->function("limPowerL");
    mvaPowerR[iBE] = ws->function("limPowerR");

  }
}

float ElectronRegressionGBR(int i) {

  float mean;

  int iBE = (fabs(eleSCEta[i]) < 1.4442) ? 0 : 1;

  /**invar[iBE][1] = eleSCEta[i];
  *invar[iBE][2] = eleSigmaIEtaIEta[i];
  *invar[iBE][3] = eleSCPhi[i];
  *invar[iBE][4] = eleSigmaIPhiIPhi[i];
  *invar[iBE][5] = eleSCEtaWidth[i];
  *invar[iBE][6] = eleSCPhiWidth[i];
  *invar[iBE][7] = eleHoverE[i];

  mean = mvaMean[iBE]->getVal();*/

  return mean;
}



#endif
