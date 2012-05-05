/************************************************************/
/*    NAME: robtruax                                              */
/*    ORGN: MIT                                             */
/*    FILE: CollectTempStats.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "CollectTempStats.h"

using namespace std;
using namespace Eigen;

double wrapDeg(double deg) {
  while (deg > 360.0) {
    deg = deg-360.0;
  }
  while (deg < 0) {
    deg = deg+360.0;
  }
  return deg;
}

double CollectTempStats::getHotHeading() {

  Measurement hot = _meas.getMaxTemp();
  Measurement cold = _meas.getMinTemp();
  double rad_heading = atan2(hot.y-cold.y,hot.x-cold.x);
  double deg_heading = rad_heading * 180.0 / PI - 90;
  deg_heading = wrapDeg(deg_heading);
  return(deg_heading);
}

double CollectTempStats::getHotHeading2() {

  Measurement hot = _meas.getMaxTemp();
  Measurement cold = _meas.getMinTemp();

  double deg_heading = -1.0;

  MatrixXd L = MatrixXd::Zero(3,3);
  MatrixXd R = MatrixXd::Zero(3,1);

  std::vector<Measurement> allMs = _meas.all();

  for (int i = 0; i < allMs.size(); i++) {
    double a = (hot.temp-allMs[i].temp)/(hot.temp-cold.temp)*2-1;
    double x = allMs[i].x;
    double y = allMs[i].y;
    L(0,0) += x*x;
    L(0,1) += x*y;
    L(0,2) += x;
    L(1,0) = L(0,1);
    L(1,1) += y*y;
    L(1,2) += y;
    L(2,0) = L(0,2);
    L(2,1) = L(1,2);
    L(2,2) += 1;
    R(0,0) += x*a;
    R(1,0) += y*a;
    R(2,0) += a;
  }

  MatrixXd theta = L.fullPivLu().solve(R);
  double tx = theta(0,0);
  double ty = theta(1,0);
  double b = theta(2,0);

  publishDecisionLine(tx,ty,b);

  deg_heading = atan2(-ty,-tx);
  deg_heading = deg_heading*180/PI;
  deg_heading = wrapDeg(-deg_heading+90);

  return(deg_heading);
}

void CollectTempStats::publishDecisionLine(double vx, double vy, double b) {
  double len = sqrt(vx*vx + vy*vy);
  double linedist = -b/(vx*vx+vy*vy);
  double centerx = vx*linedist;
  double centery = vy*linedist;

  // line vector -ty,tx
  double x1 = centerx-vy/len*200;
  double y1 = centery+vx/len*200;

  double x2 = centerx+vy/len*200;
  double y2 = centery-vx/len*200;

  stringstream s;
  s << "label,decision_line:vertex_color,1,1,0:edge_size,1.0:vertex_size,2.0:" << x1 << "," << y1 << ":" << x2 << "," << y2;
  m_Comms.Notify("VIEW_SEGLIST",s.str());
}

bool runTester() {
  cout << "HIHIHI" << endl;
  MeasurementList ml;
  ml.add(Measurement(0, 0, 3823.0, 0, "abc"));
  ml.add(Measurement(1, 2, 0, 0, "helloooo"));
  ml.add(Measurement(32.8, 32.232, 0, 0, "helloooo")); // should be ignored
  ml.add(Measurement(328.238, 0, 3283.328, 32832082380.338, "abc"));
  ml.add(Measurement(-330.302, -20, -2382, 3282082.238, "abc"));
  std::cout << ml.toString() << endl;
  if (ml.toString() != "0,0,3823,0,abc@1,2,0,0,helloooo@328.238,0,3283.33,3.28321e+10,abc@-330.302,-20,-2382,3.28208e+06,abc@") {
    cout << "Test Failed" << endl;
    return(false);
  }

  cout << "MeasurementList test passed" << endl;
  return(true);
}

//---------------------------------------------------------
// Constructor

CollectTempStats::CollectTempStats()
{
  runTester();
  m_iterations = 0;
  m_timewarp   = 1;
}

//---------------------------------------------------------
// Destructor

CollectTempStats::~CollectTempStats()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CollectTempStats::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

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

    if (msg.GetKey() == "UCTD_MSMNT_REPORT") {
      // Process
      double x,y,temp;
      vector<string> inmsg = parseString(msg.GetString(), ",");
      for (int i = 0; i < inmsg.size(); i++) {
	vector<string> items = parseString(inmsg[i],"=");
	if (items.size() == 2) {
	  if (items[0] == "temp") {
	    temp = atof(items[1].c_str());
	  }
	  else if (items[0] == "x") {
	    x = atof(items[1].c_str());
	  }
	  else if (items[0] == "y") {
	    y = atof(items[1].c_str());
	  }
	}
      }

      Measurement m = Measurement(x, y, temp, MOOSTime(), this->vname);
      _meas.add(m);
      this->sendFullState(_meas.toString());
    }
    else if (msg.GetKey() == "FULL_STATE") {
	// parse state
	string allMeasurements = msg.GetString();
	// read all of the measurements in
    }
    else if (msg.GetKey() == "SURVEY_UNDERWAY") {
      if (msg.GetString() == "true" && _last_underway_state == false) {
	// Reset stats for new run
	clearMeasurements();
      }

      if (msg.GetString() == "true") {
	_last_underway_state = true;
      }
      else if (msg.GetString() == "false") {
	_last_underway_state = false;
      }

    }
  }
	
   return(true);
}

void CollectTempStats::sendFullState(string stateMsg) {
    stringstream msg;
    msg << "src_node=" << "archie"
	<< ",dest_node=all,var_name=FULL_STATE,string_val=\"" 
	<< stateMsg << "\"";
    cout << "sent to other " << msg.str() << endl;
    m_Comms.Notify("NODE_MESSAGE_LOCAL", msg.str());
    usleep(200 * 1000);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CollectTempStats::OnConnectToServer()
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

bool CollectTempStats::Iterate()
{
  m_iterations++;
  if (_meas.all().size() > 3 && m_iterations % 20 == 0) {
    m_Comms.Notify("HOT_HEADING", getHotHeading2());
    m_Comms.Notify("MAX_TEMP", _meas.getMaxTemp().temp);
    m_Comms.Notify("MIN_TEMP", _meas.getMinTemp().temp);
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CollectTempStats::OnStartUp()
{

  // // Eigen test
  // MatrixXd m(2,2);
  // m(0,0) = 1;
  // m(1,0) = 2;
  // m(0,1) = 3;
  // m(1,1) = 4;

  // MatrixXd b(2,1);
  // b(0,0) = 5;
  // b(1,0) = 6;

  // cout << "m:\n" << m << endl;
  // cout << "b:\n" << b << endl;

  // MatrixXd ans(2,1);
  // ans = m.colPivHouseholderQr().solve(b);

  // cout << "ans:\n" << ans << endl;


  // Initializations
  _last_underway_state = false;

  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "VEHICLENAME") {
	  this->vname = value;
      }

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

void CollectTempStats::RegisterVariables()
{
  m_Comms.Register("UCTD_MSMNT_REPORT", 0);
  m_Comms.Register("NAV_X", 0);
  m_Comms.Register("NAV_Y", 0);
  m_Comms.Register("SURVEY_UNDERWAY", 0);
  m_Comms.Register("FULL_STATE", 0);
}

