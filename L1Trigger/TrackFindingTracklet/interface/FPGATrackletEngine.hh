//This class implementes the tracklet engine
#ifndef FPGATRACKLETENGINE_H
#define FPGATRACKLETENGINE_H

#include "FPGAProcessBase.hh"
#include "FPGATETable.hh"
#include "FPGATETableDisk.hh"
#include "FPGATETableOverlap.hh"
#include "TiltedGeometryInfo.hh"

using namespace std;

class FPGATrackletEngine:public FPGAProcessBase{

public:

  FPGATrackletEngine(string name, unsigned int iSector):
    FPGAProcessBase(name,iSector){
    double dphi=two_pi/NSector;
    phimin_=iSector*dphi;
    phimax_=phimin_+dphi;
    if (phimin_>0.5*two_pi) phimin_-=two_pi;
    if (phimax_>0.5*two_pi) phimax_-=two_pi;
    if (phimin_>phimax_)  phimin_-=two_pi;
    //cout << "phimin_ phimax_ "<<phimin_<<" "<<phimax_<<endl;
    assert(phimax_>phimin_);
    stubpairs_=0;
    innervmstubs_=0;
    outervmstubs_=0;
    table_=0;
    layer1_=0;
    layer2_=0;
    disk1_=0;
    disk2_=0;
    dct1_=0;
    dct2_=0;
    phi1_=0;
    phi2_=0;
    z1_=0;
    z2_=0;
    r1_=0;
    r2_=0;
    if (name[3]=='L') {
      layer1_=name[4]-'0';
      z1_=name[12]-'0';
    }
    if (name[3]=='D') {
      disk1_=name[4]-'0';
      r1_=name[12]-'0';
    }
    if (name[11]=='L') {
      layer2_=name[12]-'0';
      z2_=name[23]-'0';
    }
    if (name[11]=='D') {
      disk2_=name[12]-'0';
      r2_=name[23]-'0';
    }
    if (name[12]=='L') {
      layer2_=name[13]-'0';
      z2_=name[23]-'0';
    }
    if (name[12]=='D') {
      disk2_=name[13]-'0';
      r2_=name[23]-'0';
    }
    
    phi1_=name[10]-'0';
    phi2_=name[21]-'0';

    dct1_=name[6]-'0';
    dct2_=name[17]-'0';


  }

  void addOutput(FPGAMemoryBase* memory,string output){
    if (writetrace) {
      cout << "In "<<name_<<" adding output to "<<memory->getName()
	   << " to output "<<output<<endl;
    }
    if (output=="stubpairout") {
      FPGAStubPairs* tmp=dynamic_cast<FPGAStubPairs*>(memory);
      assert(tmp!=0);
      stubpairs_=tmp;
      return;
    }
    assert(0);
  }

  void addInput(FPGAMemoryBase* memory,string input){
    if (writetrace) {
      cout << "In "<<name_<<" adding input from "<<memory->getName()
	   << " to input "<<input<<endl;
    }
    if (input=="innervmstubin") {
      FPGAVMStubsTE* tmp=dynamic_cast<FPGAVMStubsTE*>(memory);
      assert(tmp!=0);
      innervmstubs_=tmp;
      return;
    }
    if (input=="outervmstubin") {
      FPGAVMStubsTE* tmp=dynamic_cast<FPGAVMStubsTE*>(memory);
      assert(tmp!=0);
      outervmstubs_=tmp;
      return;
    }
    cout << "Could not find input : "<<input<<endl;
    assert(0);
  }

