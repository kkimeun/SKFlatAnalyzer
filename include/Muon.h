#ifndef Muon_h
#define Muon_h

#include "Lepton.h"
#include "TString.h"

class Muon: public Lepton{

public:

  Muon();
  ~Muon();

  inline bool isPOGTight() const {return j_isPOGTight;}
  inline bool isPOGMedium() const {return j_isPOGMedium;}
  inline bool isPOGLoose() const {return j_isPOGLoose;}
  void SetisPOGTight(bool b){ j_isPOGTight = b; }
  void SetisPOGMedium(bool b){ j_isPOGMedium = b; }
  void SetisPOGLoose(bool b){ j_isPOGLoose = b; }

  void SetIso(double ch04, double nh04, double ph04, double pu04);
  void CalcPFRelIso();
  double EA();

  void SetMiniAODPt(double d);
  double MiniAODPt() const {return j_MiniAODPt;}

  void SetMomentumUpDown(double up, double down);
  double MomentumUp() const {return j_MomentumUp;}
  double MomentumDown() const {return j_MomentumDown;}

  //==== ID
  bool PassID(TString ID);
  bool Pass_POGTight();
  bool Pass_POGTightWithTightIso();
  bool Pass_SUSYTight();
  bool Pass_TESTID();

private:

  bool j_isPOGTight, j_isPOGMedium, j_isPOGLoose;
  double j_PFCH04, j_PFNH04, j_PFPH04, j_PU04;
  double j_MiniAODPt, j_MomentumUp, j_MomentumDown;

};

Muon::Muon() : Lepton() {
  j_isPOGTight = false;
  j_PFCH04 = -999.;
  j_PFNH04 = -999.;
  j_PFPH04 = -999.;
  j_PU04 = -999.;
  this->SetLeptonFlavour(MUON);
  j_MiniAODPt = -999.;
  j_MomentumUp = -999.;
  j_MomentumDown = -999.;
}

Muon::~Muon(){
}

void Muon::SetIso(double ch04, double nh04, double ph04, double pu04){
  j_PFCH04 = ch04;
  j_PFNH04 = nh04;
  j_PFPH04 = ph04;
  j_PU04 = pu04;
  CalcPFRelIso();
}

void Muon::CalcPFRelIso(){
  double absiso = j_PFCH04+std::max( 0., j_PFNH04 + j_PFPH04 - 0.5*j_PU04 );
  //cout << "[Muon::CalcPFRelIso] j_PFCH04 = " << j_PFCH04 << endl;
  //cout << "[Muon::CalcPFRelIso] j_PFNH04 = " << j_PFNH04 << endl;
  //cout << "[Muon::CalcPFRelIso] j_PFPH04 = " << j_PFPH04 << endl;
  //cout << "[Muon::CalcPFRelIso] j_PU04 = " << j_PU04 << endl;
  //cout << "[Muon::CalcPFRelIso] --> absiso = " << absiso << endl;
  this->SetRelIso(absiso/this->Pt());
}

double Muon::EA(){

  double eta = fabs(this->Eta());

  if     (eta<0.8000) return 0.0566;
  else if(eta<1.3000) return 0.0562;
  else if(eta<2.0000) return 0.0363;
  else if(eta<2.2000) return 0.0119;
  else if(eta<2.4000) return 0.0064;
  else return 0.0064;

}

void Muon::SetMiniAODPt(double d){
  j_MiniAODPt = d;
}
void Muon::SetMomentumUpDown(double up, double down){
  j_MomentumUp = up;
  j_MomentumDown = down;
}

bool Muon::PassID(TString ID){
  if(ID=="POGTight") return isPOGTight();
  if(ID=="POGMedium") return isPOGMedium();
  if(ID=="POGLoose") return isPOGLoose();
  if(ID=="POGTightWithTightIso") return Pass_POGTightWithTightIso();
  if(ID=="SUSYTight") return Pass_SUSYTight();
  if(ID=="TEST") return Pass_TESTID();
  return false;
}
bool Muon::Pass_POGTightWithTightIso(){
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.15 ))  return false;
  return true;
}
bool Muon::Pass_SUSYTight(){
  if(! isPOGMedium() ) return false;
  if(! (MiniRelIso()<0.2) ) return false;
  if(! (fabs(dXY())<0.05 && fabs(dZ())<0.1 && fabs(IP3D()/IP3Derr())<8.) ) return false;
  return true;
}
bool Muon::Pass_TESTID(){
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.15 ))  return false;
  return true;
}

#endif
