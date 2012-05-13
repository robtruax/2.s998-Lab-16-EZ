/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: Genetic.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "Genetic.h"

using namespace std;

//---------------------------------------------------------
// Constructor

#define TIME_OFFSET 0

Genetic::Genetic()
{
    m_iterations = 0;
    m_timewarp   = 1;
    sentDeploy = false;
    // THIS DOES NOT WORK!!! START TIME WILL BE 0 STILL. SEE LATER IN CODE.
//    startTime = MOOSTime();
}

//---------------------------------------------------------
// Destructor

Genetic::~Genetic()
{
}

void Genetic::SendMessage(string variableName, string variablesString) {
    stringstream msg;
    string _vname = "genetic"; // TODO
    msg << "src_node=aegir"
	<< ",dest_node=all,var_name=" + variableName + ",string_val=\"" 
	<< variablesString << "\"";
    //cout << "sent to other " << msg.str() << endl;
    m_Comms.Notify("NODE_MESSAGE_LOCAL", msg.str());
    //usleep(200 * 100000);
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Genetic::OnNewMail(MOOSMSG_LIST &NewMail)
{
    MOOSMSG_LIST::iterator p;
   
    for(p=NewMail.begin(); p!=NewMail.end(); p++) {
	CMOOSMsg &msg = *p;

	string key   = msg.GetKey();
// must be on shoreside for this to work
	if (key == "UCTD_SCORE_REPORT") {
	    cout << "got score report " << msg.GetString() << endl;
	    exit(0);
	}

//    "FOLLOW_UPDATES"
//    pd = 0.5
//    speed = 2.0
//    pwt = 100

// stringstream msg;
	//   msg << "src_node=" << _vname
	// 	<< ",dest_node=all,var_name=FULL_STATE,string_val=\"" 
	// 	<< stateMsg << "\"";
	//   //cout << "sent to other " << msg.str() << endl;
	//   m_Comms.Notify("NODE_MESSAGE_LOCAL", msg.str());
	//   //usleep(200 * 1000);

#if 0 // Keep these around just for template
	string key   = msg.GetKey();
	string comm  = msg.GetCommunity();
	double dval  = msg.GetDouble();
	string sval  = msg.GetString(); 
	string msrc  = msg.GetSource();
	double mtime = msg.GetTime();
	bool   mdbl  = msg.IsDouble();
	bool   mstr  = msg.IsString();
#endif
    }

    return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Genetic::OnConnectToServer()
{
    // register for variables here
    // possibly look at the mission file?
    // m_MissionReader.GetConfigurationParam("Name", <string>);
    // m_Comms.Register("VARNAME", 0);
	
    RegisterVariables();
   
    return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Genetic::Iterate()
{
    m_iterations++;
  
//  SendMessage("FOLLOW_UPDATE", "pd=0.5,speed=2.0,pwt=0");
    m_Comms.Notify("PING_PONG", "pd=0.0");

    if (sentDeploy == false) {
	usleep(10 * 500000);
//	cout << "deploying..." << endl;
	m_Comms.Notify("DEPLOY_ALL", "true");
	m_Comms.Notify("DEPLOY", "true");
	m_Comms.Notify("LOITER_ALL", "true");
	m_Comms.Notify("MOOS_MANUAL_OVERIDE_ALL", "false");
	m_Comms.Notify("STATION_ALL", "false");
	m_Comms.Notify("RETURN_ALL", "false");
	startTime = MOOSTime();
	sentDeploy = true;
    }
    else if ((MOOSTime() - startTime) > timeout) {
 	// spam the return message system
        // from meta_shoreside pMarineViewer return all button
//	cout << "returning...";
	m_Comms.Notify("RETURN_ALL", "true");
	m_Comms.Notify("RETURN", "true");
	m_Comms.Notify("LOITER_ALL", "false");
	m_Comms.Notify("MOOS_MANUAL_OVERIDE_ALL", "false");
	m_Comms.Notify("STATION_ALL", "false");
    } 
//    cout << "iter done " << MOOSTime() - startTime << "timeout " << timeout << endl;
    usleep(10 * 10000);
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Genetic::OnStartUp()
{
    list<string> sParams;
    m_MissionReader.EnableVerbatimQuoting(false);
    if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
	list<string>::iterator p;
	for(p=sParams.begin(); p!=sParams.end(); p++) {
	    string original_line = *p;
	    string param = stripBlankEnds(toupper(biteString(*p, '=')));
	    string value = stripBlankEnds(*p);
      
	    if(param == "FOO") {
		//handled
	    }
	    else if(param == "BAR") {
		//handled
	    }
	}
    }
  
    m_timewarp = GetMOOSTimeWarp();

    RegisterVariables();	
    return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Genetic::RegisterVariables()
{
    // m_Comms.Register("FOOBAR", 0);
    m_Comms.Register("UCTD_SCORE_REPORT", 0);
}