  void execute() {

    
    if (!((doL1L2&&(layer1_==1)&&(layer2_==2))||
	  (doL3L4&&(layer1_==3)&&(layer2_==4))||
	  (doL5L6&&(layer1_==5)&&(layer2_==6))||
	  (doD1D2&&(disk1_==1)&&(disk2_==2))||
	  (doD3D4&&(disk1_==3)&&(disk2_==4))||
	  (doL1D1&&(disk1_==1)&&(layer2_==1))||
	  (doL2D1&&(disk1_==1)&&(layer2_==2)))) return;

    //cout << getName()<<" "<<layer1_<<" "<<layer2_<<" "<<disk1_<<" "<<disk2_<<endl;
    

    //if (disk1_==0||disk2_==0) return;

    //cout << getName()<<" disk1_ disk2_ "<<disk1_<<" "<<disk2_<<endl;


    //if (getName().substr(0,5)!="TE_L1") return;

   
    unsigned int countall=0;
    unsigned int countpass=0;

    bool print=false&&(getName()=="TE_L1PHIC7_L2PHIC5");

    if (print) {
      cout << "In FPGATrackletEngine::execute : "<<getName()
	   <<" "<<innervmstubs_->nStubs()
	   <<" "<<outervmstubs_->nStubs()
	   <<endl;
    }

    //cout << "layer/disk : "<<layer1_<<" "<<layer2_<<" "
    //	 <<disk1_<<" "<<disk2_<<endl;

    assert(innervmstubs_!=0);
    assert(outervmstubs_!=0);
    for(unsigned int i=0;i<innervmstubs_->nStubs();i++){
      std::pair<FPGAStub*,L1TStub*> innerstub=innervmstubs_->getStub(i);
      if (debug1) {
	cout << "In "<<getName()<<" have inner stub"<<endl;
      }

      static ofstream outd("trackletenginediff.txt");

      if ((layer1_==1 && layer2_==2)||
	  (layer1_==3 && layer2_==4)||
	  (layer1_==5 && layer2_==6)) {


	if (debug1) cout << "Layer-layer pair" <<endl;
	
	int bin=innervmstubs_->getzbin(i);
	//cout << "z bin : "<<(bin>>1)<<" "<<(bin&1)<<endl;
	int start=(bin>>1);
	int last=start+(bin&1);
	for(int ibin=start;ibin<=last;ibin++) {
	  for(unsigned int j=0;j<outervmstubs_->nStubsBinned(ibin);j++){
	    std::pair<FPGAStub*,L1TStub*> outerstub=outervmstubs_->getStubBinned(ibin,j);
	    countall++;
	    if (0) {
	      TiltedGeometryInfo *tgi = TiltedGeometryInfo::getInstance();
	      unsigned int detId1 = innerstub.second->stackDetId();
	      double r1 = tgi->r(detId1)/1000;
	      double tiltAngle1 = tgi->tiltAngle(detId1);
	      double sensorSpacing1 = tgi->sensorSpacing(detId1);
	      double z1 = tgi->z(detId1)/1000;
	      double theta1 = atan(r1/z1);
	      double invpitch1 = 126.246686;
	      if (innerstub.second->isPSmodule()) {
		invpitch1 = 102.030405;
	      }
	      unsigned int detId2 = outerstub.second->stackDetId();
	      double r2 = tgi->r(detId2)/1000;
	      double tiltAngle2 = tgi->tiltAngle(detId2);
	      double sensorSpacing2 = tgi->sensorSpacing(detId2);
	      double z2 = tgi->z(detId2)/1000;
	      double theta2 = atan(r2/z2);
	      double invpitch2 = 126.246686;
	      if (outerstub.second->isPSmodule()) {
		invpitch2 = 102.030405;
	      }
	      double pt1 = 0.57*(innerstub.second->bend()/fabs(innerstub.second->bend()))*r1*sqrt(1+invpitch1*(pow(sin(theta1)*sensorSpacing1,2)/pow(cos(theta1-tiltAngle1)*innerstub.second->bend(),2)));
	      if (innerstub.second->bend()==0) {
		pt1 = 0.57*r1*sqrt(1+invpitch1*(pow(sin(theta1)*sensorSpacing1,2)/pow(cos(theta1-tiltAngle1)*innerstub.second->bend(),2)));
	      }
	      double pt2 = 0.57*(outerstub.second->bend()/fabs(outerstub.second->bend()))*r2*sqrt(1+invpitch2*(pow(sin(theta2)*sensorSpacing2,2)/pow(cos(theta2-tiltAngle2)*outerstub.second->bend(),2)));
	      if (outerstub.second->bend()==0) {
		pt2 = 0.57*r2*sqrt(1+invpitch2*(pow(sin(theta2)*sensorSpacing2,2)/pow(cos(theta2-tiltAngle2)*outerstub.second->bend(),2)));
	      }
	      
	      outd << "LL " << innerstub.second->allStubIndex() <<" "<< layer1_ <<" "<< innerstub.second->bend() <<" "<< pt1 <<" "<< outerstub.second->simtrackid() <<" "<< layer2_ <<" "<< outerstub.second->bend() <<" "<< pt2 <<" "<< 1.0/pt1 - 1.0/pt2 << endl;
	      
	      double pass = 1.0;
	      if (layer1_==1 && layer2_==2) {
		pass = 0.6;
	      }
	      if (layer1_==3 && layer2_==4) {
		pass = 0.4;
	      }
	      if (layer1_==5 && layer2_==6) {
		pass = 0.3;
	      }
	      
	      if (fabs(1.0/pt1-1.0/pt2)>pass) {
		if (debug1) {
		  cout << "FPGATrackletEngine Rejected1 pt1 pt2 : "<<pt1<<" "<<pt2<<endl;
		}
		continue;
	      }
	    }
	    countpass++;
	    //cout << "FPGATrackletEngine : "<<getName()<<" Adding stub pair"<<endl;
	    stubpairs_->addStubPair(innerstub,outerstub);
	  }
	}
	
      } else if ((disk1_==1 && disk2_==2)||
		 (disk1_==3 && disk2_==4)) {

	if (debug1) cout << "Disk-disk pair" <<endl;

	int bin=innervmstubs_->getrbin(i);
	//cout << "r bin : "<<(bin>>1)<<" "<<(bin&1)<<endl;
	int start=(bin>>1);
	int last=start+(bin&1);
	for(int ibin=start;ibin<=last;ibin++) {
	  for(unsigned int j=0;j<outervmstubs_->nStubsBinned(ibin);j++){
	    std::pair<FPGAStub*,L1TStub*> outerstub=outervmstubs_->getStubBinned(ibin,j);
	    countall++;
	    if (0) {
	      TiltedGeometryInfo *tgi = TiltedGeometryInfo::getInstance();
	      unsigned int detId1 = innerstub.second->stackDetId();
	      double r1 = tgi->r(detId1)/1000;
	      double tiltAngle1 = tgi->tiltAngle(detId1);
	      double sensorSpacing1 = tgi->sensorSpacing(detId1);
	      double z1 = tgi->z(detId1)/1000;
	      double theta1 = atan(r1/z1);
	      double invpitch1 = 126.246686;
	      if (innerstub.second->isPSmodule()) {
		invpitch1 = 102.030405;
	      }
	      unsigned int detId2 = outerstub.second->stackDetId();
	      double r2 = tgi->r(detId2)/1000;
	      double tiltAngle2 = tgi->tiltAngle(detId2);
	      double sensorSpacing2 = tgi->sensorSpacing(detId2);
	      double z2 = tgi->z(detId2)/1000;
	      double theta2 = atan(r2/z2);
	      double invpitch2 = 126.246686;
	      if (outerstub.second->isPSmodule()) {
		invpitch2 = 102.030405;
	      }

	      double pt1 = 0.57*(innerstub.second->bend()/fabs(innerstub.second->bend()))*r1*sqrt(1+invpitch1*(pow(sin(theta1)*sensorSpacing1,2)/pow(cos(theta1-tiltAngle1)*innerstub.second->bend(),2)));
	      if (innerstub.second->bend()==0) {
		pt1 = 0.57*r1*sqrt(1+invpitch1*(pow(sin(theta1)*sensorSpacing1,2)/pow(cos(theta1-tiltAngle1)*innerstub.second->bend(),2)));
	      }
	      double pt2 = 0.57*(outerstub.second->bend()/fabs(outerstub.second->bend()))*r2*sqrt(1+invpitch2*(pow(sin(theta2)*sensorSpacing2,2)/pow(cos(theta2-tiltAngle2)*outerstub.second->bend(),2)));
	      if (outerstub.second->bend()==0) {
		pt2 = 0.57*r2*sqrt(1+invpitch2*(pow(sin(theta2)*sensorSpacing2,2)/pow(cos(theta2-tiltAngle2)*outerstub.second->bend(),2)));
	      }
	      
	      outd << "DD " << innerstub.second->simtrackid() <<" "<< disk1_ <<" "<< innerstub.second->bend() <<" "<< pt1 <<" "<< outerstub.second->simtrackid() <<" "<< disk2_ <<" "<< outerstub.second->bend() <<" "<< pt2 <<" "<< 1.0/pt1-1.0/pt2 << endl;
	      
	      double pass = 1.0;
	      if (disk1_==1 && disk2_==2) {
		pass = 0.8;
	      }
	      if (disk1_==3 && disk2_==4) {
		pass = 0.7;
	      }
	      
	      if (fabs(1.0/pt1-1.0/pt2)>pass) {
		if (debug1) {
		  cout << "FPGATrackletEngine Rejected1 pt1 pt2 : "<<pt1<<" "<<pt2<<endl;
		}
		continue;
	      }
	    }
	    countpass++;
	    //cout << "FPGATrackletEngine : "<<getName()<<" Adding stub pair"<<endl;
	    stubpairs_->addStubPair(innerstub,outerstub);
	  }
	}
      }	else if (disk1_==1 && layer2_==1) {

	if (debug1) cout << "Disk+layer1 pair" <<endl;
	
	int bin=innervmstubs_->getzbinfromdisk(i);
	//cout << "z bin : "<<(bin>>1)<<" "<<(bin&1)<<endl;
	int start=(bin>>1);
	int last=start+(bin&1);
	for(int ibin=start;ibin<=last;ibin++) {
	  for(unsigned int j=0;j<outervmstubs_->nStubsBinned(ibin);j++){
	    std::pair<FPGAStub*,L1TStub*> outerstub=outervmstubs_->getStubBinned(ibin,j);
	    countall++;
	    if (0) {
	      TiltedGeometryInfo *tgi = TiltedGeometryInfo::getInstance();
	      unsigned int detId1 = innerstub.second->stackDetId();
	      double r1 = tgi->r(detId1)/1000;
	      double tiltAngle1 = tgi->tiltAngle(detId1);
	      double sensorSpacing1 = tgi->sensorSpacing(detId1);
	      double z1 = tgi->z(detId1)/1000;
	      double theta1 = atan(r1/z1);
	      double invpitch1 = 126.246686;
	      if (innerstub.second->isPSmodule()) {
		invpitch1 = 102.030405;
	      }
	      unsigned int detId2 = outerstub.second->stackDetId();
	      double r2 = tgi->r(detId2)/1000;
	      double tiltAngle2 = tgi->tiltAngle(detId2);
	      double sensorSpacing2 = tgi->sensorSpacing(detId2);
	      double z2 = tgi->z(detId2)/1000;
	      double theta2 = atan(r2/z2);
	      double invpitch2 = 126.246686;
	      if (outerstub.second->isPSmodule()) {
		invpitch2 = 102.030405;
	      }

	      double pt1 = 0.57*(innerstub.second->bend()/fabs(innerstub.second->bend()))*r1*sqrt(1+invpitch1*(pow(sin(theta1)*sensorSpacing1,2)/pow(cos(theta1-tiltAngle1)*innerstub.second->bend(),2)));
	      if (innerstub.second->bend()==0) {
		pt1 = 0.57*r1*sqrt(1+invpitch1*(pow(sin(theta1)*sensorSpacing1,2)/pow(cos(theta1-tiltAngle1)*innerstub.second->bend(),2)));
	      }
	      double pt2 = 0.57*(outerstub.second->bend()/fabs(outerstub.second->bend()))*r2*sqrt(1+invpitch2*(pow(sin(theta2)*sensorSpacing2,2)/pow(cos(theta2-tiltAngle2)*outerstub.second->bend(),2)));
	      if (outerstub.second->bend()==0) {
		pt2 = 0.57*r2*sqrt(1+invpitch2*(pow(sin(theta2)*sensorSpacing2,2)/pow(cos(theta2-tiltAngle2)*outerstub.second->bend(),2)));
	      }
	      
	      outd << "DL1 " << innerstub.second->simtrackid() <<" "<< disk1_ <<" "<< innerstub.second->bend() <<" "<< pt1 <<" "<< outerstub.second->simtrackid() <<" "<< layer2_ <<" "<< outerstub.second->bend() <<" "<< pt2 <<" "<< 1.0/pt1-1.0/pt2 << endl;
	      
	      double pass = 0.8;

	      if (fabs(1.0/pt1-1.0/pt2)>pass) {
		if (debug1) {
		  cout << "FPGATrackletEngine Rejected1 pt1 pt2 : "<<pt1<<" "<<pt2<<endl;
		}
		continue;
	      }
            }
	    countpass++;
	    //cout << "FPGATrackletEngine : "<<getName()<<" Adding stub pair"<<endl;
	    stubpairs_->addStubPair(innerstub,outerstub);
	  }
	}

      } else if (disk1_==1 && layer2_==2) {

	if (debug1) cout << "Disk+layer2 pair" <<endl;
	
	int bin=0;
	if (innerstub.first->isBarrel()) {
	  bin=innervmstubs_->getzbin(i,true);
	} else {
	  bin=innervmstubs_->getzbinfromdisk(i,layer2_);
	}
	//cout << "z bin : "<<(bin>>1)<<" "<<(bin&1)<<endl;
	int start=(bin>>1);
	int last=start+(bin&1);
	for(int ibin=start;ibin<=last;ibin++) {
	  for(unsigned int j=0;j<outervmstubs_->nStubsBinned(ibin);j++){
	    std::pair<FPGAStub*,L1TStub*> outerstub=outervmstubs_->getStubBinned(ibin,j);

	    //countall++;
	    double r1=innerstub.second->r();
	    double z1=innerstub.second->z();
	    
	    double r2=outerstub.second->r();
	    double z2=outerstub.second->z();
	    
	    double z0=z1-(z1-z2)*r1/(r1-r2);
	    
	    if (fabs(z0)>30.0) continue;


	    double rinv=2*sin(outerstub.second->phi()-innerstub.second->phi())/
	      (innerstub.second->r()-outerstub.second->r());
	    
	    double pt=0.00299792*3.8/rinv;

	    if (fabs(pt)<ptcut-0.3) continue;

	    
	    countall++;
	    if (0) {
	      TiltedGeometryInfo *tgi = TiltedGeometryInfo::getInstance();
	      unsigned int detId1 = innerstub.second->stackDetId();
	      double cr1 = tgi->r(detId1)/1000;
	      double tiltAngle1 = tgi->tiltAngle(detId1);
	      double sensorSpacing1 = tgi->sensorSpacing(detId1);
	      double cz1 = tgi->z(detId1)/1000;
	      double theta1 = atan(cr1/cz1);
	      double invpitch1 = 126.246686;
	      if (innerstub.second->isPSmodule()) {
		invpitch1 = 102.030405;
	      }
	      unsigned int detId2 = outerstub.second->stackDetId();
	      double cr2 = tgi->r(detId2)/1000;
	      double tiltAngle2 = tgi->tiltAngle(detId2);
	      double sensorSpacing2 = tgi->sensorSpacing(detId2);
	      double cz2 = tgi->z(detId2)/1000;
	      double theta2 = atan(cr2/cz2);
	      double invpitch2 = 126.246686;
	      if (outerstub.second->isPSmodule()) {
		invpitch2 = 102.030405;
	      }

	      double pt1 = 0.57*(innerstub.second->bend()/fabs(innerstub.second->bend()))*cr1*sqrt(1+invpitch1*(pow(sin(theta1)*sensorSpacing1,2)/pow(cos(theta1-tiltAngle1)*innerstub.second->bend(),2)));
	      if (innerstub.second->bend()==0) {
		pt1 = 0.57*cr1*sqrt(1+invpitch1*(pow(sin(theta1)*sensorSpacing1,2)/pow(cos(theta1-tiltAngle1)*innerstub.second->bend(),2)));
	      }
	      double pt2 = 0.57*(outerstub.second->bend()/fabs(outerstub.second->bend()))*cr2*sqrt(1+invpitch2*(pow(sin(theta2)*sensorSpacing2,2)/pow(cos(theta2-tiltAngle2)*outerstub.second->bend(),2)));
	      if (outerstub.second->bend()==0) {
		pt2 = 0.57*cr2*sqrt(1+invpitch2*(pow(sin(theta2)*sensorSpacing2,2)/pow(cos(theta2-tiltAngle2)*outerstub.second->bend(),2)));
	      }

	      outd << "DL2 " << innerstub.second->simtrackid() <<" "<< disk1_ <<" "<< innerstub.second->bend() <<" "<< pt1 <<" "<< outerstub.second->simtrackid() <<" "<< layer2_ <<" "<< outerstub.second->bend() <<" "<< pt2 <<" "<< 1.0/pt1-1.0/pt2 << endl;

	      double pass = 0.8;

	      if (fabs(1.0/pt1-1.0/pt2)>pass) {
		if (debug1) {
		  cout << "FPGATrackletEngine Rejected1 pt1 pt2 : "<<pt1<<" "<<pt2<<endl;
		}
		continue;
	      }
            }
	    countpass++;
	    //cout << "FPGATrackletEngine : "<<getName()<<" Adding stub pair"<<endl;
	    stubpairs_->addStubPair(innerstub,outerstub);
	  }
	}

      } else {

	assert(0);
	
	for(unsigned int j=0;j<outervmstubs_->nStubs();j++){
	  std::pair<FPGAStub*,L1TStub*> outerstub=outervmstubs_->getStub(j);
	  countall++;


	  if (layer1_!=0 && layer2_!=0) {

	    assert(0); //should never get here!!!
	    if (debug1) {
	      cout << "In "<<getName()<<" have barrel candidate stub pair"<<endl;
	    }
	    
	  
	    double r1=innerstub.second->r();
	    double z1=innerstub.second->z();
	    
	    double r2=outerstub.second->r();
	    double z2=outerstub.second->z();

	    double z2avg=z1*r2/r1;

	    //cout << "z2avg z1 z2 : "<<z2avg<<" "<<z1<<" "<<z2<<endl;
	  
	    if (fabs(z2avg-z2)>30.0) {
	      countall--; //undo the count since in the proper implementation this will be binned
	      continue;
	    }
	    
	    double z0=z1-(z1-z2)*r1/(r1-r2);
	    
	    //cout << "z0 : "<<z0<<endl;

	    
	    if (layer1_==1&&fabs(z0)>20.0) continue;
	    if (layer1_==3&&fabs(z0)>30.0) continue;
	    if (layer1_==5&&fabs(z0)>40.0) continue;
	  
	    //cout << "FPGATrackletEngine z0 = "<<z0<<endl;
	    
	    double rinv=2*sin(outerstub.second->phi()-innerstub.second->phi())/
	      (innerstub.second->r()-outerstub.second->r());
	    
	    double pt=0.00299792*3.8/rinv;

	    if (fabs(pt)<ptcut-0.3) continue;
	    
	    double pt1=innerstub.second->pt();
	    double pt2=outerstub.second->pt();
	    
	    if (fabs(1.0/pt-1.0/pt1)>teptconsistency||
		fabs(1.0/pt-1.0/pt2)>teptconsistency) {
	      if (debug1) {
		cout << "FPGATrackletEngine Rejected1 pt pt1 pt2 : "<<pt<<" "
		     <<pt1<<" "<<pt2<<endl;
	      }
	      continue;
	    }
	    

	  } else if (disk1_!=0 && disk2_!=0) {

	    assert(0); //should never get here!!!

	    if (debug1) {
	      cout << "In "<<getName()<<" have endcap candidate stub pair"<<endl;
	    }
	    
	    double z1=innerstub.second->z();
	    double z2=outerstub.second->z();

	    if (z1*z2<0.0) {
	      countall--; //undo the count since in the proper implementation this will be binned
	      continue;
	    }
	    
	    double r1=innerstub.second->r();
	    double r2=outerstub.second->r();

	    double rproj=(r1/z1)*z2;
	    
	    if (fabs(r2-rproj)>5.0) {
	      if (debug1) {
		cout << "FPGATrackletEngine failed rproj cut"<<endl;
	      }
	      countall--; //undo the count since in the proper implementation this will be binned
	      continue;
	    }
	    
	    double z0=z1-(z1-z2)*r1/(r1-r2);

	    if (fabs(z0)>25.0) {
	      if (debug1) {
		cout << "FPGATrackletEngine failed z0 cut"<<endl;
	      }
	      continue;
	    }
	    
	    //cout << "FPGATrackletEngine z0 = "<<z0<<endl;

	    double rinv=2*sin(outerstub.second->phi()-innerstub.second->phi())/
	      (innerstub.second->r()-outerstub.second->r());
	    
	    double pt=0.00299792*3.8/rinv;

	    if (fabs(pt)<ptcut-0.3) {
	      if (debug1) {
		cout << "FPGATrackletEngine failed min pt cut"<<endl;
	      }
	      continue;
	    }
	    
	    //cout << "pt = "<<pt<<endl;
	    
	    double pt1=innerstub.second->pt();
	    double pt2=outerstub.second->pt();
	    
	    if (z1<0) {   //HACK
	      pt1=-pt1;
	      pt2=-pt2;
	    }
	    
	    
	    if (fabs(1.0/pt-1.0/pt1)>teptconsistencydisk||
		fabs(1.0/pt-1.0/pt2)>teptconsistencydisk) {
	      if (debug1) {
		cout << "FPGATrackletEngine Rejected2 pt pt1 pt2 : "
		     <<pt<<" "<<pt1<<" "<<pt2<<endl;
	      }
	      continue;
	    }

	  } else if (disk1_!=0 && layer2_!=0) {

	    cout << "disk1_ layer2_ "<<disk1_<<" "<<layer2_<<endl;
	    
	    assert(0);
	    
	    if (debug1) {
	      cout << "In "<<getName()<<" have overlap candidate stub pair"<<endl;
	    }


	    double z1=innerstub.second->z();
	  
	    double z2=outerstub.second->z();
	    
	    //cout << "z1 = "<<z1<<endl;
	    //cout << "r1 = "<<innerstub.second->r()<<endl;
	    
	    //cout << "z2 = "<<z2<<endl;
	    //cout << "r2 = "<<outerstub.second->r()<<endl;
	    
	    //if (fabs(z2)<57.0) {
	    //  countall--; //undo the count since in the proper implementation only stubs in correct z range will be includes
	    //  continue;
	    //}
	    
	  
	    if (z1*z2<0.0) {
	      countall--; //undo the count since in the proper implementation this will be binned
	      continue;
	    }
	    
	    
	    double r1=innerstub.second->r();
	    
	    double r2=outerstub.second->r();

	    if (fabs(r1)>42.0&&layer2_==1) {
	      countall--; //undo the count since in the proper implementation only stubs in correct z range will be includes
	      continue;
	    }
	    
	  
	    if (r2>r1) continue;

	    double rproj=(r2/z2)*z1;
	    
	    if (fabs(r1-rproj)>5.0) {
	      countall--; //undo the count since in the proper implementation this will be binned
	      continue;
	    }
	    

	    double z0=z1-(z1-z2)*r1/(r1-r2);
	    
	    if (fabs(z0)>25.0) continue;
	    
	  
	    //cout << "FPGATrackletEngine z0 = "<<z0<<endl;
	    
	    double rinv=2*sin(outerstub.second->phi()-innerstub.second->phi())/
	      (innerstub.second->r()-outerstub.second->r());
	    
	    double pt=0.00299792*3.8/rinv;

	    if (fabs(pt)<ptcut-0.3) continue;
	    
	    double pt1=innerstub.second->pt();
	    double pt2=outerstub.second->pt();
	    
	    if (z1<0) {   //HACK
	      pt1=-pt1;
	    }
	    
	  
	    if (fabs(1.0/pt-1.0/pt1)>teptconsistencyoverlap||
		fabs(1.0/pt-1.0/pt2)>teptconsistencyoverlap) {
	      if (debug1) {
		cout << "FPGATrackletEngine Rejected3 pt pt1 pt2 : "<<pt<<" "
		     <<pt1<<" "<<pt2<<endl;
	      }
	      continue;
	    }

	    if (debug1) {
	      cout << "Found overlap stub pair "<<getName()<<endl;
	    }
	    
	  } else {
	    assert(0);
	  }
	  
	  if (debug1) {
	    cout << "Adding stub pair in "<<getName()<<endl;
	  }
	  assert(stubpairs_!=0);
	  countpass++;
	  //cout << "FPGATrackletEngine : "<<getName()<<" Adding stub pair"<<endl;
	  stubpairs_->addStubPair(innerstub,outerstub);
	  
	  if (countall>=MAXTE) break;
	}
	if (countall>=MAXTE) break;
      }

      if (countall>5000) {
	cout << "In FPGATrackletEngine::execute : "<<getName()
	     <<" "<<innervmstubs_->nStubs()
	     <<" "<<outervmstubs_->nStubs()
	     <<" "<<countall<<" "<<countpass
	     <<endl;
	for(unsigned int i=0;i<innervmstubs_->nStubs();i++){
	  std::pair<FPGAStub*,L1TStub*> innerstub=innervmstubs_->getStub(i);
	  cout << "In FPGATrackletEngine::execute inner stub : "
	       << innerstub.second->r()<<" "
	       << innerstub.second->phi()<<" "
	       << innerstub.second->r()*innerstub.second->phi()<<" "
	       << innerstub.second->z()<<endl;
	}
	for(unsigned int i=0;i<outervmstubs_->nStubs();i++){
	  std::pair<FPGAStub*,L1TStub*> outerstub=outervmstubs_->getStub(i);
	  cout << "In FPGATrackletEngine::execute outer stub : "
	       << outerstub.second->r()<<" "
	       << outerstub.second->phi()<<" "
	       << outerstub.second->r()*outerstub.second->phi()<<" "
	       << outerstub.second->z()<<endl;
	}
	
      }
    }
      
    if (writeTE) {
      static ofstream out("trackletengine.txt");
      out << getName()<<" "<<countall<<" "<<countpass<<endl;
    }
      
  }

  private:

    double phimax_;
    double phimin_;

    FPGATETable* table_;
    
    int layer1_;
    int layer2_;
    int disk1_;
    int disk2_;
    int dct1_;
    int dct2_;
    int phi1_;
    int phi2_;
    int z1_;
    int z2_;
    int r1_;
    int r2_;
  
    FPGAVMStubsTE* innervmstubs_;
    FPGAVMStubsTE* outervmstubs_;
    
    FPGAStubPairs* stubpairs_;

    
  };

#endif
