#pragma once

#include <ode/ode.h>
#include <iostream>
#include <math.h>
#include <texturepath.h>
#include <drawstuff/drawstuff.h>
#include "Defines.h"

class CManipulator
{
public://constructor
	double BORDER;
	CManipulator(void);
	~CManipulator(void);
	void SetParas(SJointPara myJointPara[NUMOFLEGJOINTS],SBodyPara myManipulatorBasePara,SStopPara stopPara[NUMOFLEGJOINTS],double border);
	void CreateManipulator(dWorldID world,dSpaceID space);
	void drawBox (dGeomID id, float R, float G, float B);
	void drawCylinder (dGeomID id, float R, float G, float B);
	void drawCylinderWithBall (dGeomID idGeom,float R, float G, float B);
	void drawManipulator ();	
	void CreateJoint (SPoint &myptAnchor,SJointPara paraJoint,double Border,dBodyID &mybodyLastJoint,dBodyID &mybodyJoint,dGeomID &mygeomLink,dJointID &myJoint,dWorldID myworld,dSpaceID myspace);
	void SetStop(dJointID myJoint,SStopPara stopPara);

	dManipulatorJoint jointManipulator[NUMOFLEGJOINTS];
	dManipulatorBase baseManipulator;
	SStopPara stopparaJoint[NUMOFLEGJOINTS];
	SColor colorManipulator;
	SPosition positionLink[NUMOFLEGJOINTS];
private:
	void init();//initialize the variables	

};
