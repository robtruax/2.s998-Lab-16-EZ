/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: Genetic.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Genetic_HEADER
#define Genetic_HEADER

#include "MBUtils.h"
#include "MOOSLib.h"
#include <string>

using namespace std;

class Genetic : public CMOOSApp
{
 public:
   Genetic();
   ~Genetic();

   int timeout;
   bool sentDeploy;

 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();
   void SendMessage(string, string);

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
   double startTime;
};

#endif 
