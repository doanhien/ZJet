#ifndef EnergyRegression_h_
#define EnergyRegression_h_

#include <TMath.h>
#include <TMVA/Reader.h>

#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooConstVar.h>
#include <RooWorkspace.h>


float EleSCEta_, EleSCPhi_, EleSigmaIEtaIEta_2012_;
float EleSigmaIPhiIPhi_, EleSCEtaWidth_, EleSCPhiWidth_;
float EleHoverE_, EleR9_, EleE3x3_, EleE5x5_;
float EleR9Full5x5_, EleE3x3Full5x5_, EleE5x5Full5x5_;
float NClusters_, EleSeedEta_, EleSeedPhi_;
float EleSeedCryEta_, EleSeedCryPhi_, EleSeedCryIeta_, EleSeedCryIphi_;
float EleSeedEnToRawEn_, EleESEnToRawEn_;

static TMVA::Reader* tmvaReader[2] = {NULL, NULL};

//call this function once at the beginning of event loop
void initreader() {


  for (int iBE = 0; iBE < 2; iBE++) {

    tmvaReader[iBE] = new TMVA::Reader("!Color:Silent");

    //add classification variables, variable name have to be the same in weight files
    tmvaReader[iBE]->AddVariable("eleSCPhi", &EleSCPhi_);
    tmvaReader[iBE]->AddVariable("eleSCEta", &EleSCEta_);
    tmvaReader[iBE]->AddVariable("eleSigmaIEtaIEta_2012", &EleSigmaIEtaIEta_2012_);
    tmvaReader[iBE]->AddVariable("eleSigmaIPhiIPhi", &EleSigmaIPhiIPhi_);
    tmvaReader[iBE]->AddVariable("eleSCEtaWidth", &EleSCEtaWidth_);
    tmvaReader[iBE]->AddVariable("eleSCPhiWidth", &EleSCPhiWidth_);
    tmvaReader[iBE]->AddVariable("eleHoverE", &EleHoverE_);
    tmvaReader[iBE]->AddVariable("eleR9", &EleR9_);
    tmvaReader[iBE]->AddVariable("eleE3x3", &EleE3x3_);
    tmvaReader[iBE]->AddVariable("eleE5x5", &EleE5x5_);
    tmvaReader[iBE]->AddVariable("eleR9Full5x5", &EleR9Full5x5_);
    tmvaReader[iBE]->AddVariable("eleE3x3Full5x5", &EleE3x3Full5x5_);
    tmvaReader[iBE]->AddVariable("eleE5x5Full5x5", &EleE5x5Full5x5_);
    tmvaReader[iBE]->AddVariable("eleSeedEn/eleSCRawEn", &EleSeedEnToRawEn_);
    tmvaReader[iBE]->AddVariable("eleSeedEta",  &EleSeedEta_);
    tmvaReader[iBE]->AddVariable("eleSeedPhi",  &EleSeedPhi_);

    //book method, read weight files
    if (iBE == 0)
      tmvaReader[0]->BookMVA("BDTG", "weights/BDTG_EB_PbPb.weights.xml");
    else
      tmvaReader[1]->BookMVA("BDTG", "weights/BDTG_EE_PbPb.weights.xml");
  }
}

//call this function for every electron
//multiply return value with electron energy or electron pt (eg. elePt*electronRegressionTMVA(i))
float ElectronRegressionTMVA(int i) {

  int iBE = (fabs(eleSCEta[i]) < 1.4442) ? 0 : 1;

  //set variable
  EleSCPhi_ = eleSCPhi[i];
  EleSCEta_ = eleSCEta[i];
  EleSigmaIEtaIEta_2012_ = eleSigmaIEtaIEta_2012[i];
  EleSigmaIPhiIPhi_ = eleSigmaIPhiIPhi[i];
  EleSCEtaWidth_ = eleSCEtaWidth[i];
  EleSCPhiWidth_ = eleSCPhiWidth[i];
  EleHoverE_ = eleHoverE[i];
  EleR9_ = eleR9[i];
  EleE3x3_ = eleE3x3[i];
  EleE5x5_ = eleE5x5[i];
  EleR9Full5x5_ = eleR9Full5x5[i];
  EleE3x3Full5x5_ = eleE3x3Full5x5[i];
  EleE5x5Full5x5_ = eleE5x5Full5x5[i];
  EleSeedEnToRawEn_ = eleSeedEn[i]/eleSCRawEn[i];
  EleSeedEta_ = eleSeedEta[i];
  EleSeedPhi_ = eleSeedPhi[i];

  //get regression
  return (tmvaReader[iBE]->EvaluateRegression("BDTG"))[0];

}

#endif
