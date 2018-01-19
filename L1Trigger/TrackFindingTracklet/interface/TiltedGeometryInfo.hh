#ifndef TILTEDGEOMETRYINFO_H
#define TILTEDGEOMETRYINFO_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class TiltedGeometryInfo {

public:

  static TiltedGeometryInfo *getInstance(std::string filename = "") {
    if (filename != "") {
      if (!instance_) {
        instance_ = new TiltedGeometryInfo(filename);
      } else if (instance_->filename_ != filename) {
        //Assume we're trying to switch to a different file name
        delete instance_;
        instance_ = new TiltedGeometryInfo(filename);
      }
    }

    //Warning: Will return a null pointer if you don't call it at
    //least once with a file name
    return instance_;
  }

  double r(const unsigned int &detId) const {
    auto entry = rMap_.find(detId);
    if (entry != rMap_.end()) {
      return entry->second;
    } else {
      return -9e20;
    }
  }
  double tiltAngle(const unsigned int &detId) const {
    auto entry = tiltAngleMap_.find(detId);
    if (entry != tiltAngleMap_.end()) {
      return entry->second;
    } else {
      return -9e20;
    }
  }
  double sensorSpacing(const unsigned int &detId) const {
    auto entry = sensorSpacingMap_.find(detId);
    if (entry != sensorSpacingMap_.end()) {
      return entry->second;
    } else {
      return -9e20;
    }
  }
  double z(const unsigned int &detId) const {
    auto entry = zMap_.find(detId);
    if (entry != zMap_.end()) {
      return entry->second;
    } else {
      return -9e20;
    }
  }

  
private:

  std::map<unsigned int, double> rMap_, tiltAngleMap_, sensorSpacingMap_, zMap_;
  std::string filename_;

  //Below we make the constructor private and have a static pointer so
  //that we can use this class as a "Singlton" (i.e. never more than
  //one)
  static TiltedGeometryInfo *instance_;

  TiltedGeometryInfo(std::string filename): filename_(filename) {
    //Read in the csv file
    std::ifstream csvFile(filename_);
    
    //Loop over the lines in the file
    std::string line;
    int nLine = 0;
    while (std::getline(csvFile,line)) {
        
        //Skip the header row
        if (line.find("DetId") == 0) continue;
        if (line.length() == 0) continue;
        
        std::stringstream lineStream(line);
        std::string field;
        
        unsigned int detId = 0;
        double r = -9e20;
        double tiltAngle = -9e20;
        double sensorSpacing = -9e20;
	double z = -9e20;
        
        int column = 0;
        while (std::getline(lineStream,field,',')) {
          
          switch (column) {
            
          case 0:
            detId = std::stoul(field);
            break;
          case 5:
            r = std::stod(field);
            break;
	  case 6:
	    z = std::stod(field);
	    break;
          case 7:
            tiltAngle = std::stod(field);
            // Convert to radians
            tiltAngle *= 0.017453292519943;
	    // Change to complement
	    tiltAngle = 1.57079632679 - tiltAngle;
            break;
          case 11:
            sensorSpacing = std::stod(field);
            break;
          }
          
          ++column;
        }
        
        if (column < 13) {
          std::cerr << "Line has only " << column << " fields instead of 13.  Skipping." << std::endl
                    << "  Line:" << line << std::endl;
          continue;
        }
        
        rMap_.insert({detId,r});
        tiltAngleMap_.insert({detId,tiltAngle});
        sensorSpacingMap_.insert({detId,sensorSpacing});
	zMap_.insert({detId,z});
        
        ++nLine;
    }

    std::cout << "Read in " << nLine << " lines of geometry info from " << filename_ << std::endl;
  }

};

#endif
