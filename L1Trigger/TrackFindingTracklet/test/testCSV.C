#include "../interface/TiltedGeometryInfo.hh"

#include <string>
#include <iostream>

void testCSV(std::string fileName = "allCoordinates.csv") {

  TiltedGeometryInfo *tgi = TiltedGeometryInfo::getInstance(fileName);

  std::cout << tgi->r(441201716) << std::endl;
  std::cout << tgi->tiltAngle(441201716) << std::endl;
  std::cout << tgi->sensorSpacing(441201716) << std::endl;
  std::cout << std::endl;
  std::cout << tgi->r(439621692) << std::endl;
  std::cout << tgi->tiltAngle(439621692) << std::endl;
  std::cout << tgi->sensorSpacing(439621692) << std::endl;
  std::cout << std::endl;



}
