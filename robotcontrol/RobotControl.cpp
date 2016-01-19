#include "RobotControl.h"
#include <fstream>
#include <iostream>
using namespace std;
ofstream out1("data\\th1.txt");
ofstream out2("data\\th2.txt");
ofstream out3("data\\th3.txt");
ofstream out4("data\\th4.txt");
ofstream out5("data\\th5.txt");
ofstream out6("data\\th6.txt");

ofstream out13("data\\Giventh1.txt");
ofstream out14("data\\Giventh2.txt");
ofstream out15("data\\Giventh3.txt");
ofstream out16("data\\Giventh4.txt");
ofstream out17("data\\Giventh5.txt");
ofstream out18("data\\Giventh6.txt");


CRobotControl::CRobotControl(CManipulator *myManipulator)
{
	ManipulatorControl=myManipulator;
	init();
	SetManipulatorPara(); 
	return;
}
void CRobotControl::SetManipulatorPara()
{
    InitPara();
	ChangePara();

	ManipulatorControl->baseManipulator.paraBase=bodyparaBase;
	ManipulatorControl->colorManipulator=colorManipulator;
	ManipulatorControl->BORDER=border;
	for(int i=0;i<NUMOFLEGJOINTS;i++)
	{
		ManipulatorControl->stopparaJoint[i]=stopparaManipulator[i];
		ManipulatorControl->jointManipulator[i].paraJoint=jointParaManipulator[i];
	}
	return;
}
void CRobotControl::InitPara() ///后执行，覆盖init()
{
	//set border
	border=0.001;

	//set color
	colorManipulator.B=(float)0.1;
	colorManipulator.R=(float)0.6;
	colorManipulator.G=(float)0.3;

//	set joint paras
	int i;
	double density=1;////////////////////////////
	i=0;
	
	if(i<NUMOFLEGJOINTS)
	{
		jointParaManipulator[i].modeAxis=ZZ;
		jointParaManipulator[i].typeJoint=BOX;
		jointParaManipulator[i].paraManipulatorBox.angle=0;////////////////杆件0绕基座标Y轴的转动角度
		jointParaManipulator[i].paraManipulatorBox.x=0.4;
		jointParaManipulator[i].paraManipulatorBox.y=0.4;////////////////
		jointParaManipulator[i].paraManipulatorBox.z=0.585;////////////////
		if(jointParaManipulator[i].typeJoint==CYLINDER)
			dMassSetCylinder(&jointParaManipulator[i].massBody,density,ZZ,jointParaManipulator[i].paraManipulatorCylinder.r,jointParaManipulator[i].paraManipulatorCylinder.length);
		else if(jointParaManipulator[i].typeJoint==BOX)
			dMassSetBox(&jointParaManipulator[i].massBody,density,jointParaManipulator[i].paraManipulatorBox.x,jointParaManipulator[i].paraManipulatorBox.y,jointParaManipulator[i].paraManipulatorBox.z);
	
	}

	
	i=1;	
	if(i<NUMOFLEGJOINTS)
	{
		jointParaManipulator[i].modeAxis=XX;
		jointParaManipulator[i].typeJoint=BOX;
		jointParaManipulator[i].paraManipulatorBox.angle=0;////////////////杆件0绕基座标Y轴的转动角度
		jointParaManipulator[i].paraManipulatorBox.x=0.4;
		jointParaManipulator[i].paraManipulatorBox.y=0.4;////////////////
		jointParaManipulator[i].paraManipulatorBox.z=0.6;////////////////
		if(jointParaManipulator[i].typeJoint==CYLINDER)
			dMassSetCylinder(&jointParaManipulator[i].massBody,density,ZZ,jointParaManipulator[i].paraManipulatorCylinder.r,jointParaManipulator[i].paraManipulatorCylinder.length);
		else if(jointParaManipulator[i].typeJoint==BOX)
			dMassSetBox(&jointParaManipulator[i].massBody,density,jointParaManipulator[i].paraManipulatorBox.x,jointParaManipulator[i].paraManipulatorBox.y,jointParaManipulator[i].paraManipulatorBox.z);
	}

	i=2;
	if(i<NUMOFLEGJOINTS)
	{
		jointParaManipulator[i].modeAxis=XX;
		jointParaManipulator[i].typeJoint=BOX;
		jointParaManipulator[i].paraManipulatorBox.angle=0;
		jointParaManipulator[i].paraManipulatorBox.x=0.2;////////////////
		jointParaManipulator[i].paraManipulatorBox.y=0.2;////////////////
		jointParaManipulator[i].paraManipulatorBox.z=0.115;////////////////
		if(jointParaManipulator[i].typeJoint==CYLINDER)
			dMassSetCylinder(&jointParaManipulator[i].massBody,density,ZZ,jointParaManipulator[i].paraManipulatorCylinder.r,jointParaManipulator[i].paraManipulatorCylinder.length);
		else if(jointParaManipulator[i].typeJoint==BOX)
			dMassSetBox(&jointParaManipulator[i].massBody,density,jointParaManipulator[i].paraManipulatorBox.x,jointParaManipulator[i].paraManipulatorBox.y,jointParaManipulator[i].paraManipulatorBox.z);
	}

	i=3;
	if(i<NUMOFLEGJOINTS)
	{
		jointParaManipulator[i].modeAxis=XX;
		jointParaManipulator[i].typeJoint=BOX;
		jointParaManipulator[i].paraManipulatorBox.angle=Pi/2;
		jointParaManipulator[i].paraManipulatorBox.x=0.2;////////////////
		jointParaManipulator[i].paraManipulatorBox.y=0.2;////////////////
		jointParaManipulator[i].paraManipulatorBox.z=0.385;////////////////
		if(jointParaManipulator[i].typeJoint==CYLINDER)
			dMassSetCylinder(&jointParaManipulator[i].massBody,density,ZZ,jointParaManipulator[i].paraManipulatorCylinder.r,jointParaManipulator[i].paraManipulatorCylinder.length);
		else if(jointParaManipulator[i].typeJoint==BOX)
			dMassSetBox(&jointParaManipulator[i].massBody,density,jointParaManipulator[i].paraManipulatorBox.x,jointParaManipulator[i].paraManipulatorBox.y,jointParaManipulator[i].paraManipulatorBox.z);
	}

	i=4;
	if(i<NUMOFLEGJOINTS)
	{
		jointParaManipulator[i].modeAxis=YY;
		jointParaManipulator[i].typeJoint=BOX;
		jointParaManipulator[i].paraManipulatorBox.angle=Pi/2;
		jointParaManipulator[i].paraManipulatorBox.x=0.2;////////////////
		jointParaManipulator[i].paraManipulatorBox.y=0.2;////////////////
		jointParaManipulator[i].paraManipulatorBox.z=0.385;////////////////
		if(jointParaManipulator[i].typeJoint==CYLINDER)
			dMassSetCylinder(&jointParaManipulator[i].massBody,density,ZZ,jointParaManipulator[i].paraManipulatorCylinder.r,jointParaManipulator[i].paraManipulatorCylinder.length);
		else if(jointParaManipulator[i].typeJoint==BOX)
			dMassSetBox(&jointParaManipulator[i].massBody,density,jointParaManipulator[i].paraManipulatorBox.x,jointParaManipulator[i].paraManipulatorBox.y,jointParaManipulator[i].paraManipulatorBox.z);
	}

	i=5;
	if(i<NUMOFLEGJOINTS)
	{
		jointParaManipulator[i].modeAxis=XX;
		jointParaManipulator[i].typeJoint=BOX;
		jointParaManipulator[i].paraManipulatorBox.angle=Pi/2;
		jointParaManipulator[i].paraManipulatorBox.x=0.2;////////////////
		jointParaManipulator[i].paraManipulatorBox.y=0.2;////////////////
		jointParaManipulator[i].paraManipulatorBox.z=0.1;////////////////
		if(jointParaManipulator[i].typeJoint==CYLINDER)
			dMassSetCylinder(&jointParaManipulator[i].massBody,density,ZZ,jointParaManipulator[i].paraManipulatorCylinder.r,jointParaManipulator[i].paraManipulatorCylinder.length);
		else if(jointParaManipulator[i].typeJoint==BOX)
			dMassSetBox(&jointParaManipulator[i].massBody,density,jointParaManipulator[i].paraManipulatorBox.x,jointParaManipulator[i].paraManipulatorBox.y,jointParaManipulator[i].paraManipulatorBox.z);
	}

	i=6;
	if(i<NUMOFLEGJOINTS)
	{
		jointParaManipulator[i].modeAxis=YY;
		jointParaManipulator[i].typeJoint=BOX;
		jointParaManipulator[i].paraManipulatorBox.angle=Pi/2;
		jointParaManipulator[i].paraManipulatorBox.x=0.1;////////////////
		jointParaManipulator[i].paraManipulatorBox.y=0.1;////////////////
		jointParaManipulator[i].paraManipulatorBox.z=0.6;////////////////
		if(jointParaManipulator[i].typeJoint==CYLINDER)
			dMassSetCylinder(&jointParaManipulator[i].massBody,density,ZZ,jointParaManipulator[i].paraManipulatorCylinder.r,jointParaManipulator[i].paraManipulatorCylinder.length);
		else if(jointParaManipulator[i].typeJoint==BOX)
			dMassSetBox(&jointParaManipulator[i].massBody,density,jointParaManipulator[i].paraManipulatorBox.x,jointParaManipulator[i].paraManipulatorBox.y,jointParaManipulator[i].paraManipulatorBox.z);
	}
	

	//set base body paras;
	bodyparaBase.typeJoint=BOX;
	bodyparaBase.paraManipulatorCylinder.angle=0;//no use
	bodyparaBase.paraManipulatorCylinder.length=0.1;
	bodyparaBase.paraManipulatorCylinder.r=1;

	//set stops
	for(i=0;i<NUMOFLEGJOINTS;i++)
	{
		stopparaManipulator[i].lowstop=0;
		stopparaManipulator[i].histop=0;
		stopparaManipulator[i].bounce=0.0001;
		stopparaManipulator[i].cfm=0;
	}
	return;
}

