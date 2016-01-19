#pragma once
#include "Manipulator.h"
#include "Defines.h"

class CRobotControl
{
public:
	CRobotControl(CManipulator *myManipulator);
	~CRobotControl(void);
	
	void init();
	void GetManipulatorState(SManipulatorState myState);
	void GetGivenState(SGivenState myGiven);

	void SetManipulatorPara();
	void InitPara();
	void ChangePara();
	void Controller(int ControllerType);

	CManipulator *ManipulatorControl;
	SManipulatorState CurState;
	SGivenState GivenState;
	SCommand CommandManipulator;
	SBodyPara bodyparaBase;
	SJointPara jointParaManipulator[NUMOFLEGJOINTS];
	SColor colorManipulator;
	SPosition positionLink[NUMOFLEGJOINTS];
	SStopPara stopparaManipulator[NUMOFLEGJOINTS];
	double border;

private:

	void Vel_Controller();
	void Angle_Controller();
	void Force_Controller();

};
