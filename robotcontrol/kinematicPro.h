
#pragma once

#include <memory.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdlib.h> 
#include <stdio.h>  
#include  <conio.h>   
#include  <time.h> 
#include <iostream>
#include <fstream>

using namespace std;


#define RATE 0.001
#define PI  3.1415926
#define RANK 4
#define INFIN 30
#define PRE 1e-2
#define SCALE 0.01
#define W1  50
#define W2  25
#define W3  10
#define W4   5
#define W5   2
#define W6   1
#define BUFLEN 30000
#define INF1  100
#define INF2  200
#define FORMAT setiosflags(ios::fixed)<<setiosflags(ios::left)<<setprecision(6)<<setw(19)
#define UNIT  PI/180
#define simstep 0.0002
#define JMETHOD 0
#define CMETHOD 1
#define ARCMETHOD 2


ofstream kout2("data\\record.txt");
ofstream kout1("data\\JTRAJ.txt");
ofstream kout3("data\\UU.txt");
ofstream kout4("data\\TH.txt");
ofstream PX("data\\PX.txt");
ofstream PY("data\\PY.txt");
ofstream PZ("data\\PZ.txt");

struct Lever_Para
{
	int    num;
	double length;
	double torsion_angle;
	double offset;
	double joint_angle;
	double max;
	double min;
};

struct   nump
{
	double num;
	int p;
};





class dT44
{
	
public:
	double  mat[4][4];

	void H2Trans(double a[3])
	{	
		a[0]=mat[0][3];
		a[1]=mat[1][3];
		a[2]=mat[2][3];
	}

	void TR2Q(double Q[4])
	 {
		 
		 double trace=mat[0][0]+mat[1][1]+mat[2][2]; 
		 double qs = sqrt(trace+1)/2.0;
         double kx = mat[2][1] - mat[1][2];    // Oz - Ay
         double ky = mat[0][2] - mat[2][0];   // Ax - Nz
         double kz = mat[1][0] - mat[0][1];  // Ny - Ox
		 double kx1,ky1,kz1,nm,s;
		 bool  add;
		 if ((mat[0][0] >= mat[1][1]) && (mat[0][0] >= mat[2][2]))
		 {
                    kx1 = mat[0][0] - mat[1][1] - mat[2][2] + 1; 
                     ky1 = mat[1][0] + mat[0][1];          
                        kz1 =mat[2][0] + mat[0][2];          
                            add = (kx >= 0);
		 }
         else if ( mat[1][1] >= mat[2][2])
				{
                     kx1 = mat[1][0]+mat[0][1];          
                        ky1 = mat[1][1]  -mat[0][0] - mat[2][2] + 1; 
                            kz1 = mat[2][1] + mat[1][2];          
                                add = (ky >= 0);
				 }
			else
				{
                    kx1 = mat[2][0] + mat[0][2];          
                     ky1 = mat[2][1]+ mat[1][2];          
                        kz1 = mat[2][2] - mat[0][0] - mat[1][1] + 1; 
                            add = (kz >= 0);
				}
          
                if(add==true)
				{
                    kx = kx + kx1;
				    ky = ky + ky1;
					kz = kz + kz1;
				}
                else
				{
                    kx = kx - kx1;
                    ky = ky - ky1;
                    kz = kz - kz1;
				}
				nm =fabs(kx*kx)+fabs(ky*ky)+fabs(kz*kz);
				nm = sqrt(nm);
                if(nm == 0)
				{
                    Q[0]=1;
					Q[1]=0;	Q[2]=0;	Q[3]=0;
				}
                else
				{
					s = sqrt(1 - qs*qs) / nm;
					Q[0]=qs;
					Q[1]=s*kx;	Q[2]=s*ky;	Q[3]=s*kz;
				}           
	 }


	dT44()
	{
		int m,n;
		for(m=0;m<4;++m)
		{
			for(n=0;n<4;++n)
			mat[m][n]=0;
		}
		mat[3][3]=1;
	}



	dT44(double a[][4])
	{

		int m,n;
		for(m=0;m<4;++m)
		{
			for(n=0;n<4;++n)
			mat[m][n]=a[m][n];
		}
	}

	dT44(dT44 & a)
	{

		int m,n;
		for(m=0;m<4;++m)
		{
			for(n=0;n<4;++n)
			mat[m][n]=a.mat[m][n];
		}
	}

	void update(Lever_Para t)
	{
	int i,j;
	double temp1[4][4] = 
		{ {cos(t.joint_angle), 0, -sin(t.joint_angle), t.length*cos(t.joint_angle)},
		  {sin(t.joint_angle), 0, cos(t.joint_angle),  t.length*sin(t.joint_angle)},
		  {0,        -1, 0,          t.offset},
		  {0,         0, 0,          1}}; 
	     double temp2[4][4] = 
	     { {sin(t.joint_angle),  cos(t.joint_angle), 0, t.length*sin(t.joint_angle)},
		   {-cos(t.joint_angle), sin(t.joint_angle), 0, -t.length*cos(t.joint_angle)},
		   {0,          0,         1, 0},
		   {0,          0,         0, 1}};
	      double temp3[4][4] = 
		 { {cos(t.joint_angle), 0, -sin(t.joint_angle),  t.length*cos(t.joint_angle)-t.offset*sin(t.joint_angle)},
			{sin(t.joint_angle), 0, cos(t.joint_angle),   t.length*sin(t.joint_angle)+t.offset*cos(t.joint_angle)},
			{0,         -1,0,           0},
			{0,         0, 0,           1}};
	      double temp4[4][4] =
		 { {cos(t.joint_angle), 0, sin(t.joint_angle),  0},
			{sin(t.joint_angle), 0, -cos(t.joint_angle), 0},
			{0,         1, 0,          0},
			{0,         0, 0,          1}};
	     double temp5[4][4] =
		 { {cos(t.joint_angle), 0, -sin(t.joint_angle), 0},
		   {sin(t.joint_angle), 0, cos(t.joint_angle),  0},
			{0,         -1,0,          0},
			{0,         0, 0,          1}};
	     double temp6[4][4] =
		{ {cos(t.joint_angle), -sin(t.joint_angle), 0, 0},
		{sin(t.joint_angle), cos(t.joint_angle),  0, 0},
		{0,         0,          1, t.offset},
		{0,         0,          0, 1}};
	switch(t.num)
	{   case 1:   
	for( i=0;i<4;++i){
          for(j=0;j<4;++j)
			  mat[i][j]=temp1[i][j];} break;
	
         case 2:
			 for( i=0;i<4;++i){
     for(j=0;j<4;++j)
		 mat[i][j]=temp2[i][j];}break;
		 case 3:
			 for( i=0;i<4;++i){
     for(j=0;j<4;++j)
		 mat[i][j]=temp3[i][j];}break;
		 case 4:
			 for( i=0;i<4;++i){
     for(j=0;j<4;++j)
		 mat[i][j]=temp4[i][j];}break;
		 case 5:
			 for( i=0;i<4;++i){
     for(j=0;j<4;++j)
		 mat[i][j]=temp5[i][j];}break;
		 case 6:
			 for( i=0;i<4;++i){
     for(j=0;j<4;++j)
		 mat[i][j]=temp6[i][j];}break;
	}

	
	}
	
