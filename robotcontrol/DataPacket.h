//DataPacket.h
//定义通信数据包的头文件
#pragma once
#include "Position.h"
#include "head.h"
#include "Program.h"

//任务A : 示教一个点
struct A_information
{
private:
//	int FrameType;     //坐标类型,0:关节坐标; 1:基坐标; 2:用户坐标; 3工具坐标
//	int FrameNo;       //坐标号码
	char Key;          //键值S+ ~ T- : 'A' ~ 'L'
//	double Speed;      //运行速度, 角速度: 度/s, 线速度: mm/s
//速度取自ROBOT的_handspeed

public:
	/***************************
	A_information()
	构造函数
	***************************/
	A_information()
	{
//		FrameType = -1;//坐标系类型置为无效
//		FrameNo = -1;//坐标系号码置为无效
		Key = NULL;//键值置为空
//		Speed = 0;//速度置为0
	}//A_information()

	/****************************
	~A_information()
	析构函数
	****************************/
	~A_information()
	{}//~A_information()

	/****************************
	int GetFrameType()
	提取坐标类型
	-----------------------------
	返回 : 坐标类型
	
	****************************/
//	int GetFrameType()
//	{
//		return this->FrameType;
//	}//GetFrameType()

	/****************************
	void SetFrameType(int NewType)
	修改坐标系类型
	-----------------------------
	NewType : 新的坐标系类型 
	****************************/
//	void SetFrameType(int NewType)
//	{
//		this->FrameType = NewType;
//	}//SetFrameType()

	/****************************
	int GetFrameNo()
	提取坐标系号码
	-----------------------------
	返回 : 坐标系号码
	****************************/
//	int GetFrameNo()
//	{
//		return this->FrameNo;
//	}//GetFrameNo()

	/****************************
	void SetFrameNo(int NewNo)
	修改坐标系号码
	-----------------------------
	NewNo : 新的坐标系号码
	****************************/
//	void SetFrameNo(int NewNo)
//	{
//		this->FrameNo = NewNo;
//	}//SetFrameNo()

	/***************************
	char GetKey()
	提取键值
	----------------------------
	返回 : 键值
	***************************/
	char GetKey()
	{
		return this->Key;
	}//GetKey()

	/***************************
	void SetKey(char NewKey)
	修改键值
	----------------------------
	NewKey : 新键值
	***************************/
	void SetKey(char NewKey)
	{
		this->Key = NewKey;
	}//SetKey()

	/***************************
	double GetSpeed()
	提取速度值
	----------------------------
	返回 : 速度值
	***************************/
//	double GetSpeed()
//	{
//		return this->Speed;
//	}//GetSpeed()

	/***************************
	void SetSpeed(double NewSpeed)
	修改速度值
	----------------------------
	NewSpeed : 新的速度值
	***************************/
//	void SetSpeed(double NewSpeed)
//	{
//		this->Speed = NewSpeed;
//	}//SetSpeed()

};//A_information

//任务B : 控制机器人运动到一个已知点
struct B_information
{
private:
//	int InterType;//插补方式:0关节插补MOVJ, 1直线插补MOVL, 2圆弧插补MOVC, 3曲线插补MOVS
//插补方式默认为MOVJ
//	double Speed; //插补速度, 圆弧: 度/s, 其它: mm/s
//速度为手动速度,即Robot下的_handspeed
//	Position Target[2];//目标位置信息; MOVJ/MOVL只取Target[0]; MOVC/MOVS取Target的两个数据
	int Target;//目标点,0:绝对原点, 1:第二原点, 2:工作原点
	

public:
	/*****************************
	B_information()
	构造函数
	*****************************/
	B_information()
	{
//		InterType = -1;//插补方式置为无效值
//		Speed = 0.0;// 速度置为0
		Target = -1;
	}//B_information()

	/******************************
	~B_information()
	析构函数
	******************************/
	~B_information(){}//~B_information()

	/******************************
	int GetTarget()
	提取目标点代码
	******************************/
	int GetTarget()
	{
		return Target;
	}//int GetTarget()

	/******************************
	void SetTarget(int value)
	修改目标点代码
	----------- 返回 --------------
	******************************/
	void SetTarget(int value)
	{
		Target = value;
	}//void SetTarget()

	/******************************
	int GetInterType()
	提取插补方式
	-------------------------------
	返回 : 插补方式
	******************************/
//	int GetInterType()
//	{
//		return this->InterType;
//	}//GetInterType()

