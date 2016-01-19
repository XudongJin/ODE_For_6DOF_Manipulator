//:RobotDeatil.h 
//组成机器人实体的各种子类
//2010-7-28 8:39:01

/*************************************************************************************
//类清单:SafeMode    SpecialRun    Interfere    IODetail    IO    AlarmDetail    Alarm
//       ToolFrame   UserFrame
*************************************************************************************/

#include "Position.h"
#include "Macro.h"
#include <time.h>
#pragma once

typedef struct SafeMode//安全模式，0为操作模式、1编辑模式、2管理模式
	{
	private:
		int SafeModeLevel;//转换到操作模式，不需要密码.0为操作，1为编辑，2为管理
		char EditorPassword[9];
		char AdministratorPassword[9];
		
		//修改安全等级
		void SetLevel(int newlevel)
		{	
			SafeModeLevel = newlevel;
		}
		//修改编辑密码
		bool SetEditorPassWord(char * newpassword)
		{
			if(strlen(newpassword) > 8)
			{
				return false;
			}

			strcpy(EditorPassword, newpassword);
			return true;
		}
		//修改管理密码
		bool SetAdministratorPassword(char * newpassword)
		{
			if(strlen(newpassword) > 8)
			{
				return false;
			}

			strcpy(AdministratorPassword, newpassword);
			return true;
		}

	public:
		//提取安全等级
		int GetLevel()
		{
			return SafeModeLevel;
		}
		//密码确认,正确true,错误false
		bool CheckPassword(int mode, char * password)
		{
			switch(mode)
			{
			case 1:
				{
					if(strcmp(EditorPassword, password))//编辑密码
					{
						return false;//密码有误
					}
					else
					{
						return true;
					}
					break;
				}
			case 2:
				{
					if(strcmp(AdministratorPassword, password))//编辑密码
					{
						return false;//密码有误
					}
					else
					{
						return true;
					}
					break;
				}
			default:break;
			}
			return false;
		}
		//修改安全等级,成功true,失败false
		bool SetLevel(int newlevel, char * password)
		{
			if(SafeModeLevel >= newlevel)//由高权限等级切换至低权限等级，不需要密码
			{
				SafeModeLevel = newlevel;
				return true;
			}
			switch(newlevel)
			{
			case 1://切换至编辑模式
				{
					bool state = this->CheckPassword(1, password);
					if(state)
					{
						SafeModeLevel = 1;//编辑模式
						return true;
					}
					else
					{
						return false;
					}
					break;
				}
			case 2://切换至管理模式
				{
					bool state = this->CheckPassword(2, password);
					if(state)
					{
						SafeModeLevel = 2;//管理模式
						return true;
					}
					else
					{
						return false;
					}
					break;
				}
			default:break;
			}
			return false;
		}
		//修改密码,成功true,失败false
		bool SetPassword(int level, char * password, char * newpassword)
		{
			switch(level)
			{
			case 0://操作模式
				return true;
			case 1://编辑模式
				{
					if(this->CheckPassword(1, password))//原密码正确
					{
						if(this->SetEditorPassWord(newpassword))//修改密码
							return true;//修改成功
						else
							return false;//修改失败
					}
					break;
				}
			case 2://管理模式
				{
					if(this->CheckPassword(2, password))//原密码正确
					{
						if(this->SetAdministratorPassword(newpassword))//修改密码
							return true;//修改成功
						else
							return false;//修改失败
					}
					break;
				}
			default:break;
			}
			return false;
		}

		//构造函数
		SafeMode()
		{ 
			SafeModeLevel = 0;                         //初始化为操作模式
			strcpy(EditorPassword, "00000000");        //编辑密码，初始化为00000000(8个0)
			strcpy(AdministratorPassword, "99999999"); //管理密码，初始化为99999999(8个9)
		}
}SAFEMODE;

struct SpecialRun//特殊运行
{
private:
	bool Low;      //低速运行
	bool Limit;    //限速运行
	bool Null;     //空运行
	bool Lock;     //机械锁定运行
	bool Check;    //检查运行
public:
	SpecialRun()    //构造函数
	{
		Low = false;
		Limit = false;
		Null = false;
		Lock = false;
		Check = false;
	}

