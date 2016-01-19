//Position.h  位置数据与位置变量的头文件
//2010-7-11 16:48:46
#include "Macro.h"
#include "kinematicPro.h"
#pragma once

typedef struct Position
{
	private:
		//位姿描述
		double x, y, z;//末端空间位置
		double Rx, Ry, Rz;//小臂与三坐标轴夹角
		//脉冲描述
		double S, L, U, R, B, T;//六轴脉冲
		//采用直角坐标还是关节坐标, true 直角坐标, false关节坐标
		
	public:
		dT44 Tpos;
		bool Flag;
		
		Position()              //构造函数
		{
			S = L = U = R = B = T = 0;
			this->Pulse2Pos();
			Flag = false;//关节坐标
		}
		void Clear()            //清空
		{
			S = L = U = R = B = T = 0;
			x = y = z = Rx = Ry = Rz = 0.0;
			Flag = false;
			this->Pulse2Pos();
		}

		//提取Flag
		bool GetFlag()
		{
			return Flag;
		}

		//修改Flag
		void SetFlag(bool value)
		{
			Flag = value;
		}

		void Pulse2Pos()        //脉冲值转化为空间坐标
		{
			Pulse2T();
			T2pos();			
		}

		void Pos2Pulse()        //空间坐标转化为脉冲值
		{
		}
		void T2pos()
		{
			Rx=atan2(Tpos.mat[2][1],Tpos.mat[2][2]);
			double sp=sin(Rx);
			double cp=cos(Rx);
			Ry=atan2(-Tpos.mat[2][0],sp*Tpos.mat[2][1]+cp*Tpos.mat[2][2]);
			Rz=atan2(sp*Tpos.mat[0][2]-cp*Tpos.mat[0][1],cp*Tpos.mat[1][1]-sp*Tpos.mat[1][2]);
		}

		void Pulse2T()
		{
			Pose temp;
			double angle[6]={S,L,U,R,B,T};
			temp.update(angle);
			Tpos=temp.pos;
		}
		void Pos2T()
		{
			double a[4][4]={{1,0,0,0},{0,cos(Rx),-sin(Rx),0},{0,sin(Rx),cos(Rx),0},{0,0,0,1}};
			double b[4][4]={{cos(Ry),0,sin(Ry),0},{0,1,0,0},{-sin(Ry),0,cos(Ry),0},{0,0,0,1}};
			double c[4][4]={{cos(Rz),-sin(Rz),0,0},{sin(Rz),cos(Rz),0,0},{0,0,1,0},{0,0,0,1}};
			dT44 Rotx(a);
			dT44 Roty(b);
			dT44 Rotz(c);
			Tpos=Rotz*Roty*Rotx;
			Tpos.mat[0][3]=x;Tpos.mat[1][3]=y;Tpos.mat[2][3]=z;
		}
		double GetPulse(int No)       //提取脉冲值
		{
			double result;
			switch(No)
			{
			case 1: result = S; break;
			case 2: result = L; break;
			case 3: result = U; break;
			case 4: result = R; break;
			case 5: result = B; break;
			case 6: result = T; break;
			default: break;
			}
			return result;
		}
		void SetPulse(int No,double value) //修改脉冲值
		{
			switch(No)
			{
			case 1: S = value; break;
			case 2: L = value; break;
			case 3: U = value; break;
			case 4: R = value; break;
			case 5: B = value; break;
			case 6: T = value; break;
			default: break;
			}
			this->Pulse2Pos(); 
			this->Pulse2T();//同步修改坐标值数据
		}
		double GetPos(int No)      //提取坐标值
		{
			double result;
			switch(No)
			{
			case 1: result = x;  break;
			case 2: result = y;  break;
			case 3: result = z;  break;
			case 4: result = Rx; break;
			case 5: result = Ry; break;
			case 6: result = Rz; break;
			default: break;
			}
			return result;
		}
		void SetPos(int No, double value)//修改坐标值
		{
			switch(No)
			{
			case 1: x  = value; break;
			case 2: y  = value; break;
			case 3: z  = value; break;
			case 4: Rx = value; break;
			case 5: Ry = value; break;
			case 6: Rz = value; break;
			default: break;
			}
			this->Pos2Pulse();
			this->Pos2T();//同步修改脉冲数据
		}
		//重载运算符
		Position & operator += (Position & right)//+=
		{
			S += right.S; L += right.L; U += right.U;
			R += right.R; B += right.B; T += right.T;
			this->Pulse2Pos();//同步修改坐标值
			this->Pulse2T();
			return *this;
		}
		Position & operator += (int right)//+=
		{
			S += right; L += right; U += right;
			R += right; B += right; T += right;
			this->Pulse2Pos();//同步修改坐标值
			this->Pulse2T();
			return *this;
		}
		Position & operator -= (Position & right)//-=
		{
			S -= right.S; L -= right.L; U -= right.U;
			R -= right.R; B -= right.B; T -= right.T;
			this->Pulse2Pos();//同步修改坐标值
			this->Pulse2T();
			return *this;
		}
		Position & operator -= (int right)//-=
		{
			S -= right; L -= right; U -= right;
			R -= right; B -= right; T -= right;
			this->Pulse2Pos();//同步修改坐标值
			this->Pulse2T();
			return *this;
		}
		Position & operator *= (int right)// *=
		{
			S *= right; L *= right; U *= right;
			R *= right; B *= right; T *= right;
			this->Pulse2Pos();//同步修改坐标值
			this->Pulse2T();
			return *this;
		}
		Position & operator /= (int right)// /=
		{
			S /= right; L /= right; U /= right;
			R /= right; B /= right; T /= right;
			this->Pulse2Pos();//同步修改坐标值
			this->Pulse2T();
			return *this;
		}
}_POSITION;



//位置型变量
typedef struct POStype
{
	public:
		Position value;
	private:
		char Note[17];
		
	public:
		POStype()//构造函数
		{
			strcpy(Note, "");
		}
		void Clear()//清空
		{
			value.Clear();
			strcpy(Note, "");
		}
		Position GetValue()//提取value
		{
			return value;
		}
		void SetValue(Position newvalue)//修改value
		{
			value = newvalue;
		}
		char * GetNote()//提取注释
		{
			return Note;
		}
		void SetNote(char * newnote)//修改注释
		{
			strcpy(Note, newnote);
		}
		//重载运算符
		POStype & operator += (POStype & right)//+=
		{
			value += right.value;
			return *this;
		}
		POStype & operator += (int right)
		{
			value += right;
			return *this;
		}
		POStype & operator -= (POStype & right)
		{
			value -= right.value;
			return *this;
		}
		POStype & operator -= (int right)
		{
			value -= right;
			return *this;
		}
		POStype & operator *= (int right)
		{
			value *= right;
			return *this;
		}
		POStype & operator /= (int right)
		{
			value /= right;
			return *this;
		}
}_POSTYPE;
