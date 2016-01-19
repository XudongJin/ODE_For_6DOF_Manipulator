//Robot.h 机器人实体的头文件
//2010-7-28 8:40:39

#include "RobotDetail.h"
#include "Macro.h"
#pragma once

struct Robot//所有子对象的定义形式,均为下划线+类名的小写
{
public:
	SafeMode   _safemode;             //安全模式
	int        _runmode;              //运行模式,0为示教,1为再现
	int        _loopmode;             //循环模式,0为单步,1为单循环,2为连续循环
	SpecialRun _specialrun;           //特殊运行
	int        _handspeed;            //手动速度,0为微动,1为低速,2为中速,3为高速
	bool       _softlimit;            //软极限,true为极限有效,false为极限解除
	bool       _alllimit;             //全部极限,true为极限有效,false为极限解除

	int        _frametype;            //坐标类型,0关节坐标,1直角坐标,2工具坐标,3用户坐标
	int        _frameno;              //坐标号码,仅对工具坐标和用户坐标有效
	ToolFrame  _toolframe[_FRAME_NO]; //工具坐标数据
	UserFrame  _userframe[_FRAME_NO]; //用户坐标数据


	Interfere  _axis[_AXIS_SUM];       //轴干涉区
	Interfere  _cube[_CUBE_SUM];       //立方体干涉区

	IO         _GIN;                   //通用输入
	IO         _GOUT;                  //通用输出
	IO         _SIN;                   //专用输入
	IO         _SOUT;                  //专用输出
	IO         _DELAY;                 //继电器

	Position   _AbsoluteOrigin;        //绝对原点
	Position   _WorkOrigin;            //作业原点
	Position   _SecondOrigin;          //第二原点

	Alarm      _alarm;                 //报警信息
 
	Position   _currentposition;       //当前位置

private:
	double     _ManualHigh;            //高速速度值
	double     _ManualMiddle;          //中速速度值
	double     _ManualLow;             //低俗速度值
	double     _ManualSlow;            //微动速度值


public:
	/****************************
	double GetManualSpeed()
	-----------------------------
	根据手动速度等级提取手动速度值
	-----------------------------
	返回 : 对应的手动速度值
	****************************/
	double GetManualSpeed()
	{
		double result=0;//欲返回的值

		switch(this->_handspeed)
		{
		case 0://微动
			{
				result = _ManualSlow;
				break;
			}
		case 1://底速
			{
				
				result = _ManualLow;
				break;
			}
		case 2://中速
			{
				
				result = _ManualMiddle;
				break;
			}
		case 3://高速
			{
				
				result = _ManualHigh;
				break;
			}
		default:break;
		}

		return result;
	}//GetManualSpeed()

	/****************************
	bool SetFrameType(int type)
	-----------------------------
	说明:
	修改坐标系类型
	-----------------------------
	参数:
	type: 坐标系类型数据,只能取0-4
	-----------------------------
	返回:
	true : 修改成功
	false: 输入参数越界,修改失败
	****************************/
	bool SetFrameType(int type)
	{
		if(type<0 || type>4)
		{
			return false;
		}
		else
		{
			_frametype = type;
			return true;
		}		
	}

	/****************************
	bool SetFrameNo(int no)
	-----------------------------
	说明:
	修改坐标系号码
	-----------------------------
	参数:
	no: 坐标系号码,只能取0- _FRAMENO
	-----------------------------
	返回:
	true : 修改成功
	false: 输入参数越界,修改失败
	****************************/
	bool SetFrameNo(int no)
	{
		if(no<0 || no>_FRAME_NO)
		{
			return false;
		}
		else
		{
			_frameno = no;
			return true;
		}
	}

	/****************************
	int GetFrameType()
	-----------------------------
	说明:
	提取坐标系类型
	-----------------------------
	返回:
	坐标系类型数据
	****************************/
	int GetFrameType()
	{
		return _frametype;
	}

	/****************************
	int GetFrameNo()
	-----------------------------
	说明:
	提取工具或者用户坐标系号码
	-----------------------------
	返回:
	坐标系号码
	****************************/
	int GetFrameNo()
	{
		return _frameno;
	}

	/****************************
	int GetLoopmode()
	-----------------------------
	说明:
	提取循环模式
	-----------------------------
	返回:
	循环模式数据
	****************************/
	int GetLoopmode()
	{
		return _loopmode;
	}

	/****************************
	void SetLoopmode(int mode)
	-----------------------------
	说明:
	修改循环模式
	-----------------------------
	参数:
	mode: 循环模式取值,只能取0,1,2
	****************************/
	void SetLoopmode(int mode)
	{
		_loopmode = mode;
	}

	/****************************
	bool GetAlllimit()
	-----------------------------
	说明:
	提取全部极限
	-----------------------------
	返回:
	全部极限信息
	****************************/
	bool GetAlllimit()
	{
		return _alllimit;
	}

	/****************************
	void SetAlllimit(bool value)
	-----------------------------
	说明:
	修改全部极限
	-----------------------------
	参数:
	value: 全部极限是否有效
	       true : 有效
		   false: 无效
	****************************/
	void SetAlllimit(bool value)
	{
		_alllimit = value;
	}