	bool dT44::operator != (dT44 & b)
	{
		int m,n;
	    for(m=0;m<4;++m)
	   {
		  for(n=0;n<4;++n)
		  {
		 	 if(fabs(mat[m][n]-b.mat[m][n])>PRE)
				 return true;
		  }
	   }
	    return false;
	}

	void TransMode()
	{
		int n,k;
		double  temp[4][4];
		for(n=0;n<4;++n)
		  {
			 for(k=0;k<4;++k)
		 	 temp[n][k]=mat[n][k];
		  }
		for(n=0;n<4;++n)
		  {
			 for(k=0;k<4;++k)
		 	 mat[k][n]=temp[n][k];
		  }

	}
	friend  dT44 operator  * (dT44 & a,dT44 & b)
	{
		double temp[4][4]={0};
		int m,n,k;
	    for(m=0;m<4;++m)
	   {
		  for(n=0;n<4;++n)
		  {
			  for(k=0;k<4;++k)
		 	 temp[m][n]+=a.mat[m][k]*b.mat[k][n];
		  }
	   }
	    return dT44(temp);
	}

	void show( )
	{
		
		kout2<<"Nx=";
		kout2<<FORMAT<<mat[0][0];
		kout2<<"Ox=";
		kout2<<FORMAT<<mat[0][1];
		kout2<<"Ax=";
		kout2<<FORMAT<<mat[0][2];
		kout2<<"Px=";
		kout2<<FORMAT<<mat[0][3];
		kout2<<endl;
		kout2<<"Ny=";
		kout2<<FORMAT<<mat[1][0];
		kout2<<"Oy=";
		kout2<<FORMAT<<mat[1][1];
		kout2<<"Ay=";
		kout2<<FORMAT<<mat[1][2];
		kout2<<"Py=";
		kout2<<FORMAT<<mat[1][3];
		kout2<<endl;
		kout2<<"Nz=";
		kout2<<FORMAT<<mat[2][0];
		kout2<<"Oz=";
		kout2<<FORMAT<<mat[2][1];
		kout2<<"Az=";
		kout2<<FORMAT<<mat[2][2];
		kout2<<"Pz=";
		kout2<<FORMAT<<mat[2][3];
		kout2<<endl;
	}


};


struct Keypoint
{
	dT44 pos;
	double target_vel;
	int method;
	double angle[6];
};


class Lever
{
	public :
		dT44 DH_matrix;
		Lever_Para  para;
		
		Lever()
		{
			para.num=0;
			para.joint_angle=0;
			para.length=0;
			para.offset=0;
			para.torsion_angle=0;
			para.max=0;
			para.min=0;
		}

bool init(Lever_Para test)
{

	if(((test.joint_angle>=test.min)&&(test.joint_angle<=test.max)&&(test.num>=1)&&(test.num<=6))==true)
	{
		para.num=test.num;
	    para.max=test.max;
        para.offset=test.offset;
        para.length=test.length;
        para.torsion_angle=test.torsion_angle;
        para.min=test.min;
        para.joint_angle=test.joint_angle;
		DH_matrix.update(para);
		return true;
	}
	else 
	    return false;

}
	bool update(double angle)
	{
		para.joint_angle=angle;
	    DH_matrix.update(para);
		return true;

	}
};




class Quaternion
{
public:
        double RotationVector[3];
        double RotationAngle;
		Quaternion()
		{
			RotationAngle = 0;
			RotationVector[0]=0;RotationVector[1]= 0;RotationVector[2]=0;
		}


		Quaternion(double a[4])
		{
			RotationAngle = a[0];
			RotationVector[0]= a[1];RotationVector[1]= a[2];RotationVector[2]= a[3];
		}

		Quaternion(dT44 T)
		{
			double Q[4];
			T.TR2Q(Q);
			RotationAngle = Q[0];
			RotationVector[0]= Q[1];RotationVector[1]= Q[2];RotationVector[2]= Q[3];
		}
		void Q2Vect(double a[4])
		{
				a[0]=RotationAngle;a[1]=RotationVector[0];a[2]=RotationVector[1];a[3]=RotationVector[2];
		}
		dT44 Q2H()
		{
				double	s = RotationAngle;
				double x = RotationVector[0];
				double y = RotationVector[1];
				double z = RotationVector[2];
				double a[4][4]={0};
				a[0][0]=1-2*(y*y+z*z);a[0][1]=2*(x*y-s*z);a[0][2]=2*(x*z+s*y);
				a[1][0]=2*(x*y+s*z);a[1][1]=1-2*(x*x+z*z);a[1][2]=2*(y*z-s*x);
				a[2][0]=2*(x*z-s*y);a[2][1]=2*(y*z+s*x);a[2][2]=1-2*(x*x+y*y);
				a[3][3] =1;
				return dT44(a);
		}
};







