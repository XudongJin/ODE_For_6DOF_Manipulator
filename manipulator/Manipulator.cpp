#include "Manipulator.h"

CManipulator::CManipulator(void)
{
	init();
}

CManipulator::~CManipulator(void)
{
}
void CManipulator::drawManipulator ()
{	
	float R,G,B;
	R=colorManipulator.R;
	G=colorManipulator.G;
	B=colorManipulator.B;
	if(baseManipulator.paraBase.typeJoint==CYLINDER)
		drawCylinder(baseManipulator.geomBase,B,G,R);
	else
		drawBox(baseManipulator.geomBase,B,G,R);
	int i;
	for(i=0;i<NUMOFLEGJOINTS;i++)
		if(jointManipulator[i].paraJoint.typeJoint==CYLINDER)
			drawCylinderWithBall(jointManipulator[i].geomJoint,R,G,B);
		else
			drawBox(jointManipulator[i].geomJoint,G,R,B);
	return;
}

void CManipulator::drawBox (dGeomID id, float R, float G, float B)
{
  if (!id)
    return;

  const dReal *pos = dGeomGetPosition (id);
  const dReal *rot = dGeomGetRotation (id);
  dsSetColor (R,G,B);

  dVector3 l;
  dGeomBoxGetLengths (id, l);
  dsDrawBoxD (pos, rot, l);
}
void CManipulator::drawCylinder (dGeomID id, float R, float G, float B)
{
  if (!id)
    return;

  const dReal *pos = dGeomGetPosition (id);
  const dReal *rot = dGeomGetRotation (id);
  dsSetColor (R,G,B);

  dReal l,r;
  dGeomCylinderGetParams(id,&r,&l);
  dsDrawCylinderD(pos, rot, (float)l,(float)r);
}


void CManipulator::drawCylinderWithBall (dGeomID idGeom, float R, float G, float B)
{
  if (!idGeom)
    return;  
  dVector3 pos1;
  const dReal *pos = dGeomGetPosition (idGeom);
  const dReal *rot = dGeomGetRotation (idGeom);
  dsSetColor (R,G,B);
  dReal l,r;
  dGeomCylinderGetParams(idGeom,&r,&l);
  dsDrawCylinderD(pos, rot, (float)l,(float)r);
  pos1[0]=pos[0]+(l/2.0)*rot[2]*-1;
  pos1[1]=pos[1]+(l/2.0)*rot[6]*-1;
  pos1[2]=pos[2]+(l/2.0)*rot[10]*-1;
  dsDrawSphereD(pos1,rot,float(r+BORDER));
  pos1[0]=pos[0]+(l/2.0)*rot[2];
  pos1[1]=pos[1]+(l/2.0)*rot[6];
  pos1[2]=pos[2]+(l/2.0)*rot[10];
  dsDrawSphereD(pos1,rot,float(r+BORDER));
}

