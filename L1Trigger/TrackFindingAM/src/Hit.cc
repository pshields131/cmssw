#include "../interface/Hit.h"

Hit::Hit(char l, char lad, char zp, char seg, float strip, int idx, int tp, float pt, float ip, float eta, float phi0, float p_x, float p_y, float p_z, float p_x0, float p_y0, float p_z0, float p_b){
  layer = l;
  ladder = lad;
  zPos = zp;
  segment = seg;
  stripNumber=strip;
  stub_idx=idx;
  part_id = tp;
  part_pt = pt;
  part_ip = ip;
  part_eta = eta;
  part_phi0 = phi0;
  x = p_x;
  y = p_y;
  z = p_z;
  X0 = p_x0;
  Y0 = p_y0;
  Z0 = p_z0;
  bend = p_b;
}

Hit::Hit(const Hit& h){
  layer = h.layer;
  ladder = h.ladder;
  zPos = h.zPos;
  segment = h.segment;
  stripNumber=h.stripNumber;
  stub_idx=h.stub_idx;
  part_id=h.part_id;
  part_pt=h.part_pt;
  part_ip=h.part_ip;
  part_eta=h.part_eta;
  part_phi0=h.part_phi0;
  x=h.x;
  y=h.y;  
  z=h.z;
  X0=h.X0;
  Y0=h.Y0;  
  Z0=h.Z0;
  bend=h.bend;
}

char Hit::getLayer() const{
  return layer;
}

char Hit::getLadder() const{
  return ladder;
}

char Hit::getModule() const{
  return zPos;
}

char Hit::getSegment() const{
  return segment;
}

short Hit::getStripNumber() const{
  return (short)stripNumber;
}

float Hit::getHDStripNumber() const{
  return stripNumber;
}

int Hit::getID() const{
  return stub_idx;
}

int Hit::getParticuleID() const{
  return part_id;
}

float Hit::getParticulePT() const{
  return part_pt;
}

float Hit::getParticuleIP() const{
  return part_ip;
}

float Hit::getParticuleETA() const{
  return part_eta;
}

float Hit::getParticulePHI0() const{
  return part_phi0;
}

float Hit::getX() const{
  return x;
}

float Hit::getY() const{
  return y;
}

float Hit::getZ() const{
  return z;
}

float Hit::getX0() const{
  return X0;
}

float Hit::getY0() const{
  return Y0;
}

float Hit::getZ0() const{
  return Z0;
}

float Hit::getBend() const{
  return bend;
}

float Hit::getPolarPhi() const{
  float val = atan(y/x);
  if(x<0){
    if(y<0){
      val-=M_PI;
    }
    else{
      val+=M_PI;
    }
  }
  return val;
}

float Hit::getPolarDistance() const{
  return sqrt(x*x+y*y);
}

ostream& operator<<(ostream& out, const Hit& h){
  double d0=(h.getY0()-(tan(h.getParticulePHI0())*h.getX0()))*cos(h.getParticulePHI0());

  out<<"Layer "<<(int)h.getLayer()<<", ladder "<<(int)h.getLadder()<<", module "<<(int)h.getModule()<<", segment "<<(int)h.getSegment()<<", strip "<<h.getStripNumber()<<" (tp "<<h.getParticuleID()<<" - PT : "<<h.getParticulePT()<<" GeV - ip : "<<h.getParticuleIP()<<" - PHI0 : "<<h.getParticulePHI0()<<" - ETA0 : "<<h.getParticuleETA()<<" - D0 : "<<d0<<" - X : "<<h.getX()<<" - Y : "<<h.getY()<<" - Z : "<<h.getZ()<<" - Bend : "<<h.getBend()<<" - Z0 : "<<h.getZ0()<<")";
  return out;
}