//非常重要，必须修改该函数，特别是改变set stop。的lowstop和histop，否则机器人动不了
void CRobotControl::ChangePara()
{

	//set stops
	stopparaManipulator[0].lowstop=-360*Pi/180;
	stopparaManipulator[0].histop=360*Pi/180;

	stopparaManipulator[1].lowstop=-360*Pi/180;
	stopparaManipulator[1].histop=360*Pi/180;

	stopparaManipulator[2].lowstop=-360*Pi/180;
	stopparaManipulator[2].histop=360*Pi/180;

	stopparaManipulator[3].lowstop=-0;
	stopparaManipulator[3].histop=0;
	
	stopparaManipulator[4].lowstop=-360*Pi/180;
	stopparaManipulator[4].histop=360*Pi/180;

	stopparaManipulator[5].lowstop=-360*Pi/180;
	stopparaManipulator[5].histop=360*Pi/180;

	stopparaManipulator[6].lowstop=-360*Pi/180;
	stopparaManipulator[6].histop=360*Pi/180;

	for(int i=0;i<NUMOFLEGJOINTS;i++)
	{
		stopparaManipulator[i].bounce=0;
		stopparaManipulator[i].cfm=0;
	}	
	return;
}

CRobotControl::~CRobotControl(void)
{
}
void CRobotControl::init()
{	
	for(int i=0;i<6;i++)
		CommandManipulator.force[i]=CommandManipulator.vel[i]=CommandManipulator.angle[i]=NOCOMMAND;
	return;
}
void CRobotControl::GetManipulatorState(SManipulatorState myState)
{
	CurState=myState;
	return;
}