	/****************************
	void SetAlllimit()
	-----------------------------
	说明:
	修改全部极限,将全部极限的值取反
	****************************/
	void SetAlllimit()
	{
		_alllimit = !_alllimit;
	}

	/****************************
	bool GetSoftlimit()
	-----------------------------
	说明:
	提取软极限
	-----------------------------
	返回:
	软极限信息
	****************************/
	bool GetSoftlimit()
	{
		return _softlimit;
	}

	/****************************
	void SetSoftlimit(bool value)
	-----------------------------
	说明:
	修改软极限
	-----------------------------
	参数:
	value: 软极限是否有效
	       true : 有效
		   false: 无效
	****************************/
	void SetSoftlimit(bool value)
	{
		_softlimit = value;
	}

	/****************************
	void SetSoftlimit()
	-----------------------------
	说明:
	修改软极限,将软极限的值取反
	****************************/
	void SetSoftlimit()
	{
		_softlimit = !_softlimit;
	}	

	/****************************
	void AddHandspeed()
	-----------------------------
	说明:
	增大手动速度,每次增加一个等级。在高速情况下，不再增大
	-----------------------------
	****************************/
	void AddHandspeed()
	{
		++ _handspeed;
		if(_handspeed > 3)
		{
			_handspeed = 3;
		}
	}

	/****************************
	void ReduceHandspeed()
	-----------------------------
	说明:
	减小手动速度,每次减小一个等级。在微动情况下，不再减小
	-----------------------------
	****************************/
	void ReduceHandspeed()
	{
		-- _handspeed;
		if(_handspeed < 0)
		{
			_handspeed = 0;
		}
	}

	/****************************
	功能:
	原定位置校对.当前位置与原点位置脉冲差超过允许范围,则认为有误
	-----------------------------
	原型:
	bool CheckOrigin(int type, int error=5)
	-----------------------------
	参数:
	type : 原点类型,0:绝对原点; 1:作业原点; 2:第二原点
	error: 允许的最大误差脉冲数,默认为5
	-----------------------------
	返回值:
	true : 确认无误
	false: 位置有误
	****************************/
	bool CheckOrigin(int type, int error=5)
	{
		switch(type)
		{
		case 0://绝对原点
			{break;}
		case 1://作业原点
			{break;}
		case 2://第二原点
			{
				//任意一个轴组的脉冲差值大于error,则认为有误
				if(abs(this->_SecondOrigin.GetPulse(1) - this->_currentposition.GetPulse(1)) > error)
				{
					return false;
				}
				if(abs(this->_SecondOrigin.GetPulse(2) - this->_currentposition.GetPulse(2)) > error)
				{
					return false;
				}
				if(abs(this->_SecondOrigin.GetPulse(3) - this->_currentposition.GetPulse(3)) > error)
				{
					return false;
				}
				if(abs(this->_SecondOrigin.GetPulse(4) - this->_currentposition.GetPulse(4)) > error)
				{
					return false;
				}
				if(abs(this->_SecondOrigin.GetPulse(5) - this->_currentposition.GetPulse(5)) > error)
				{
					return false;
				}
				if(abs(this->_SecondOrigin.GetPulse(6) - this->_currentposition.GetPulse(6)) > error)
				{
					return false;
				}

				return true;//确认无误
				break;
			}
		default:return false;break;
		}
		return false;
	}


	/****************************
	功能:
	手动修改原点位置数据
	手动指定目标位置
	-----------------------------
	原型:
	bool SetOrigin_Manual(int type, Position value)
	-----------------------------
	参数:
	type : 原点类型,0:绝对原点; 1:作业原点; 2:第二原点
	value: 目标位置
	-----------------------------
	返回值
	true : 修改成功
	false: 修改失败
	****************************/
	bool SetOrigin_Manual(int type, Position value)
	{
		switch(type)
		{
		case 0://绝对原点
			{
				this->_AbsoluteOrigin = value;
				return true;
				break;
			}
		case 1://作业原点
			{
				this->_WorkOrigin = value;
				return true;
				break;
			}
		case 2://第二原点
			{
				this->_SecondOrigin = value;
				return true;
				break;
			}
		default:return false;
		}
		return false;
	}

	/***************************************
	自动设定原点位置数据
	自动将机器人当前位置保存为原点位置
	bool SetOrigin_Auto(int type)
	type: 原点类型,0:绝对原点; 1:作业原点; 2:第二原点
	***************************************/
	bool SetOrigin_Auto(int type)
	{
		switch(type)
		{
		case 0://绝对原点
			{
				this->_AbsoluteOrigin = this->_currentposition;
				return true;break;
			}
		case 1://作业原点
			{
				this->_WorkOrigin = this->_currentposition;
				return true;break;
			}
		case 2://第二原点
			{
				this->_SecondOrigin = this->_currentposition;
				return true;break;
			}
		default:return false;break;
		}
		return false;
	}