class Pose
{
public:
	Lever lev[6];
	dT44  pos;
	Pose()
	{
		
		Lever_Para initpara[6]={{1,200,-90*UNIT,585,0*UNIT, 170*UNIT,-170*UNIT},
								{2,600,  0*UNIT,0,  0*UNIT, 120*UNIT,-80*UNIT},
								{3,115,-90*UNIT,770,0*UNIT, 85*UNIT, -170*UNIT},
								{4,0 ,  90*UNIT,0,  0*UNIT, 180*UNIT,-180*UNIT},
								{5,0,  -90*UNIT,0,  0*UNIT, 135*UNIT,-135*UNIT},
								{6,0,    0*UNIT,100,0*UNIT, 360*UNIT,-360*UNIT}};
		for(int i=0;i<6;++i)
	{
		lev[i].init(initpara[i]);
	}
		pos=lev[0].DH_matrix*lev[1].DH_matrix*lev[2].DH_matrix*lev[3].DH_matrix*lev[4].DH_matrix*lev[5].DH_matrix;
	}


void update(double theta[6])
{

				for(int j=0;j<6;++j)
				{
					lev[j].update(theta[j]);
				}
				pos=lev[0].DH_matrix*lev[1].DH_matrix*lev[2].DH_matrix*lev[3].DH_matrix*lev[4].DH_matrix*lev[5].DH_matrix;
}


void position_display()
	{
		kout2<<"机器人当前末端位置坐标："<<endl;
		kout2<<"Nx=";
		kout2<<FORMAT<<pos.mat[0][0];
		kout2<<"Ox=";
		kout2<<FORMAT<<pos.mat[0][1];
		kout2<<"Ax=";
		kout2<<FORMAT<<pos.mat[0][2];
		kout2<<"Px=";
		kout2<<FORMAT<<pos.mat[0][3];
		kout2<<endl;
		kout2<<"Ny=";
		kout2<<FORMAT<<pos.mat[1][0];
		kout2<<"Oy=";
		kout2<<FORMAT<<pos.mat[1][1];
		kout2<<"Ay=";
		kout2<<FORMAT<<pos.mat[1][2];
		kout2<<"Py=";
		kout2<<FORMAT<<pos.mat[1][3];
		kout2<<endl;
		kout2<<"Nz=";
		kout2<<FORMAT<<pos.mat[2][0];
		kout2<<"Oz=";
		kout2<<FORMAT<<pos.mat[2][1];
		kout2<<"Az=";
		kout2<<FORMAT<<pos.mat[2][2];
		kout2<<"Pz=";
		kout2<<FORMAT<<pos.mat[2][3];
		kout2<<endl;

	}

};

class GSK
{
public:
	static void zero(double u[], int s)
	{
		int i = 0;
		for(i = 0; i< s; i++)
		if(fabs(u[i])<1e-10)
			u[i] = 0;
	}

	static nump mymin(double u[],int s)
	{
		nump temp;
		temp.num = u[0];
		temp.p = 0;
		for(int i = 0; i< s-1; i++)
		if(temp.num> u[i+1])
		{
			temp.num = u[i+1];
			temp.p = i+1;
		}	
		return temp;
	}


	static dT44 Forward_Kinematics (double joint_angle[6])
	{
		Pose temp;
		temp.update(joint_angle);
		return temp.pos;
	}