	/******************************
	void SetInterType(int Type)
	修改插补方式
	-------------------------------
	Type=0 : 关节查补
	Type=1 : 直线查补
	Type=2 : 圆弧查补
	Type=3 : 曲线查补
	-------------------------------
	Type : 新的插补方式
	******************************/
//	void SetInterType(int Type)
//	{
//		this->InterType = Type;
//	}//SetInterType()

	/******************************
	double GetSpeed()
	提取运行速度
	-------------------------------
	返回 : 运行速度
	******************************/
//	double GetSpeed()
//	{
//		return this->Speed;
//	}//GetSpeed()

	/******************************
	void SetSpeed(double NewSpeed)
	修改运行速度
	-------------------------------
	NewSpeed : 新的运行速度
	******************************/
//	void SetSpeed(double NewSpeed)
//	{
//		this->Speed = NewSpeed;
//	}//SetSpeed()

	/******************************
	Position GetTarget(int No)
	提取目标位置的第No个数据
	-------------------------------
	No : 欲提取的数据在Target[]中的下标
	No = (0 ~ 2)
	******************************/
//	Position GetTarget(int No)
//	{
//		Position Result;
//		switch(No)
//		{
//		case 0://提取Targe[0]
//			{
//				Result = this->Target[0];
//				break;
//			}
//		case 1://提取Targe[1]
///			{
///				Result = this->Target[1];
//				break;
//			}
//		default:break;
//		}
//		return Result;
//	}//GetTarget()

	/******************************
	bool SetTarget(int No, Position Pos)
	修改目标位置的第No个数据
	-------------------------------
	No : 欲提取的数据在Target[]中的下标,NO=(0 - 2)
	Pos: Target[No]的新数据
	******************************/
//	bool SetTarget(int No, Position Pos)
//	{
///		bool Result = true;
//		switch(No)
//		{
//		case 0://修改Target[0]
//			{
//				this->Target[0] = Pos;
//				break;
//			}
//		case 1://修改Target[1]
//			{
//				this->Target[1] = Pos;
//				break;
//			}
//		default:
//			{
//				Result = false;
//				break;
//			}
//		}
//		return Result;
//	}//SetTarget()

	
};//B_information

//任务C : 发送位置型数据
struct C_information
{
public:
	bool state;
	Position Pos;//位置数据
public:
};//C_information

//任务D : 前进
struct D_information
{
private:
	char Key;//键值：'A'：前进，'B'：连锁+前进，'C'：转换+前进

public:
	D_information()
	{
		Key = NULL;
	}//D_information()

	//提取Key
	char GetKey()
	{
		return Key;
	}//GetKey()

	//修改Key
	void SetKey(char value)
	{
		Key = value;
	}//SetKey()
};//D_information

//任务 E : 后退动作
struct E_information
{};//E_information

//任务F : 运行
struct F_information
{};//F_information

//任务G : 试运行
struct G_information
{};//G_information

//任务H : 停止
struct H_information
{};//H_information

//任务I : 暂停
struct I_information
{};//I_information




/********************DataPacket**************************/
struct DataPacket
{
	//数据成员
private:
	char Mission;    //任务代号
public:
	//数据端
	Program m_program;//欲执行的程序
	Robot ROBOT;//全局变量ROBOT
	UserVariable VARIABLE;//全局变量VARIABLE

	//任务信息
	A_information A_Mission;//示教一个点
	B_information B_Mission;//运动到某个原点
	C_information C_Mission;//传送位置数据
	D_information D_Mission;//"前进"动作
	E_information E_Mission;//"后退"动作
	F_information F_Mission;//"运行"动作
	G_information G_Mission;//"试运行"动作
	H_information H_Mission;//"停止"动作
	I_information I_Mission;//"暂停"动作

	//函数成员
public:
	/************************
	DataPacket()
	构造函数
	************************/
	DataPacket()
	{
		Mission = 'Z';//'Z' 暂时做为空任务的代码 
	}//DataPacket()

	/*************************
	~DataPacket()
	析构函数
	*************************/
	~DataPacket()
	{}//~DataPacket()

	/**************************
	char GetMission()
	提取任务代码
	---------------------------
	返回:
	任务代码
	**************************/
	char GetMission()
	{
		return this->Mission;
	}//GetMission()

	/**************************
	void SetMission(char NewMission)
	修改任务代码
	---------------------------
	NewMission : 新的任务代码
	**************************/
	void SetMission(char NewMission)
	{
		this->Mission = NewMission;
	}//SetMission()

};//DataPacket