	Position GetCurrentPosition()//提取当前位置
	{
		return _currentposition;
	}

	int GetRunMode()//提取运行模式
	{
		return _runmode;
	}

	void SetRunMode(int newMode)//修改运行模式
	{
		_runmode = newMode;
	}

	//轴操作
	//通过按键控制机器人移动,同时修改与之相关的参数
	void Move()
	{
		//机器人移动
		/*在此处添加移动机器人的代码*/

		//修改相应参数
		this->_currentposition = this->RobotCurrentPosition();//修改当前位置
	}

	//获得机器人当前位置
	//从编码器中读取六轴的脉冲值,并以Position类型数据返回
	Position RobotCurrentPosition()
	{
		Position pos;
		//读取脉冲值,修改pos变量
		//模拟读取到的数据
		pos.SetPulse(6, this->_currentposition.GetPulse(6)+5);
		pos.SetPulse(1, this->_currentposition.GetPulse(1)+5);
		pos.SetPulse(2, this->_currentposition.GetPulse(2)+5);
		pos.SetPulse(3, this->_currentposition.GetPulse(3)+5);
		pos.SetPulse(4, this->_currentposition.GetPulse(4)+5);
		pos.SetPulse(5, this->_currentposition.GetPulse(5)+5);

		return pos;
	}

	/********************************************************* 
	    2010-8-4 16:19:38 
	    bool SetInterfere(bool Type, int No, bool Point, Position Value, int Axis=0)
	----------------------------------------------------------
		功能:  设定干涉区数据
	----------------------------------------------------------
		Type:  干涉区类型; true立方体干涉,false轴干涉
		No:    干涉区号码
		Point: Max、Min; true设定Max, false设定Min
		Value: 位置数据		
		Aixs:  仅在轴干涉区时使用,指定欲设定的轴
	*********************************************************/
	bool SetInterfere(bool Type, int No, bool Point, Position Value, int Axis=0)
	{
		bool result = false;
		if(Type)//立方体干涉
		{
			if(No > _CUBE_SUM)
			{
				return false;
			}
			if(Point)//Max
			{
				this->_cube[No].Max = Value;
				return true;
			}
			else//Min
			{
				this->_cube[No].Min = Value;
				return true;
			}			
		}
		else//轴干涉
		{
			if(No > _AXIS_SUM)//越界失败
			{
				return false;
			}

			switch(Axis)
			{
			case 1://S
				{
					if(Point)//Max
					{
						this->_axis[No].Max.SetPulse(1, this->_currentposition.GetPulse(1));
					}
					else
					{
						this->_axis[No].Min.SetPulse(1, this->_currentposition.GetPulse(1));
					}					
					return true;
					break;
				}
			case 2://L
				{
					if(Point)//Max
					{
						this->_axis[No].Max.SetPulse(2, this->_currentposition.GetPulse(2));
					}
					else
					{
						this->_axis[No].Min.SetPulse(2, this->_currentposition.GetPulse(2));
					}					
					return true;
					break;
				}
			case 3://U
				{
					if(Point)//Max
					{
						this->_axis[No].Max.SetPulse(3, this->_currentposition.GetPulse(3));
					}
					else
					{
						this->_axis[No].Min.SetPulse(3, this->_currentposition.GetPulse(3));
					}					
					return true;
					break;
				}
			case 4://R
				{
					if(Point)//Max
					{
						this->_axis[No].Max.SetPulse(4, this->_currentposition.GetPulse(4));
					}
					else
					{
						this->_axis[No].Min.SetPulse(4, this->_currentposition.GetPulse(4));
					}					
					return true;
					break;
				}
			case 5://B
				{
					if(Point)//Max
					{
						this->_axis[No].Max.SetPulse(5, this->_currentposition.GetPulse(5));
					}
					else
					{
						this->_axis[No].Min.SetPulse(5, this->_currentposition.GetPulse(5));
					}					
					return true;
					break;
				}
			case 6://T
				{
					if(Point)//Max
					{
						this->_axis[No].Max.SetPulse(6, this->_currentposition.GetPulse(6));
					}
					else
					{
						this->_axis[No].Min.SetPulse(6, this->_currentposition.GetPulse(6));
					}					
					return true;
					break;
				}
			default:
				return false;
				break;
			}
	
		}
		return result;
	}

	/*******************************
	2010-8-4
	构造函数
	*******************************/
	Robot()
	{
		//初始化手动速度
		this->_ManualHigh = _MANUAL_HIGH;
		this->_ManualMiddle = _MANUAL_MIDDLE;
		this->_ManualLow = _MANUAL_LOW;
		this->_ManualSlow = _MANUAL_SLOW;

		//初始化运行模式为"示教"
		_runmode = 0;

		//手动速度初始化为低速
		_handspeed = 1;

		//软极限、全部极限均初始化为有效
		_softlimit = true;
		_alllimit = true;

		//初始化轴干涉区
		for(int i=0; i<_AXIS_SUM; ++i)
		{
			_axis[i].SetType(false);
			_cube[i].SetType(true);
		}
	}
};

//extern Robot ROBOT;