void CRobotControl::GetGivenState(SGivenState myGiven)
{
	GivenState=myGiven;
	return;
}


void CRobotControl::Controller(int ControllerType)
{
		switch(ControllerType)/////////////////////////////////////222222222222222222222222222222222222222222222222
		{
			case ANGLE:
					Angle_Controller();
					break;
			case VELOCITY:
					Vel_Controller();
					break;
			case FORCE:
					Force_Controller();
					break;
			default:
					break;
		}
}


void CRobotControl::Vel_Controller()
{
	double KP[6]={100,100,100,100,100,100};
	double Deviation=0;
	double Precision=0.02;
	for(int i=0;i<6;++i)
	{
		Deviation=GivenState.angle[i]-CurState.angle[i];
		if(fabs(Deviation)<Precision)
			Deviation=0;
		CommandManipulator.vel[i]=KP[i]*Deviation;
	}
	out1<<CurState.angle[0]<<endl;
	out2<<CurState.angle[1]<<endl;
	out3<<CurState.angle[2]<<endl;
	out4<<CurState.angle[3]<<endl;
	out5<<CurState.angle[4]<<endl;
	out6<<CurState.angle[5]<<endl;
	out13<<GivenState.angle[0]<<endl;
	out14<<GivenState.angle[0]<<endl;
	out15<<GivenState.angle[0]<<endl;
	out16<<GivenState.angle[0]<<endl;
	out17<<GivenState.angle[0]<<endl;
	out18<<GivenState.angle[0]<<endl;
}
void CRobotControl::Angle_Controller()
{
}
void CRobotControl::Force_Controller()
{
}