	bool GetLow() //提取Low
	{return Low;}

	void SetLow(bool value)//修改Low
	{Low = value;}

	bool GetLimit()//提取Limit
	{return Limit;}

	void SetLimit(bool value)//修改Limit
	{Limit = value;}

	bool GetNull()//提取Null
	{return Null;}

	void SetNull(bool value)//修改Null
	{Null = value;}

	bool GetLock()//提取Lock
	{return Lock;}

	void SetLock(bool value)//修改Lock
	{Lock = value;}

	bool GetCheck()//提取Check
	{return Check;}

	void SetCheck(bool value)//修改Check
	{Check = value;}
};

//Interfere干涉区
struct Interfere
{
private:
	bool Type;     //true为立方体干涉区,false为轴干社区	

public:
	Position Max;  //最大值
	Position Min;  //最小值

	Interfere()
	{
		Type = true;
	}

	bool GetType()//提取Type
	{return Type;}

	void SetType(bool newType)//修改Type
	{Type = newType;}
};

//IO输入输出,单个通道
struct IODetail
{
private:
	bool Value;
	char Note[33];

public:
	IODetail()
	{
		Value = false;
		strcpy(Note, "");
	}

	bool GetValue()//提取Vale
	{return Value;}

	void SetValue(bool newValue)//修改Value
	{Value = newValue;}

	char *GetNote()//提取Note
	{return Note;}

	void SetNote(char * newNote)
	{
		strcpy(Note, newNote);
	}

};

//IO,每组通道8个
struct IO
{
	IODetail io[_IO_SUM];

public:
	/********************
	显示IO,将IO信息转化为字符串返回
	string Show()
	********************/
	string Show()
	{
		string result="";
		for(int i=_IO_SUM-1; i>=0; --i)
		{
			if(this->io[i].GetValue())
			{
				result += "1";
			}
			else
			{
				result += "0";
			}

			if(4==i)
			{
				result += "_";
			}
		}
		return result;
	}
};

//AlarmDetail报警细节
struct AlarmDetail
{
private:
	char  Time[25];    //报警发生时间
	char  ProName[9];  //发生报警的程序
	int   LineNo;      //发生报警的行数
	char  Note[33];    //报警描述

public:
	/*****************************
	AlarmDetail()
	------------------------------
	说明: 
	构造函数
	*****************************/
	AlarmDetail()
	{
	}

	/*******************************
	void SetTime()
	--------------------------------
	说明:
	修改报警时间
	*******************************/
	void SetTime()
	{
		time_t *tp,t;
		tp = &t;
		time(tp);
		strcpy(Time, ctime(tp));//得到时间字符串
		Time[24]='\0';
	}

	/*******************************
	char *GetTime()
	--------------------------------
	说明:
	提取时间
	--------------------------------
	返回:
	返回时间字符串
	*******************************/
	char *GetTime()
	{
		return Time;
	}

	/*******************************
	void SetProName(char * name)
	--------------------------------
	说明:
	修改发生报警的程序名
	--------------------------------
	参数:
	name: 程序名
	*******************************/
	void SetProName(char *name)
	{
		strcpy(ProName, name);
	}

	/*******************************
	char *GetProName()
	--------------------------------
	说明:
	提取发生警报的程序名
	*******************************/
	char *GetProName()
	{
		return ProName;
	}

	/*******************************
	void SetLineNo(int no)
	--------------------------------
	说明:
	修改发生报警的行号
	--------------------------------
	参数:
	no: 发生报警的行号
	*******************************/
	void SetLineNo(int no)
	{
		LineNo = no;
	}

	/*******************************
	int GetLineNo()
	--------------------------------
	说明:
	提取发生报警的行号
	--------------------------------
	参数:
	no : 发生报警的行号
	*******************************/
	int GetLineNo()
	{
		return LineNo;
	}

	/*******************************
	void SetNote(char *note)
	--------------------------------
	说明:
	修改报警描述
	--------------------------------
	参数:
	note: 报警描述
	*******************************/
	void SetNote(char *note)
	{
		strcpy(Note, note);
	}

