/************************************************************/
/*    NAME: robtruax                                              */
/*    ORGN: MIT                                             */
/*    FILE: CollectTempStats.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef MeasurementList_HEADER
#define MeasurementList_HEADER

#include "Measurement.h"
#include "MOOSLib.h"
#include "MBUtils.h"
#include <iterator>
#include <iostream>
#include <string>
#include <math.h>

class MeasurementList {
public:

    MeasurementList() {};

    MeasurementList(std::string s) {
	this->fromString(s);
    }

    std::string toString() {
	std::stringstream s;
	for (int i = 0; i < _meas.size(); i++) {
	    s << _meas[i].toString() << ":";
	}
	return s.str();
    };

    void fromString(std::string encodedMeasurementList) {
	std::vector<std::string> svector = parseString(encodedMeasurementList, ':');
	for (int i = 0; i < svector.size(); i++) {
	    this->add(Measurement(svector[i]));
	}
    };

    void integrate(MeasurementList other) {
	
    };

    void clear() {
	_meas.clear();
    };

    Measurement getMaxTemp() {
	Measurement m = Measurement(0,0,0);
	double max_t = -1;
	for (int i = 0; i < _meas.size(); i++) {
	    if (_meas[i].temp > max_t) {
		m = _meas[i];
		max_t = _meas[i].temp;
	    }
	}
	return(m);
    }

    Measurement getMinTemp() {
	Measurement m = Measurement(0,0,0);
	double min_t = -1;
	for (int i = 0; i < _meas.size(); i++) {
	    if (_meas[i].temp < min_t || min_t < 0) {
		m = _meas[i];
		min_t = _meas[i].temp;
	    }
	}

	return(m);
    }

    std::vector<Measurement> all() {
	return _meas;
    };

    void add(Measurement m) {
	for (int i = 0; i < _meas.size(); i++) {
	    if (m.timestamp == _meas[i].timestamp && m.vehicleID == _meas[i].vehicleID) {
		return;
	    }
	}
	_meas.push_back(m);
    };


    //private:
 public:
    std::vector<Measurement> _meas;

};

#endif
