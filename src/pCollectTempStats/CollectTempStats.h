/************************************************************/
/*    NAME: robtruax                                              */
/*    ORGN: MIT                                             */
/*    FILE: CollectTempStats.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef CollectTempStats_HEADER
#define CollectTempStats_HEADER

#include "MeasurementList.h"
#include "MOOSLib.h"
#include "MBUtils.h"
#include <math.h>
#include <Eigen/Dense>
#include <string>

using namespace std;

class CollectTempStats : public CMOOSApp
{
 public:
    CollectTempStats();
   ~CollectTempStats();

 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();

   void clearMeasurements() {
     _meas.clear();
     _otherMeas.clear();
   };

   double getHotHeading();
   double getHotHeading2();
   void publishDecisionLine(double,double,double);
   void sendFullState(string state);
   bool runTester();

 private: // Configuration variables
   double _surveyTime;

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
   string _vname;
   int _otherIndex; // next index in other_meas to publish

   bool _last_underway_state;
   double _lastTime;

   MeasurementList _meas; // copy of ownship current full state
   MeasurementList _otherMeas; // copy of full state of other vehicle

};

#endif 
