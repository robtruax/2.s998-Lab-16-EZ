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
   };

   double getHotHeading();
   double getHotHeading2();
   void publishDecisionLine(double,double,double);
   void sendFullState(string state);

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
   string vname;

   bool _last_underway_state;

   MeasurementList _meas;
};

#endif 
