#include <memory.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdlib.h> 
#include <stdio.h>  
#include  <conio.h>   
#include  <time.h> 



using namespace std;

#define PRE 0.01

#define Pi  3.14159265358979323846264338327950288419716939937510
#define UNIT Pi/180

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



class HomoMatrix
{
	
public:
	double  mat[4][4];

	void H2Trans(double a[3])
	{	
		a[0]=mat[0][3];
		a[1]=mat[1][3];
		a[2]=mat[2][3];
	}

	void    TR2Q(double	Q[4])
	 {
		 
		double trace=mat[0][0]+mat[1][1]+mat[2][2]; 
		 double qs = sqrt(trace+1)/2.0;
         double kx = mat[2][1] - mat[1][2];    // Oz - Ay
         double ky = mat[0][2]- mat[2][0]; // Ax - Nz
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

				nm =fabs(kx)*fabs(kx)+fabs(ky)*fabs(ky)+fabs(kz)*fabs(kz);
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


	HomoMatrix()
	{
		int m,n;
		for(m=0;m<4;++m)
		{
			for(n=0;n<4;++n)
			mat[m][n]=0;
		}
		mat[3][3]=1;
	}



	HomoMatrix(double a[][4])
	{

		int m,n;
		for(m=0;m<4;++m)
		{
			for(n=0;n<4;++n)
			mat[m][n]=a[m][n];
		}
	}

	HomoMatrix(HomoMatrix & a)
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
	

	bool HomoMatrix::operator != (HomoMatrix & b)
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

	friend  HomoMatrix operator  * (HomoMatrix & a,HomoMatrix & b)
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


	    return HomoMatrix(temp);
	}

	void show( )
	{
		int m,n;
		for(m=0;m<4;++m)
		{
			for(n=0;n<4;++n)
//			cout<<FORMAT<<mat[m][n];
			cout<<endl;
		}
	}


};


class Lever
{
	public :
		HomoMatrix DH_matrix;
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
		if(((angle>=para.min)&&(angle<=para.max))==true)
	{
		para.joint_angle=angle;
	    DH_matrix.update(para);
		return true;
	}
	else 
	{
		return false;
	}
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

		Quaternion(HomoMatrix T)
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
		HomoMatrix Q2H()
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
				return HomoMatrix(a);
		}
};









class Chain
{
public:
	Lever lev[6];
	HomoMatrix  pos;
	double curangle[6];
	Chain()
	{
		Lever_Para initpara[6]={{1,200,-90*UNIT,585,0*UNIT,170*UNIT,-170*UNIT},{2,600,0*UNIT,0,0*UNIT,150*UNIT,-90*UNIT},{3,115,-90*UNIT,770,0*UNIT,90*UNIT,-150*UNIT},{4,0,90*UNIT,0,0*UNIT,180*UNIT,-180*UNIT},{5,0,-90*UNIT,0,0*UNIT,45*UNIT,-225*UNIT},{6,0,0*UNIT,100,0*UNIT,360*UNIT,-360*UNIT}};
		for(int i=0;i<6;++i)
	{
		lev[i].init(initpara[i]);
		curangle[i]=lev[i].para.joint_angle;
	}
		pos=lev[0].DH_matrix*lev[1].DH_matrix*lev[2].DH_matrix*lev[3].DH_matrix*lev[4].DH_matrix*lev[5].DH_matrix;
	}


void update(double theta[6])
{

				for(int j=0;j<6;++j)
				{
					lev[j].update(theta[j]);
					curangle[j]=lev[j].para.joint_angle;
				}
				pos=lev[0].DH_matrix*lev[1].DH_matrix*lev[2].DH_matrix*lev[3].DH_matrix*lev[4].DH_matrix*lev[5].DH_matrix;
}

};