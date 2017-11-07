#ifndef TILTEDGEOMETRYINFO_H
#define TILTEDGEOMETRYINFO_H

#include <map>

class TiltedGeometryInfo {

public:
  TiltedGeometryInfo(std::string filename) {
    //Read in the csv file
    std::ifstream csvFile(filename);
    
    //Loop over the lines in the file
    std::string line;
    int nLine = 0;
    while (std::getline(csvFile,line)) {
        
        //Skip the header row
        if (line.find("DetId") == 0) continue;
        
        std::stringstream lineStream(line);
        std::string field;
        
        unsigned int detID = 0;
        double r = -9e20;
        double tiltAngle = -9e20;
        double sensorSpacing = -9e20;
        
        int column = 0;
        while (std::getline(lineStream,field,',')) {
          
          switch (column) {
            
          case 0:
            detID = std::stoul(field);
            break;
          case 5:
            r = std::stod(field);
            break;
          case 7:
            tiltAngle = std::stod(field);
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
        
        rMap.insert({detId,r});
        tiltAngleMap.insert({detId,tiltAngle});
        sensorSpacingMap.insert({detId,sensorSpacing});
        
        ++nLine;
    }

    std::cout << "Read in " << nLine << " lines of geometry info from " << fileName << std::endl;
  }

  double r(const unsigned int &detId) const {return rMap[detId];}
  double tiltAngle(const unsigned int &detId) const {return tiltAngleMap[detId];}
  double sensorSpacing(const unsigned int &detId) const {return sensorSpacingMap[detId];}
}

private:
std::map<unsigned int, double> rMap, tiltAngleMap, sensorSpacingMap;

}

#endif