void CManipulator::SetParas(SJointPara myJointPara[NUMOFLEGJOINTS],SBodyPara myManipulatorBasePara,SStopPara stopPara[NUMOFLEGJOINTS],double border)
{
	for(int i=0;i<NUMOFLEGJOINTS;i++)
	{
		jointManipulator[i].paraJoint=myJointPara[i];
		stopparaJoint[i]=stopPara[i];
	}
	baseManipulator.paraBase=myManipulatorBasePara;	
	BORDER=border;
	return;
}
void CManipulator::CreateJoint (SPoint &myptAnchor,SJointPara paraJoint,double Border,dBodyID &mybodyLastJoint,dBodyID &mybodyJoint,dGeomID &mygeomLink,dJointID &myJoint,dWorldID myworld,dSpaceID myspace)
{
	SPoint ptBody;
	if(!mybodyLastJoint)//如果是第一个关节，上个body=NULL
	{
		if(paraJoint.typeJoint==CYLINDER)
			mygeomLink=dCreateCylinder(myspace,paraJoint.paraManipulatorCylinder.r,paraJoint.paraManipulatorCylinder.length);
		else 
			mygeomLink=dCreateBox(myspace,paraJoint.paraManipulatorBox.x,paraJoint.paraManipulatorBox.y,paraJoint.paraManipulatorBox.z);
		mybodyJoint=dBodyCreate(myworld);
		dGeomSetBody(mygeomLink,mybodyJoint);
		dBodySetMass(mybodyJoint,&paraJoint.massBody);
		ptBody.x=0;
		if(paraJoint.typeJoint==CYLINDER)
		{
			ptBody.y=myptAnchor.y+paraJoint.paraManipulatorCylinder.length*sin(paraJoint.paraManipulatorCylinder.angle)/2.0;
			ptBody.z=myptAnchor.z+paraJoint.paraManipulatorCylinder.length*cos(paraJoint.paraManipulatorCylinder.angle)/2.0;
		}
		else
		{
			ptBody.y=myptAnchor.y+paraJoint.paraManipulatorBox.z*sin(paraJoint.paraManipulatorBox.angle)/2.0;
			ptBody.z=myptAnchor.z+paraJoint.paraManipulatorBox.z*cos(paraJoint.paraManipulatorBox.angle)/2.0;
		
		}
		dBodySetPosition(mybodyJoint,ptBody.x,ptBody.y,ptBody.z);

		dMatrix3 rot;
		if(paraJoint.typeJoint==CYLINDER)
			dRFromAxisAndAngle(rot,1,0,0,paraJoint.paraManipulatorCylinder.angle);
		else
			dRFromAxisAndAngle(rot,1,0,0,paraJoint.paraManipulatorBox.angle);
		dBodySetRotation(mybodyJoint,rot);
		
		myJoint=dJointCreateHinge(myworld,0);//create Joint
		dJointAttach(myJoint,0,mybodyJoint);
		dJointSetHingeAnchor(myJoint,myptAnchor.x,myptAnchor.y,myptAnchor.z);

		if(paraJoint.modeAxis==XX)
			dJointSetHingeAxis(myJoint,1,0,0);
		else if(paraJoint.modeAxis==YY)
			dJointSetHingeAxis(myJoint,0,1,0);
		else
			dJointSetHingeAxis(myJoint,0,0,1);

		if(paraJoint.typeJoint==CYLINDER)
		{
			myptAnchor.y=ptBody.y+paraJoint.paraManipulatorCylinder.length*sin(paraJoint.paraManipulatorCylinder.angle)/2.0;
			myptAnchor.z=ptBody.z+paraJoint.paraManipulatorCylinder.length*cos(paraJoint.paraManipulatorCylinder.angle)/2.0;
		}
		else
		{
			myptAnchor.y=ptBody.y+paraJoint.paraManipulatorBox.z*sin(paraJoint.paraManipulatorBox.angle)/2.0;
			myptAnchor.z=ptBody.z+paraJoint.paraManipulatorBox.z*cos(paraJoint.paraManipulatorBox.angle)/2.0;
		}
	}
	else
	{
		if(paraJoint.typeJoint==CYLINDER)
			mygeomLink=dCreateCylinder(myspace,paraJoint.paraManipulatorCylinder.r,paraJoint.paraManipulatorCylinder.length);
		else 
			mygeomLink=dCreateBox(myspace,paraJoint.paraManipulatorBox.x,paraJoint.paraManipulatorBox.y,paraJoint.paraManipulatorBox.z);
		mybodyJoint=dBodyCreate(myworld);
		dGeomSetBody(mygeomLink,mybodyJoint);
		dBodySetMass(mybodyJoint,&paraJoint.massBody);
		ptBody.x=0;
		if(paraJoint.typeJoint==CYLINDER)
		{
			ptBody.y=myptAnchor.y+paraJoint.paraManipulatorCylinder.length*sin(paraJoint.paraManipulatorCylinder.angle)/2.0;
			ptBody.z=myptAnchor.z+paraJoint.paraManipulatorCylinder.length*cos(paraJoint.paraManipulatorCylinder.angle)/2.0;
		}
		else
		{
			ptBody.y=myptAnchor.y+paraJoint.paraManipulatorBox.z*sin(paraJoint.paraManipulatorBox.angle)/2.0;
			ptBody.z=myptAnchor.z+paraJoint.paraManipulatorBox.z*cos(paraJoint.paraManipulatorBox.angle)/2.0;
		}
		dBodySetPosition(mybodyJoint,ptBody.x,ptBody.y,ptBody.z);
		dMatrix3 rot;
		if(paraJoint.typeJoint==CYLINDER)
			dRFromAxisAndAngle(rot,1,0,0,paraJoint.paraManipulatorCylinder.angle);
		else
			dRFromAxisAndAngle(rot,1,0,0,paraJoint.paraManipulatorBox.angle);
		dBodySetRotation(mybodyJoint,rot);
		myJoint=dJointCreateHinge(myworld,0);//create Joint
		dJointAttach(myJoint,mybodyLastJoint,mybodyJoint);
		dJointSetHingeAnchor(myJoint,myptAnchor.x,myptAnchor.y,myptAnchor.z);

		if(paraJoint.modeAxis==XX)
			dJointSetHingeAxis(myJoint,1,0,0);
		else if(paraJoint.modeAxis==YY)
			dJointSetHingeAxis(myJoint,0,-1,0);
		else
			dJointSetHingeAxis(myJoint,0,0,1);

		if(paraJoint.typeJoint==CYLINDER)
		{
			myptAnchor.y=ptBody.y+paraJoint.paraManipulatorCylinder.length*sin(paraJoint.paraManipulatorCylinder.angle)/2.0;
			myptAnchor.z=ptBody.z+paraJoint.paraManipulatorCylinder.length*cos(paraJoint.paraManipulatorCylinder.angle)/2.0;
		}
		else
		{
			myptAnchor.y=ptBody.y+paraJoint.paraManipulatorBox.z*sin(paraJoint.paraManipulatorBox.angle)/2.0;
			myptAnchor.z=ptBody.z+paraJoint.paraManipulatorBox.z*cos(paraJoint.paraManipulatorBox.angle)/2.0;
		}
	}
	mybodyLastJoint=mybodyJoint;	
	return;
}