	static void Inverse_Kinematics (dT44 position,double theta[6],Pose chain)
	{	
	
	double nx = position.mat[0][0];    double ox = position.mat[0][1];     double ax = position.mat[0][2];     double px = position.mat[0][3]; 
	double ny = position.mat[1][0];    double oy = position.mat[1][1];     double ay = position.mat[1][2];     double py = position.mat[1][3];
	double nz = position.mat[2][0];    double oz = position.mat[2][1];     double az = position.mat[2][2];     double pz = position.mat[2][3];

	int i,j;
	double Px =  px - ax*chain.lev[5].para.offset;
	double Py =  py - ay*chain.lev[5].para.offset;
	double Pz =  pz - az*chain.lev[5].para.offset;/**/
//	double theta[6] = {0};
	double tempth[6][16]={0};
	double flag[16]={0};
	double kinangle[6]={0};
	double r=sqrt(Px*Px + Py*Py);
    
	double D11 = r  - chain.lev[0].para.length;
	double D2  = Pz - chain.lev[0].para.offset;
	double D12 =-r  - chain.lev[0].para.length;

	double B11=2*(D2*chain.lev[2].para.offset - D11*chain.lev[2].para.length);
	double B21=2*(D11*chain.lev[2].para.offset + D2*chain.lev[2].para.length);
	double B12=2*(D2*chain.lev[2].para.offset - D12*chain.lev[2].para.length);
	double B22=2*(D12*chain.lev[2].para.offset + D2*chain.lev[2].para.length);


	double beta01 = atan2(B21,B11);
	double beta02=  atan2(B22,B12);
	double temp1=(chain.lev[1].para.length*chain.lev[1].para.length - D11*D11 - D2*D2 - chain.lev[2].para.length*chain.lev[2].para.length - chain.lev[2].para.offset*chain.lev[2].para.offset)/(sqrt(B11*B11 + B21*B21));
	double temp2=(chain.lev[1].para.length*chain.lev[1].para.length - D12*D12 - D2*D2 - chain.lev[2].para.length*chain.lev[2].para.length - chain.lev[2].para.offset*chain.lev[2].para.offset)/(sqrt(B12*B12 + B22*B22));
	double temp3;

	double gama01 = asin(temp1);
	double gama02= asin(temp2);

	double beta0[2] = {beta01,PI+beta01};
	double gama0[2] = {gama01,PI-gama01};
	double beta1[2] = {beta02,PI+beta02};
	double gama1[2] = {gama02,PI-gama02};

	double arf0[4] = {beta01+gama01,beta01+PI-gama01,PI+beta01+gama01,2*PI+beta01-gama01};
	double arf1[4] = {beta02+gama02,beta02+PI-gama02,PI+beta02+gama02,2*PI+beta02-gama02};


	double b11,b12,b13,b21,b22,b31,b32,b33;
	double c12,c22,c31,c33;
	double ans1,ans2;
	double mean[16]={0};
	double weight[6]={W1,W2,W3,W4,W5,W6};
	struct nump mintp;
	double error=0;
	dT44 targetpos;

	//if(fabs(temp1)>1||fabs(temp2)>1)
	//{
	//	kout2<<"该位姿不可解，机器人保持原位"<<endl;
	//	for(i = 0; i< 6; ++i)
	//		theta[i] = chain.lev[i].para.joint_angle;
	//}
	//else
	//{
	

	/*前臂情形*/
	for(i=0;i<8;++i)
	tempth[0][i] = atan2(Py,Px);
		

	for(i=0,j=0;i<4;++i,++j)
	{


	    
	   tempth[1][j] =atan2((D11 - chain.lev[2].para.length*sin(arf0[i]) - chain.lev[2].para.offset*cos(arf0[i])),(D2 - chain.lev[2].para.length*cos(arf0[i]) + chain.lev[2].para.offset*sin(arf0[i])));
       tempth[2][j] =arf0[i]-tempth[1][j];
	   ++j;
       tempth[1][j] =atan2((D11-chain.lev[2].para.length*sin(arf0[i])-chain.lev[2].para.offset*cos(arf0[i])),(D2-chain.lev[2].para.length*cos(arf0[i]) + chain.lev[2].para.offset*sin(arf0[i])));	 
	   tempth[2][j] =arf0[i]-tempth[1][j];
	}

	/*后臂情形*/
	for(i=8;i<16;++i)
    tempth[0][i] = atan2(Py,Px)+PI;
	for(i=0,j=8;i<4;++i,++j)
	{
	   tempth[1][j] =atan2((D12 - chain.lev[2].para.length*sin(arf1[i]) - chain.lev[2].para.offset*cos(arf1[i])),(D2 - chain.lev[2].para.length*cos(arf1[i]) + chain.lev[2].para.offset*sin(arf1[i])));
       tempth[2][j] = arf1[i]-tempth[1][j];
	   ++j;
       tempth[1][j] = atan2((D12-chain.lev[2].para.length*sin(arf1[i])-chain.lev[2].para.offset*cos(arf1[i])),(D2-chain.lev[2].para.length*cos(arf1[i]) + chain.lev[2].para.offset*sin(arf1[i])));	 
	   tempth[2][j] = arf1[i]-tempth[1][j];
	}

		
	

	/*以上为投影法求angle1-3,其中前臂四组解，后壁四组解，但因为之后用前三个角的值求出的angle6有两种情况，故前后臂共
	16组解，相同值各占两组*/

	for(i=0;i<16;i+=2)
	{
	
	 b31=-cos(tempth[0][i])*sin(tempth[1][i])*sin(tempth[2][i])+cos(tempth[0][i])*cos(tempth[1][i])*cos(tempth[2][i]);
	 b32=-sin(tempth[0][i])*sin(tempth[1][i])*sin(tempth[2][i])+sin(tempth[0][i])*cos(tempth[1][i])*cos(tempth[2][i]);
	 b33=-cos(tempth[1][i])*sin(tempth[2][i])-sin(tempth[1][i])*cos(tempth[2][i]);

	 ans1=b31*nx+b32*ny+b33*nz;
	 ans2=b31*ox+b32*oy+b33*oz;

	 //cout<<"ans1"<<ans1<<endl;
    //	cout<<"ans2"<<ans2<<endl;

	 j=i;
	 if((ans1==0)&&(ans2!=0))
	 {
		 tempth[5][j]=PI/2;
		 tempth[5][++j]=-PI/2;
	 }
	 else 
	 {
		 if ((ans1!=0)&&(ans2==0))
	    {
			tempth[5][j]=0.0;
			tempth[5][++j]=PI;
	    }
		else
	    {
		    if ((ans1!=0)&&(ans2!=0))
	        {
				tempth[5][j] = atan2((-ans2),ans1);
				tempth[5][++j] = atan2(ans2,(-ans1));
	        }
			else 
			{
				if ((ans1==0)&&(ans2==0))
			{
				tempth[5][j]   =  chain.lev[5].para.joint_angle;
				tempth[5][++j] =  chain.lev[5].para.joint_angle;
				kout2<<"**********************************奇异点出现*********************************"<<endl;
			}
		}
	  }
	 }
	}
	
	for(i=0;i<16;++i)
	{

	 b11 = cos(tempth[0][i])*sin(tempth[1][i])*cos(tempth[2][i]) + cos(tempth[0][i])*cos(tempth[1][i])*sin(tempth[2][i]);
	 b12 = sin(tempth[0][i])*sin(tempth[1][i])*cos(tempth[2][i]) + sin(tempth[0][i])*cos(tempth[1][i])*sin(tempth[2][i]);
	 b13 = cos(tempth[1][i])*cos(tempth[2][i]) - sin(tempth[1][i])*sin(tempth[2][i]);

	 b21 = sin(tempth[0][i]);
	 b22 = -cos(tempth[0][i]);
	 b31 = -cos(tempth[0][i])*sin(tempth[1][i])*sin(tempth[2][i]) + cos(tempth[0][i])*cos(tempth[1][i])*cos(tempth[2][i]);
	 b32 = -sin(tempth[0][i])*sin(tempth[1][i])*sin(tempth[2][i]) + sin(tempth[0][i])*cos(tempth[1][i])*cos(tempth[2][i]);
	 b33 = -cos(tempth[1][i])*sin(tempth[2][i]) - sin(tempth[1][i])*cos(tempth[2][i]);


     c12 = (b11*nx + b12*ny + b13*nz)*sin(tempth[5][i]) + (b11*ox + b12*oy + b13*oz)*cos(tempth[5][i]);
	 c22 = (b21*nx + b22*ny)*sin(tempth[5][i])+(b21*ox + b22*oy)*cos(tempth[5][i]);
	 c31 = (b31*nx + b32*ny + b33*nz)*cos(tempth[5][i]) - (b31*ox + b32*oy + b33*oz)*sin(tempth[5][i]);
	 c33 = b31*ax + b32*ay  + b33*az;

	 tempth[3][i] = atan2(-c12,c22);
	 tempth[4][i] = atan2(c31,c33);
	}




	for(i=0;i<16;++i)
	{
				
				for(j=0;j<6;++j)
				{
					//if (tempth[j][i]>chain.lev[j].para.max)
						if (tempth[j][i]>2*PI)
					{
					//	 if((tempth[j][i]-2*PI)>chain.lev[j].para.min)
							 tempth[j][i]-=2*PI;
					}
					else
					{
					//	if(tempth[j][i]<chain.lev[j].para.min)
						if (tempth[j][i]<-2*PI)
						{
							//if((tempth[j][i]+2*PI)<chain.lev[j].para.max)
								tempth[j][i]+=2*PI;
						}
					}
					/*
						
						if (tempth[j][i]>chain.lev[j].para.max)
						 tempth[j][i]-=2*PI;
					else 
						if(tempth[j][i]<chain.lev[j].para.min)
						 tempth[j][i]+=2*PI;
						if (tempth[j][i]>chain.lev[j].para.max)
						 tempth[j][i]-=2*PI;
					else 
						if(tempth[j][i]<chain.lev[j].para.min)
						 tempth[j][i]+=2*PI;*/
						
				}
						
	}
	/*
		for (j=0;j<16;++j)
			 {
                   for( i = 0; i< 6;i++)
				   {
                       if ((tempth[i][j]>chain.lev[i].para.max)||(tempth[i][j]<chain.lev[i].para.min))
					   {
						   flag[j]=INF1;
						   break;
					   }                          
				   }
			 }
*/
			 
		for (i=0;i<16;++i)
			 {/*
				if(flag[i]==INF1)
					continue;*/
				for(j=0;j<6;++j)
				{
					kinangle[j]=tempth[j][i];
				
				}
				targetpos=Forward_Kinematics(kinangle);
				if(targetpos!=position)
					flag[i]=INF2;
		
			}


		for (j=0;j<16;++j)
			 {
				error=0;
			//	if(flag[j]==INF1||flag[j]==INF2)
					if(flag[j]==INF2)
					continue;
					else
						{
							for(i = 0; i< 6; ++i)	
							{
				//	if(i==5)
				//	{
								temp1=fabs(tempth[i][j] - chain.lev[i].para.joint_angle);
								if(tempth[i][j]>0)
									temp3=2*PI-tempth[i][j];
								else
									temp3=2*PI+tempth[i][j];
								temp2=fabs(temp3 - chain.lev[i].para.joint_angle);
								if(temp1>temp2)
								{
									error+=temp2;
									tempth[i][j]=temp3;								
								}
								else
								error+=temp1;
				//	}
				//	else
					
				//	error+=fabs(tempth[i][j] - chain.lev[i].para.joint_angle);
							}
								flag[j]=error;
						}
				
			}		

	
			mintp = mymin(flag,16);

				if(flag[mintp.p]==INF1||flag[mintp.p]==INF2)
				{
					kout2<<"位姿不可达，保持原位"<<endl;
					for(i = 0; i< 6; ++i)
					theta[i] = chain.lev[i].para.joint_angle;
				}
				else
					for(i = 0; i< 6; ++i)
				    theta[i] = tempth[i][mintp.p];
				


		/*	
				cout<<endl<<"当前角"<<endl;
			for( i = 0; i< 6;i++)
				{
					cout<<FORMAT<<chain.lev[i].para.joint_angle*180/PI<<"  ";
					if(i==2)
					cout<<endl;
				}
					cout<<endl<<endl<<endl;
				
			*/
		
			kout2<<endl<<"逆解角（共16组多解）"<<endl;
		
			for( j=0;j<16;++j)
			{
				
				for( i = 0; i< 6;i++)
				{
					kout2<<FORMAT<<tempth[i][j]*180/PI<<"  ";
					if(i==2)
					kout2<<endl;
				}
					kout2<<FORMAT<<flag[j]<<"  ";
					kout2<<endl<<endl<<endl;
				
			}
				
	//	}		

			kout2<<endl<<"最优解"<<endl;
			for( i = 0; i< 6;i++)
				{
					kout2<<FORMAT<<theta[i]*180/PI<<"  ";
					if(i==2)
					kout2<<endl;
				}
					kout2<<endl<<endl<<endl;
				
	
		
		}
};




