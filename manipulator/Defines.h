#pragma once

#define RATE 0.001
#define MaxForce 3000

#define CYLINDER 1
#define BOX 2

#define FORCE 1
#define VELOCITY 2
#define ANGLE 3



#define NOCOMMAND -200000

#define XX 0
#define YY 1
#define ZZ 2

#define NUMOFLEGJOINTS 7

#define Pi  3.14159265358979323846264338327950288419716939937510

typedef struct _SBoxRect{
	double x,y,z,angle;
}SBoxRect;
typedef struct _SCylinderRect{
	double r,length,angle;
}SCylinderRect;

typedef struct _SBodyPara{
	SCylinderRect paraManipulatorCylinder;
	SBoxRect paraManipulatorBox;
	int typeJoint;
}SBodyPara;// parameters of a body.

typedef struct _SJointPara{
	SCylinderRect paraManipulatorCylinder;
	SBoxRect paraManipulatorBox;
	int typeJoint;
	int modeAxis;
	dMass massBody;
} SJointPara;

typedef struct _dManipulatorJoint{	
	SJointPara paraJoint;
	dBodyID bodyJoint;
	dGeomID geomJoint;
	dJointID joint;
} dManipulatorJoint; //simple joint, only ball and link. the axis is shown as a ball. 

typedef struct _dManipulatorBase{
	SBodyPara paraBase;
	dGeomID geomBase;
}dManipulatorBase; 

typedef struct _SPoint{
	double x,y,z;
} SPoint;


typedef struct _SPosition{
	dVector3 pos;
	dMatrix3 rot;
} SPosition;






typedef struct _SManipulatorState{
	double speed[6];
	double angle[6];
} SManipulatorState;


typedef struct _SGivenState{
	double speed[6];
	double angle[6];
} SGivenState;

typedef struct _SCommand{
	double vel[6];
	double force[6];
	double angle[6];
} SCommand;






typedef struct _SStopPara{
	double lowstop;
	double histop;
	double bounce;
	double cfm;
} SStopPara;



typedef struct _SColor{
	float R,G,B;
} SColor;





typedef struct _SMessage_s2c{
	int angle[6];
	int speed[6];
} SMessage_s2c;
typedef struct _SMessage_c2s{
	int speed_com[6];
} SMessage_c2s;