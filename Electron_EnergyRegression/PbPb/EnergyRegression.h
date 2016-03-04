#ifndef EnergyRegression_h_
#define EnergyRegression_h_

#include <TMath.h>
#include <TMVA/Reader.h>

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


#endif