void CManipulator::init()
{	
	BORDER=0.01;
	colorManipulator.B=(float)1.0;
	colorManipulator.R=(float)0.5;
	colorManipulator.G=(float)0.3;
	int i;
	double density=1;////////////////////////////////////
	i=0;
	if(i<NUMOFLEGJOINTS)
	{
		jointManipulator[i].paraJoint.modeAxis=ZZ;
		jointManipulator[i].paraJoint.typeJoint=BOX;
		jointManipulator[i].paraJoint.paraManipulatorBox.angle=0;
		jointManipulator[i].paraJoint.paraManipulatorBox.x=0.4;
		jointManipulator[i].paraJoint.paraManipulatorBox.y=0.4;
		jointManipulator[i].paraJoint.paraManipulatorBox.z=0.585;
		//		jointManipulator[i].paraJoint.typeJoint=CYLINDER;
		//jointManipulator[i].paraJoint.paraManipulatorCylinder.angle=0;
		//jointManipulator[i].paraJoint.paraManipulatorCylinder.length=0.585;////////////修改过
		//jointManipulator[i].paraJoint.paraManipulatorCylinder.r=0.2;////////////
		if(jointManipulator[i].paraJoint.typeJoint==CYLINDER)
			dMassSetCylinder(&jointManipulator[i].paraJoint.massBody,density,ZZ,jointManipulator[i].paraJoint.paraManipulatorCylinder.r,jointManipulator[i].paraJoint.paraManipulatorCylinder.length);
		else if(jointManipulator[i].paraJoint.typeJoint==BOX)
			dMassSetBox(&jointManipulator[i].paraJoint.massBody,density,jointManipulator[i].paraJoint.paraManipulatorBox.x,jointManipulator[i].paraJoint.paraManipulatorBox.y,jointManipulator[i].paraJoint.paraManipulatorBox.z);
	}

	i=1;
	if(i<NUMOFLEGJOINTS)
	{
		jointManipulator[i].paraJoint.modeAxis=XX;
		jointManipulator[i].paraJoint.typeJoint=BOX;
		jointManipulator[i].paraJoint.paraManipulatorBox.angle=0;
		jointManipulator[i].paraJoint.paraManipulatorBox.x=0.4;
		jointManipulator[i].paraJoint.paraManipulatorBox.y=0.4;
		jointManipulator[i].paraJoint.paraManipulatorBox.z=0.6;
		if(jointManipulator[i].paraJoint.typeJoint==CYLINDER)
			dMassSetCylinder(&jointManipulator[i].paraJoint.massBody,density,ZZ,jointManipulator[i].paraJoint.paraManipulatorCylinder.r,jointManipulator[i].paraJoint.paraManipulatorCylinder.length);
		else if(jointManipulator[i].paraJoint.typeJoint==BOX)
			dMassSetBox(&jointManipulator[i].paraJoint.massBody,density,jointManipulator[i].paraJoint.paraManipulatorBox.x,jointManipulator[i].paraJoint.paraManipulatorBox.y,jointManipulator[i].paraJoint.paraManipulatorBox.z);
	}

	i=2;
	if(i<NUMOFLEGJOINTS)
	{
		jointManipulator[i].paraJoint.modeAxis=XX;
		jointManipulator[i].paraJoint.typeJoint=BOX;
		jointManipulator[i].paraJoint.paraManipulatorBox.angle=0;
		jointManipulator[i].paraJoint.paraManipulatorBox.x=0.2;
		jointManipulator[i].paraJoint.paraManipulatorBox.y=0.2;
		jointManipulator[i].paraJoint.paraManipulatorBox.z=0.115;
		//		jointManipulator[i].paraJoint.typeJoint=CYLINDER;
		//jointManipulator[i].paraJoint.paraManipulatorCylinder.angle=0;
		//jointManipulator[i].paraJoint.paraManipulatorCylinder.length=0.115;
		//jointManipulator[i].paraJoint.paraManipulatorCylinder.r=0.1;
		if(jointManipulator[i].paraJoint.typeJoint==CYLINDER)
			dMassSetCylinder(&jointManipulator[i].paraJoint.massBody,density,ZZ,jointManipulator[i].paraJoint.paraManipulatorCylinder.r,jointManipulator[i].paraJoint.paraManipulatorCylinder.length);
		else if(jointManipulator[i].paraJoint.typeJoint==BOX)
			dMassSetBox(&jointManipulator[i].paraJoint.massBody,density,jointManipulator[i].paraJoint.paraManipulatorBox.x,jointManipulator[i].paraJoint.paraManipulatorBox.y,jointManipulator[i].paraJoint.paraManipulatorBox.z);
	}

	i=3;
	if(i<NUMOFLEGJOINTS)
	{
		jointManipulator[i].paraJoint.modeAxis=XX;
		jointManipulator[i].paraJoint.typeJoint=BOX;
		jointManipulator[i].paraJoint.paraManipulatorBox.angle=Pi/2;
		jointManipulator[i].paraJoint.paraManipulatorBox.x=0.2;
		jointManipulator[i].paraJoint.paraManipulatorBox.y=0.2;
		jointManipulator[i].paraJoint.paraManipulatorBox.z=0.385;
		if(jointManipulator[i].paraJoint.typeJoint==CYLINDER)
			dMassSetCylinder(&jointManipulator[i].paraJoint.massBody,density,ZZ,jointManipulator[i].paraJoint.paraManipulatorCylinder.r,jointManipulator[i].paraJoint.paraManipulatorCylinder.length);
		else if(jointManipulator[i].paraJoint.typeJoint==BOX)
			dMassSetBox(&jointManipulator[i].paraJoint.massBody,density,jointManipulator[i].paraJoint.paraManipulatorBox.x,jointManipulator[i].paraJoint.paraManipulatorBox.y,jointManipulator[i].paraJoint.paraManipulatorBox.z);
	}

	i=4;
	if(i<NUMOFLEGJOINTS)
	{
		jointManipulator[i].paraJoint.modeAxis=YY;
		jointManipulator[i].paraJoint.typeJoint=BOX;
		jointManipulator[i].paraJoint.paraManipulatorBox.angle=Pi/2;
		jointManipulator[i].paraJoint.paraManipulatorBox.x=0.2;
		jointManipulator[i].paraJoint.paraManipulatorBox.y=0.2;
		jointManipulator[i].paraJoint.paraManipulatorBox.z=0.385;
		if(jointManipulator[i].paraJoint.typeJoint==CYLINDER)
			dMassSetCylinder(&jointManipulator[i].paraJoint.massBody,density,ZZ,jointManipulator[i].paraJoint.paraManipulatorCylinder.r,jointManipulator[i].paraJoint.paraManipulatorCylinder.length);
		else if(jointManipulator[i].paraJoint.typeJoint==BOX)
			dMassSetBox(&jointManipulator[i].paraJoint.massBody,density,jointManipulator[i].paraJoint.paraManipulatorBox.x,jointManipulator[i].paraJoint.paraManipulatorBox.y,jointManipulator[i].paraJoint.paraManipulatorBox.z);
	}

	i=5;
	if(i<NUMOFLEGJOINTS)
	{
		jointManipulator[i].paraJoint.modeAxis=XX;
		jointManipulator[i].paraJoint.typeJoint=BOX;
		jointManipulator[i].paraJoint.paraManipulatorBox.angle=Pi/2;
		jointManipulator[i].paraJoint.paraManipulatorBox.x=0.2;
		jointManipulator[i].paraJoint.paraManipulatorBox.y=0.2;
		jointManipulator[i].paraJoint.paraManipulatorBox.z=0.1;

		//jointManipulator[i].paraJoint.paraManipulatorCylinder.angle=Pi/2;
		//jointManipulator[i].paraJoint.paraManipulatorCylinder.length=0.1;
		//jointManipulator[i].paraJoint.paraManipulatorCylinder.r=0.1;
		if(jointManipulator[i].paraJoint.typeJoint==CYLINDER)
			dMassSetCylinder(&jointManipulator[i].paraJoint.massBody,density,ZZ,jointManipulator[i].paraJoint.paraManipulatorCylinder.r,jointManipulator[i].paraJoint.paraManipulatorCylinder.length);
		else if(jointManipulator[i].paraJoint.typeJoint==BOX)
			dMassSetBox(&jointManipulator[i].paraJoint.massBody,density,jointManipulator[i].paraJoint.paraManipulatorBox.x,jointManipulator[i].paraJoint.paraManipulatorBox.y,jointManipulator[i].paraJoint.paraManipulatorBox.z);
	}

	i=6;
	if(i<NUMOFLEGJOINTS)
	{
		jointManipulator[i].paraJoint.modeAxis=YY;
		jointManipulator[i].paraJoint.typeJoint=BOX;
		jointManipulator[i].paraJoint.paraManipulatorBox.angle=Pi/2;
		jointManipulator[i].paraJoint.paraManipulatorBox.x=0.1;
		jointManipulator[i].paraJoint.paraManipulatorBox.y=0.1;
		jointManipulator[i].paraJoint.paraManipulatorBox.z=0.6;
		if(jointManipulator[i].paraJoint.typeJoint==CYLINDER)
			dMassSetCylinder(&jointManipulator[i].paraJoint.massBody,density,ZZ,jointManipulator[i].paraJoint.paraManipulatorCylinder.r,jointManipulator[i].paraJoint.paraManipulatorCylinder.length);
		else if(jointManipulator[i].paraJoint.typeJoint==BOX)
			dMassSetBox(&jointManipulator[i].paraJoint.massBody,density,jointManipulator[i].paraJoint.paraManipulatorBox.x,jointManipulator[i].paraJoint.paraManipulatorBox.y,jointManipulator[i].paraJoint.paraManipulatorBox.z);
	}
	baseManipulator.paraBase.typeJoint=BOX;
	baseManipulator.paraBase.paraManipulatorCylinder.angle=0;
	baseManipulator.paraBase.paraManipulatorCylinder.length=0.1;
	baseManipulator.paraBase.paraManipulatorCylinder.r=1;

	for(i=0;i<NUMOFLEGJOINTS;i++)
	{
		stopparaJoint[i].lowstop=0;
		stopparaJoint[i].histop=0;
		stopparaJoint[i].bounce=1;
		stopparaJoint[i].cfm=0;
	}
}