//void sub_matrix(double source[][RANK],double adjoin[][RANK-1],int x,int y);
//double dent_matrix(double adjoin[][RANK-1]);
//bool  inv_matrix(double source[][RANK],double inv[][RANK]);

//bool Simulation(Lever_Para para[6]);








class Ctraj 
{
public:
	dT44 T0;
	dT44 T1;
	double tv;
	Pose temp;
	
	double theta;
	double fx;
	double fy;
	double fz;
//	dT44 res[50];
//	double theta[50][6];
	 void init(double endpoint[6],double startpoint[6],double v)
	{
		double flat1[3]={0};
		double flat2[3]={0};
		temp.update(startpoint);
		T0=GSK::Forward_Kinematics (startpoint);
		T1=GSK::Forward_Kinematics (endpoint);
		T0.show();
		kout2<<endl;
		T1.show();
		T0.H2Trans(flat1);
		T1.H2Trans(flat2);
	//	T2.H2Trans(flat3);
		double L1=(flat2[0]-flat1[0])*(flat2[0]-flat1[0])+(flat2[1]-flat1[1])*(flat2[1]-flat1[1])+(flat2[2]-flat1[2])*(flat2[2]-flat1[2]);
		int tt=(int)(sqrt(L1)/(v*simstep)+0.5);
		tv=double(tt);
		T0.TransMode();
		//T0.show();
			dT44 T2=T0*T1;
		//	T2.show();
			double temp1=T2.mat[2][1]-T2.mat[1][2];
			double temp2=T2.mat[0][2]-T2.mat[2][0];
			double temp3=T2.mat[1][0]-T2.mat[0][1];
			theta=atan2(sqrt(temp1*temp1+temp2*temp2+temp3*temp3),T2.mat[0][0]+T2.mat[1][1]+T2.mat[2][2]-1);
			fx=temp1/(2*sin(theta));
			fy=temp2/(2*sin(theta));
			fz=temp3/(2*sin(theta));

			kout2<<fx<<endl;
			kout2<<fy<<endl;
			kout2<<fz<<endl;
			kout2<<theta*180/PI<<endl;
	}

 void Traceplanning(double index,double qt[6])
	{
		double R=0;
		if(index>=tv)
			R=1;
		else
			R=(index)/(tv);
		dT44 res=trinterp(R);
		if((int)index%2==0)
		GSK::Inverse_Kinematics(res,qt,temp);
		if((int)index%2==0)
		temp.update(qt);
	//	if((int)index%10==0)
		temp.pos.show( );
		kout2<<endl<<endl;
	}

