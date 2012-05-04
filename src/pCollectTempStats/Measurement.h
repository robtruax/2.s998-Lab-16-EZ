/************************************************************/
/*    NAME: robtruax                                              */
/*    ORGN: MIT                                             */
/*    FILE: CollectTempStats.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Measurement_HEADER
#define Measurement_HEADER

#include "MOOSLib.h"
#include "MBUtils.h"
#include <iterator>
#include <iostream>
#include <string>
#include <math.h>

class Measurement {
public:
    Measurement() { };

    Measurement(double _x, double _y, double _temp) {
	x = _x;
	y = _y;
	temp = _temp;
	timestamp = -1;
	vehicleID = "none";	
    };

    Measurement(double _x, double _y, double _temp, double _timestamp, std::string _vehicleID) {
	x = _x;
	y = _y;
	temp = _temp;
	timestamp = _timestamp;
	vehicleID = _vehicleID;
    };

    Measurement(std::string s) {
	this->fromString(s);
    }

    std::string toString() {
	std::stringstream s;
	s << this->x << "," 
	  << this->y << "," 
	  << this->temp << "," 
	  << this->timestamp << "," 
	  << this->vehicleID;
	return s.str();
    }
    
    void fromString(std::string encodedMeasurement) {
	std::vector<std::string> svector = parseString(encodedMeasurement, ',');
	x = atof(svector[0].c_str());
	y = atof(svector[1].c_str());
	temp = atof(svector[2].c_str());
	timestamp = atof(svector[3].c_str());
	vehicleID  = svector[4];
    }

public:
    double x, y, temp, timestamp;
    std::string vehicleID;

};

#endif