	/*******************************
	char *GetNote()
	--------------------------------
	说明:
	提取报警描述
	--------------------------------
	返回:
	报警描述
	*******************************/
	char *GetNote()
	{
		return Note;
	}

	/*******************************
	void Clear()
	--------------------------------
	说明:
	清空报警记录
	*******************************/
	void Clear()
	{
		strcpy(Time, "");
		strcpy(ProName, "");
		strcpy(Note, "");
		LineNo = 0;
	}

	/********************************
	string Show()
	---------------------------------
	说明:
	将报警信息以string形式返回
	---------------------------------
	返回:
	报警信息转化成的string
	********************************/
	string Show()
	{
		string result;
		result += Time;
		result += "   ";
		result += ProName;
		result += "   ";

		char lineno[2];
		sprintf(lineno, "%d   ", LineNo);
		result += lineno;

		result += Note;

		return result;		 
	}
};

//报警
struct Alarm
{
public:
	AlarmDetail Serious[_ALARM_SUM]; //严重报警
	AlarmDetail Slight[_ALARM_SUM];  //轻微报警
	AlarmDetail System[_ALARM_SUM];  //系统报警
	AlarmDetail User[_ALARM_SUM];    //用户报警
	AlarmDetail OffLine[_ALARM_SUM]; //离线报警

public:
	/*********************************
	void Clear(int type)
	----------------------------------
	说明:
	清空报警
	----------------------------------
	参数:
	type=0: 清空所有报警信息
	type=1: 清空Serious报警
	type=2: 清空Slight报警
	type=3: 清空System报警
	type=4: 清空User报警
	type=5: 清空OffLine报警
	**********************************/
	void Clear(int type)
	{
		switch(type)
		{
		case 0://清空所有
			{
				for(int i=0; i<_ALARM_SUM; ++i)
				{
					Serious[i].Clear();
					Slight[i].Clear();
					System[i].Clear();
					User[i].Clear();
					OffLine[i].Clear();
				}
				break;
			}
		case 1://清空Serious
			{
				for(int i=0; i<_ALARM_SUM; ++i)
				{
					Serious[i].Clear();
				}
				break;
			}
		case 2://清空Slight
			{
				for(int i=0; i<_ALARM_SUM; ++i)
				{
					Slight[i].Clear();
				}
				break;
			}
		case 3://清空System
			{
				for(int i=0; i<_ALARM_SUM; ++i)
				{
					System[i].Clear();
				}
				break;
			}
		case 4://清空User
			{
				for(int i=0; i<_ALARM_SUM; ++i)
				{
					User[i].Clear();
				}
				break;
			}
		case 5://清空OffLine
			{
				for(int i=0; i<_ALARM_SUM; ++i)
				{
					OffLine[i].Clear();
				}
				break;
			}
		default:break;
		}
	}

	/******************************************
	string Show(int type)
	-------------------------------------------
	说明: 
	按组显示报警信息
	-------------------------------------------
	参数:
	type=1: 显示Serious
	type=2: 显示Slight
	type=3: 显示System
	type=4: 显示User
	type=5: 显示OffLine
	-------------------------------------------
	返回值:
	将所在组的所有报警信息转化为一个string返回
	******************************************/
	string Show(int type)
	{
		string result;
		switch(type)
		{
		case 1://Serious
			{
				for(int i=0,j=1; i<_ALARM_SUM; ++i)
				{
					char *ProName = Serious[i].GetProName();
					if(strcmp(ProName , ""))//不为空
					{
						char No[10];
						sprintf(No, "%d   ", j);
						result += No;//标号
						result += Serious[i].Show();//内容
						result += "\r\n";//换行

						++j;
					}
				}
				break;
			}
		case 2://Slight
			{
				for(int i=0,j=1; i<_ALARM_SUM; ++i)
				{
					char *ProName = Slight[i].GetProName();
					if(strcmp(ProName , ""))//不为空
					{
						char No[10];
						sprintf(No, "%d   ", j);
						result += No;//标号
						result += Slight[i].Show();//内容
						result += "\r\n";//换行

						++j;
					}
				}
				break;
			}
		case 3://System
			{
				for(int i=0,j=1; i<_ALARM_SUM; ++i)
				{
					char *ProName = System[i].GetProName();
					if(strcmp(ProName , ""))//不为空
					{
						char No[10];
						sprintf(No, "%d   ", j);
						result += No;//标号
						result += System[i].Show();//内容
						result += "\r\n";//换行

						++j;
					}
				}
				break;
			}
		case 4://User
			{
				for(int i=0,j=1; i<_ALARM_SUM; ++i)
				{
					char *ProName = User[i].GetProName();
					if(strcmp(ProName , ""))//不为空
					{
						char No[10];
						sprintf(No, "%d   ", j);
						result += No;//标号
						result += User[i].Show();//内容
						result += "\r\n";//换行

						++j;
					}
				}
				break;
			}
		case 5://OffLine
			{
				for(int i=0,j=1; i<_ALARM_SUM; ++i)
				{
					char *ProName = OffLine[i].GetProName();
					if(strcmp(ProName , ""))//不为空
					{
						char No[10];
						sprintf(No, "%d   ", j);
						result += No;//标号
						result += OffLine[i].Show();//内容
						result += "\r\n";//换行

						++j;
					}
				}
				break;
			}
		default:break;
		}
		return result;
	}

};