		Quaternion Qinterp(Quaternion Q1, Quaternion Q2, double r)
	{

	    double q1[4],q2[4],a[4];
		Q1.Q2Vect(q1);
		Q2.Q2Vect(q2);
		double temp=q1[0]*q2[0]+q1[1]*q2[1]+q1[2]*q2[2]+q1[3]*q2[3];
        double theta = acos(temp);
        if ((r<0) || (r>1))
			printf("r error\n");
		if (theta == 0)
            return Q1;
        else
		{
			for(int i=0;i<=3;++i)
			a[i]=(sin((1-r)*theta) * q1[i] + sin(r*theta) * q2[i]) / sin(theta); 
            return Quaternion(a);   
		}

	}

		dT44 trinterp(double cp)
	{
		//	Quaternion q0 = Quaternion(T0);
          //  Quaternion q1 = Quaternion(T1);
		//	Quaternion qr = Qinterp(q0, q1, cp);
		//	dT44 nos=qr.Q2H();
			dT44 nos=PosInterp(cp);
			int x;
			double p0[3],p1[3];
			double a[4][4]={0};
			T0.H2Trans(p0);
			T1.H2Trans(p1);
            for(int i=0;i<=2;++i){
             a[i][3]=p0[i]*(1-cp) + cp*p1[i];
					for(x=0;x<=2;++x)
						a[i][x]=nos.mat[i][x];}
			a[3][3]=1;
			return dT44(a);
	}
		dT44 PosInterp(double cp)
	{
			double thetai=theta*cp;
			double mat[4][4]={0};
			double verstheta=1-cos(thetai);
			mat[0][0]=fx*fx*verstheta+cos(thetai);
			mat[0][1]=fy*fx*verstheta-fz*sin(thetai);
			mat[0][2]=fz*fx*verstheta+fy*sin(thetai);

			mat[1][0]=fx*fy*verstheta+fz*sin(thetai);
			mat[1][1]=fy*fy*verstheta+cos(thetai);
			mat[1][2]=fz*fy*verstheta-fx*sin(thetai);
			
			mat[2][0]=fx*fz*verstheta-fy*sin(thetai);
			mat[2][1]=fy*fz*verstheta+fx*sin(thetai);
			mat[2][2]=fz*fz*verstheta+cos(thetai);

			dT44 Ti=dT44(mat);
			T0.TransMode();
			Ti=T0*Ti;
			return Ti;
	}
};