void CManipulator::SetStop(dJointID myJoint,SStopPara stopPara)
{
	dJointSetHingeParam(myJoint,dParamLoStop,stopPara.lowstop);//-0.5*Pi);
	dJointSetHingeParam(myJoint,dParamHiStop,stopPara.histop);//0.5*Pi);
	dJointSetHingeParam(myJoint,dParamBounce,stopPara.bounce);//无弹性
	dJointSetHingeParam(myJoint,dParamStopCFM,stopPara.cfm);
	return;
}
void CManipulator::CreateManipulator(dWorldID world,dSpaceID space)
{
	if(baseManipulator.paraBase.typeJoint=CYLINDER)
		baseManipulator.geomBase=dCreateCylinder(space,baseManipulator.paraBase.paraManipulatorCylinder.r,baseManipulator.paraBase.paraManipulatorCylinder.length);
	else
		baseManipulator.geomBase=dCreateBox(space,baseManipulator.paraBase.paraManipulatorBox.x,baseManipulator.paraBase.paraManipulatorBox.y,baseManipulator.paraBase.paraManipulatorBox.z);
	if(baseManipulator.paraBase.typeJoint=CYLINDER)
		dGeomSetPosition(baseManipulator.geomBase,0,0,baseManipulator.paraBase.paraManipulatorCylinder.length/2.0);
	else
		dGeomSetPosition(baseManipulator.geomBase,0,0,baseManipulator.paraBase.paraManipulatorBox.z/2.0);

	int i;
	SPoint tempptAnchor;
	dBodyID tempbodyLast=NULL;
	tempptAnchor.x=tempptAnchor.y=0;
	if(baseManipulator.paraBase.typeJoint=CYLINDER)
		tempptAnchor.z=baseManipulator.paraBase.paraManipulatorCylinder.length;
	else
		tempptAnchor.z=baseManipulator.paraBase.paraManipulatorBox.z;
	//创建第一轴
	CreateJoint(tempptAnchor,jointManipulator[0].paraJoint,BORDER,tempbodyLast,
			jointManipulator[0].bodyJoint,jointManipulator[0].geomJoint,jointManipulator[0].joint,world,space);
	SetStop(jointManipulator[0].joint,stopparaJoint[0]);

	//创建第二轴	
	if(jointManipulator[0].paraJoint.typeJoint==CYLINDER)
	{
		tempptAnchor.x=tempptAnchor.x;
		tempptAnchor.z=tempptAnchor.z;
		tempptAnchor.y=tempptAnchor.y+jointManipulator[0].paraJoint.paraManipulatorCylinder.r;
	}
	else
	{
		tempptAnchor.x=tempptAnchor.x;
		tempptAnchor.z=tempptAnchor.z;
		tempptAnchor.y=tempptAnchor.y+jointManipulator[0].paraJoint.paraManipulatorBox.y/2;
	}
	CreateJoint(tempptAnchor,jointManipulator[1].paraJoint,BORDER,tempbodyLast,
			jointManipulator[1].bodyJoint,jointManipulator[1].geomJoint,jointManipulator[1].joint,world,space);
	SetStop(jointManipulator[1].joint,stopparaJoint[1]);
	
	//创建其他轴
	for( i=2;i<NUMOFLEGJOINTS;i++)
	{
		CreateJoint(tempptAnchor,
			jointManipulator[i].paraJoint,BORDER,
			tempbodyLast,
			jointManipulator[i].bodyJoint,
			jointManipulator[i].geomJoint,
			jointManipulator[i].joint,
			world,space);
		SetStop(jointManipulator[i].joint,stopparaJoint[i]);
	}

	return;
}