//工具坐标
//用一个Position类型数据来保存工具坐标数据
struct ToolFrame
{
public:
	Position data; //用于标示坐标系的position型数据
	char Name[9];  //坐标系名称

public:

	/****************************
	bool SetName(char *name)
	-----------------------------
	说明:
	修改坐标系名称
	-----------------------------
	参数:
	name: 新名称
	-----------------------------
	返回:
	true : 修改成功
	false: 字符串长度过长,修改失败
	****************************/
	bool SetName(char *name)
	{
		if(strlen(name) > 8)
		{
			return false;
		}
		else
		{
			strcpy(Name, name);
			return true;
		}		
	}

	/****************************
	char *GetName()
	-----------------------------
	说明:
	提取坐标系名称
	-----------------------------
	返回:
	坐标系名称
	****************************/
	char *GetName()
	{
		return Name;
	}

	/****************************
	Position GetData()
	-----------------------------
	说明: 
	返回data数据
	-----------------------------
	返回:
	返回用于标示坐标系的data数据
	****************************/
	Position GetData()
	{
		return data;
	}

	/****************************
	ToolFrame()
	-----------------------------
	说明:
	构造函数
	****************************/
	ToolFrame()
	{
		strcpy(Name, "");
	}
};

struct UserFrame
{
private:
	char      Name[9]; //坐标系名称
public:
	Position  ORG;     //远点
	Position  _XX;      //X轴上的一点
	Position  XY;      //XY平面上的一点	

public:
	/********************************
	bool SetName(char *name)
	---------------------------------
	说明:
	修改坐标系名称
	---------------------------------
	参数:
	name: 坐标系名称
	---------------------------------
	返回:
	true : 修改名称成功
	false: 名称长度过长,修改失败
	********************************/
	bool SetName(char *name)
	{
		if(strlen(name) > 8)
		{
			return false;
		}
		else
		{
			strcpy(Name, name);
			return true;
		}
	}

	/********************************
	char *GetName()
	---------------------------------
	说明:
	提取坐标系名称
	---------------------------------
	返回:
	坐标系名称
	********************************/
	char *GetName()
	{
		return Name;
	}

	/********************************
	bool SetPoint(int point, Position pos)
	---------------------------------
	说明:
	设定坐标系的三个关键点
	point只能取0、1、2
	---------------------------------
	参数:
	point: 关键点
	       0: ORG点,坐标系的原点
		   1: XX点,X轴上的一点
		   2: XY点,XY平面上的一点
	pos  : 关键点的Position型数据
	---------------------------------
	返回:
	true : 关键点设定成功
	false: point取值有无，关键点设定失败
	********************************/
	bool SetPoint(int point, Position pos)
	{
		switch(point)
		{
		case 0://ORG
			{
				ORG = pos;
				return true;
			}
		case 1://XX
			{
				_XX = pos;
				return true;
			}
		case 2://XY
			{
				XY = pos;
				return true;
			}
		default:break;
		}
		return false;
	}
};