class Arctraj 
{
public:
	double tv1;
	double tv2;
	Pose robot1,robot2;
	double point[3],dis1[3],dis2[3],dis3[3];
	double R;
	double theta1;
	double theta2;
	dT44 T0;
	dT44 T1;
	dT44 T2;
	double theta22,theta11,fx1,fx2,fy1,fy2,fz1,fz2;
	void init(double endpoint[6],double startpoint[6],double middlepoint[6],double v1,double v2)
	{
		robot1.update(startpoint);
		robot2.update(middlepoint);
		double flat1[3]={0};
		double flat2[3]={0};
		double flat3[3]={0};
		int t; 
		T0=GSK::Forward_Kinematics (startpoint);
		T1=GSK::Forward_Kinematics (middlepoint);
		T2=GSK::Forward_Kinematics (endpoint);
		T0.show();
		//cout<<"**************************************"<<endl;
		T1.show();
		//cout<<"**************************************"<<endl;
		T2.show();
		//cout<<"**************************************"<<endl;
		T0.H2Trans(flat1);
		T1.H2Trans(flat2);
		T2.H2Trans(flat3);
		double L1=(flat2[0]-flat1[0])*(flat2[0]-flat1[0])+(flat2[1]-flat1[1])*(flat2[1]-flat1[1])+(flat2[2]-flat1[2])*(flat2[2]-flat1[2]);
		double L2=(flat3[0]-flat2[0])*(flat3[0]-flat2[0])+(flat3[1]-flat2[1])*(flat3[1]-flat2[1])+(flat3[2]-flat2[2])*(flat3[2]-flat2[2]);
		int tt1=(int)(sqrt(L1)/(v1*simstep)+0.5);
		int tt2=(int)(sqrt(L2)/(v2*simstep)+0.5);
		tv1=double(tt1);tv2=double(tt2);
		
		double temp1[3]={0};
		double temp2[3]={0};
		double temp3[3]={0};
		double A[4]={0};
		double B[4]={0};
		double C[4]={0};
		double F,E;
		

		for(t=0;t<=1;++t)
		{
			temp1[t]=flat1[t+1];
			temp2[t]=flat2[t+1];
			temp3[t]=flat3[t+1];
		}

			temp1[2]=1;temp2[2]=1;temp3[2]=1;
			A[0]=det(temp1,temp2,temp3);

			temp1[0]=flat1[0];temp1[1]=flat1[2];temp1[2]=1;
			temp2[0]=flat2[0];temp2[1]=flat2[2];temp2[2]=1;
			temp3[0]=flat3[0];temp3[1]=flat3[2];temp3[2]=1;
			A[1]=-det(temp1,temp2,temp3);

		for(t=0;t<=1;++t)
		{
			temp1[t]=flat1[t];
			temp2[t]=flat2[t];
			temp3[t]=flat3[t];
		}

			temp1[2]=1;temp2[2]=1;temp3[2]=1;
			A[2]=det(temp1,temp2,temp3);
			A[3]=det(flat1,flat2,flat3);

		for(t=0;t<=2;++t)
		{
			temp1[t]=0.0;
			temp2[t]=0.0;
			temp3[t]=0.0;
		}


		for(t=0;t<=2;++t)
		{
			temp1[0]+=flat1[t]*flat1[t];
			temp1[1]+=flat2[t]*flat2[t];
			temp1[2]+=flat3[t]*flat3[t];
			B[t]=flat2[t]-flat1[t];
			C[t]=flat2[t]-flat3[t];
		}
			B[3]=0.5*(temp1[1]-temp1[0]);
			C[3]=0.5*(temp1[1]-temp1[2]);

			temp1[0]=A[0];temp1[1]=A[1];temp1[2]=A[2];
			temp2[0]=B[0];temp2[1]=B[1];temp2[2]=B[2];
			temp3[0]=C[0];temp3[1]=C[1];temp3[2]=C[2];
			E=det(temp1,temp2,temp3);
			temp1[0]=A[3];temp1[1]=A[1];temp1[2]=A[2];
			temp2[0]=B[3];temp2[1]=B[1];temp2[2]=B[2];
			temp3[0]=C[3];temp3[1]=C[1];temp3[2]=C[2];
			F=-det(temp1,temp2,temp3);
			point[0]=F/E;
			temp1[0]=A[0];temp1[1]=A[3];temp1[2]=A[2];
			temp2[0]=B[0];temp2[1]=B[3];temp2[2]=B[2];
			temp3[0]=C[0];temp3[1]=C[3];temp3[2]=C[2];
			F=det(temp1,temp2,temp3);
			point[1]=F/E;
			temp1[0]=A[0];temp1[1]=A[1];temp1[2]=A[3];
			temp2[0]=B[0];temp2[1]=B[1];temp2[2]=B[3];
			temp3[0]=C[0];temp3[1]=C[1];temp3[2]=C[3];
			F=det(temp1,temp2,temp3);
			point[2]=F/E;
			temp1[0]=(flat1[0]-point[0])*(flat1[0]-point[0]);
			temp1[1]=(flat1[1]-point[1])*(flat1[1]-point[1]);
			temp1[2]=(flat1[2]-point[2])*(flat1[2]-point[2]);
			R=sqrt(temp1[0]+temp1[1]+temp1[2]);
			temp1[0]=sqrt(B[0]*B[0]+B[1]*B[1]+B[2]*B[2]);
			theta1=2*asin(temp1[0]/(2*R));
			temp1[0]=sqrt(C[0]*C[0]+C[1]*C[1]+C[2]*C[2]);
			theta2=2*asin(temp1[0]/(2*R));
			
			for(t=0;t<=2;++t)
			{
				dis1[t]=flat1[t]-point[t];
				dis2[t]=flat2[t]-point[t];
				dis3[t]=flat3[t]-point[t];
			}

			T0.TransMode();
			T0.show();
			dT44 TT1=T0*T1;
			TT1.show();
			double temp11=TT1.mat[2][1]-TT1.mat[1][2];
			double temp21=TT1.mat[0][2]-TT1.mat[2][0];
			double temp31=TT1.mat[1][0]-TT1.mat[0][1];
			theta11=atan2(sqrt(temp11*temp11+temp21*temp21+temp31*temp31),TT1.mat[0][0]+TT1.mat[1][1]+TT1.mat[2][2]-1);
			fx1=temp11/(2*sin(theta11));
			fy1=temp21/(2*sin(theta11));
			fz1=temp31/(2*sin(theta11));

			T1.TransMode();
			T1.show();
			dT44 TT2=T1*T2;
			TT2.show();
			double temp12=TT2.mat[2][1]-TT2.mat[1][2];
			double temp22=TT2.mat[0][2]-TT2.mat[2][0];
			double temp32=TT2.mat[1][0]-TT2.mat[0][1];
			theta22=atan2(sqrt(temp12*temp12+temp22*temp22+temp32*temp32),TT2.mat[0][0]+TT2.mat[1][1]+TT2.mat[2][2]-1);
			fx2=temp12/(2*sin(theta22));
			fy2=temp22/(2*sin(theta22));
			fz2=temp32/(2*sin(theta22));

			kout2<<fx1<<endl;
			kout2<<fy1<<endl;
			kout2<<fz1<<endl;
			kout2<<theta11*180/PI<<endl;

			kout2<<fx2<<endl;
			kout2<<fy2<<endl;
			kout2<<fz2<<endl;
			kout2<<theta22*180/PI<<endl;


	}

	void Traceplanning(double index,double qt[6])
	{
		double theta=0;
		double R=0;
		int t=0;
		Quaternion q0,q1,qr;
		dT44 nos;
		if(index<=tv1)
		{
		/*	if(index>=tv1)
			{
				R=1;
				theta=theta1;
			}
			else
			{*/
				R=(index)/(tv1);
				theta=theta1/(tv1)*(index);
		//	}
		//	q0 = Quaternion(T0);
        //   q1 = Quaternion(T1);
		//	qr = Qinterp(q0, q1, R);
			nos=PosInterp1(R);
			for(t=0;t<=2;++t)
			nos.mat[t][3]=point[t]+sin(theta1-theta)/sin(theta1)*dis1[t]+sin(theta)/sin(theta1)*dis2[t];
			nos.mat[3][3]=1;
			for(t=0;t<=5;++t)
		{
			qt[t]=0.0;
		}
		//	if(index%2==0)
			GSK::Inverse_Kinematics(nos,qt,robot1);
		//	if(index%2==0)
			PX<<nos.mat[0][3]<<endl;
			PY<<nos.mat[1][3]<<endl;
			PZ<<nos.mat[2][3]<<endl;
			robot1.update(qt);
	//		robot1.pos.show( );
		}
		else
		{
			index=index-tv1;
			if(index>=tv2)
			{
				R=1;
				theta=theta2;
			}
			else
			{
				R=(index)/(tv2);
				theta=theta2/(tv2)*(index);
			}
	//	q0 = Quaternion(T1);
    //       q1 = Quaternion(T2);
	//	qr = Qinterp(q0, q1, R);
			nos=PosInterp2(R);
			for(t=0;t<=2;++t)
			nos.mat[t][3]=point[t]+sin(theta2-theta)/sin(theta2)*dis2[t]+sin(theta)/sin(theta2)*dis3[t];
			nos.mat[3][3]=1;
			for(t=0;t<=5;++t)
		{
			qt[t]=0.0;
		}
		//	if(index%2==0)
			GSK::Inverse_Kinematics(nos,qt,robot2);
			PX<<nos.mat[0][3]<<endl;
			PY<<nos.mat[1][3]<<endl;
			PZ<<nos.mat[2][3]<<endl;
		//	if(index%2==0)
			robot2.update(qt);
	//		robot2.pos.show( );
		}	

	}

	double det(double row1[3],double row2[3],double row3[3])
	{
		double t1=row1[0]*row2[1]*row3[2];//123
		double t2=-row1[0]*row2[2]*row3[1];//132

		double t3=-row1[1]*row2[0]*row3[2];//213
		double t4=row1[1]*row2[2]*row3[0];//231

		double t5=row1[2]*row2[1]*row3[0];//321
		double t6=-row1[2]*row2[0]*row3[1];//312
		return t1+t2+t3+t4+t5+t6;
	}


		dT44 PosInterp1(double cp)
	{
			double thetai=theta11*cp;
			double mat[4][4]={0};
			double verstheta=1-cos(thetai);
			mat[0][0]=fx1*fx1*verstheta+cos(thetai);
			mat[0][1]=fy1*fx1*verstheta-fz1*sin(thetai);
			mat[0][2]=fz1*fx1*verstheta+fy1*sin(thetai);

			mat[1][0]=fx1*fy1*verstheta+fz1*sin(thetai);
			mat[1][1]=fy1*fy1*verstheta+cos(thetai);
			mat[1][2]=fz1*fy1*verstheta-fx1*sin(thetai);
			
			mat[2][0]=fx1*fz1*verstheta-fy1*sin(thetai);
			mat[2][1]=fy1*fz1*verstheta+fx1*sin(thetai);
			mat[2][2]=fz1*fz1*verstheta+cos(thetai);

			dT44 Ti=dT44(mat);
			T0.TransMode();
			Ti=T0*Ti;
			return Ti;
	}

		dT44 PosInterp2(double cp)
	{
			double thetai=theta22*cp;
			double mat[4][4]={0};
			double verstheta=1-cos(thetai);
			mat[0][0]=fx2*fx2*verstheta+cos(thetai);
			mat[0][1]=fy2*fx2*verstheta-fz2*sin(thetai);
			mat[0][2]=fz2*fx2*verstheta+fy2*sin(thetai);

			mat[1][0]=fx2*fy2*verstheta+fz2*sin(thetai);
			mat[1][1]=fy2*fy2*verstheta+cos(thetai);
			mat[1][2]=fz2*fy2*verstheta-fx2*sin(thetai);
			
			mat[2][0]=fx2*fz2*verstheta-fy2*sin(thetai);
			mat[2][1]=fy2*fz2*verstheta+fx2*sin(thetai);
			mat[2][2]=fz2*fz2*verstheta+cos(thetai);

			dT44 Ti=dT44(mat);
			T1.TransMode();
			Ti=T1*Ti;
			return Ti;
	}



		
};


class Jtraj 
{
public:
	double	   tv;
	double c[6][6];
//	double qd0[6];
//	double qd1[6];
//	double qt[50][6];
	void init(double endpoint[6],double startpoint[6],double v)
	{
		//		tv=n;
//				int m,x;
				for(int i = 0;i<=5;++i)
				{
					*(c[0]+i)=6.0*(endpoint[i] - startpoint[i]); //- 3.0*(qd1[i]+qd0[i]);  
					*(c[1]+i)= -15.0*(endpoint[i] - startpoint[i]);// + (8.0*qd0[i] + 7.0*qd1[i]);
					*(c[2]+i)= 10.0*(endpoint[i] - startpoint[i]);//- (6.0*qd0[i] + 4.0*qd1[i]);
					*(c[3]+i)=0;
					*(c[4]+i)=0;
					*(c[5]+i)=startpoint[i];
				}
		double flat1[3]={0};
		double flat2[3]={0};

		dT44 T0=GSK::Forward_Kinematics (startpoint);
		dT44 T1=GSK::Forward_Kinematics (endpoint);
		T0.H2Trans(flat1);
		T1.H2Trans(flat2);
		double L1=(flat2[0]-flat1[0])*(flat2[0]-flat1[0])+(flat2[1]-flat1[1])*(flat2[1]-flat1[1])+(flat2[2]-flat1[2])*(flat2[2]-flat1[2]);
		int tt=(int)(sqrt(L1/(v*simstep))+0.5);
		tv=double(tt);
				/*
				for(m=0;m<=5;++m)
			{
					for(x=0;x<=5;++x)
						cout<<c[m][x]<<"   ";
							printf("\n\n\n");
				}*/

	}

	void Traceplanning(double index,double qt[6])
	{
		int j,n;
		double t;
		double tt[6]={0};
		if(index>=tv)
			t=1;
		else
			t=(index-1)/(tv-1);
	//	cout<<t<<endl;
		tt[0]=t*t*t*t*t;tt[1]=t*t*t*t;tt[2]=t*t*t;
		tt[3]=t*t;tt[4]=t;tt[5]=1;  
	//	for(j=0;j<=5;++j)
	//		cout<<tt[j]<<"   ";
	//		printf("\n\n\n");
		
		for(j = 0;j<=5;++j)
		{
			qt[j]=0.0;
			for(n=0;n<=5;++n)
			qt[j]+=tt[n]*c[n][j];

		}
		/*

			for(j=0;j<=5;++j)
			cout<<qt[j]*180/PI<<"   ";
			printf("\n\n\n");
			*/
		
		
	}

};


class TracePlan
{
public:
	double Total;
	Ctraj ctraj;
	Jtraj jtraj;
	Arctraj arctraj;
	int method;
	TracePlan()
	{
		Total=0;
	}
	void init(int traceplanmethod,double endpoint[6],double startpoint[6],double v1,double middlepoint[6],double v2)
	{
		method=traceplanmethod;
		if(method==JMETHOD){
			jtraj.init(endpoint,startpoint,v1);
			Total=jtraj.tv;}
		else if(method==CMETHOD){
			ctraj.init(endpoint,startpoint,v1);
			Total=ctraj.tv;}
		else{
			arctraj.init(endpoint,startpoint,middlepoint,v1,v2);
			Total=arctraj.tv1+arctraj.tv2;
		}

	}
void Traceplanning(double index,double qt[6])
	{
		if(method==JMETHOD)
			jtraj.Traceplanning(index,qt);
		else if(method==CMETHOD)
			ctraj.Traceplanning(index,qt);
		else
			arctraj.Traceplanning(index,qt);


	}
};