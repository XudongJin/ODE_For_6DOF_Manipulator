//: Execute.h
//"程序"运行的相关函数
#pragma once
#include "Robot.h"
#include "Program.h"
#include "Variable.h"

/***********************************************
int Excute(Program *pro, CommandLine cmd, int cmdNo, Robot &ROBOT, UserVariable &VARIABLE, int &go)
------------------------------------------------
执行单行命令
------------------------------------------------
pro       : 指向正在执行的客户程序的指针
cmd       : 指向当前命令行的指针
ROBOT     : 此处参数为全局变量ROBOT
VARIABLE  : 此处参数为全局变量VARIABLE
go        : 要执行的下一条命令的下标
------------------------------------------------
返回 : 
0  : 执行成功
其他 : 执行失败,返回值是错误代码
***********************************************/
int Excute(Program *pro, CommandLine cmd, Robot &ROBOT, UserVariable &VARIABLE, int &go)
{
	int result = 0;//执行结果,预先赋值为0
	
	//得到命令名称及其编号，保存在OrderNo
	int OrderNo;
	for(int i=0; i<_CMD_SUM; i++)
	{
		if(strcmp(cmd.GetName(), TargetOrderName[i])==0)
		{
			OrderNo = i;
			break;
		}
	}
	
	//根据命令名称编号,执行不同的动作
	switch(OrderNo)
	{
	case 0:     //START
		{//done
			++go;
			break;
		}
	case 1:     //END
		{//done
			go = pro->GetLineNumber();//保证程序正常退出
			//return 0;//程序结束
			break;
		}
		//////////////////运动命令
	case 2:     //MOVJ
		{//done
			//////////////////////////////////////////////////////////////////////////
			//在此添加移动机器人的代码
			//////////////////////////////////////////////////////////////////////////
			++go;
			break;
		}
	case 3:     //MOVL
		{//done
			//////////////////////////////////////////////////////////////////////////
			//在此添加移动机器人的代码
			//////////////////////////////////////////////////////////////////////////
			++go;
			break;
		}
	case 4:     //MOVC
		{//done
			//////////////////////////////////////////////////////////////////////////
			//在此添加移动机器人的代码
			//////////////////////////////////////////////////////////////////////////
			++go;
			break;
		}
		//////////////////信号处理
	case 5:     //SET
		{//done
			bool value = cmd.GetbVariable();//得到ON/OFF
			int No = cmd.GetiVariable(1);
			ROBOT._GOUT.io[No].SetValue(value);//修改IO
			
			++go;
			break;
		}
	case 6:     //WAIT
		{//done
			int No = cmd.GetiVariable(1);//io号码
			if (!strcmp(cmd.GetstrVariable(1), ""))//没有T,无限时等待
			{
				for (;;)
				{
					if ((cmd.GetbVariable() && ROBOT._SIN.io[No].GetValue())//同为true
						|| (!cmd.GetbVariable() && !ROBOT._SIN.io[No].GetValue()))//同为false
					{//已经满足等待条件
						++go;
						break;
					}
				}
			} 
			else if (!strcmp(cmd.GetstrVariable(1), "T"))//有T,限时等待
			{
				for (int i=0; i<_DELAY_1S*cmd.GetdVariable(); ++i)
				{
					if ((cmd.GetbVariable() && ROBOT._SIN.io[No].GetValue())//同为true
						|| (!cmd.GetbVariable() && !ROBOT._SIN.io[No].GetValue()))//同为false
					{//已经满足IN通道等待条件
						break;
					}
				}
				
				//为满足IN通道等待条件,但等待时间已到
				++go;
			} 
			else//出错
			{
				return 60;//60号错误,WAIT命令的str1参数有误
			}					
			
			//++go;
			break;
		}
	case 7:     //DELAY
		{
			double t = cmd.GetdVariable();//得到延时的时长
			////////////////////////////////////////////////////////////////////
			//此处添加延时程序
			//Time(t);//延时程序
			////////////////////////////////////////////////////////////////////			
			
			++go;
			break;
		}
		/////////////////流程控制
	case 8:     //L
		{//done
			break;
		}
	case 9:     //GOTO
		{//done
			char Target[_CMD_STR_LENGTH];
			strcpy(Target, cmd.GetstrVariable(1));//得到跳转目的
			for (int i=0; i<pro->GetLineNumber(); ++i)
			{
				if (!strcmp(Target, pro->commands[i].GetName()))//判断是否为跳转地址
				{
					go = i;
					break;//执行成功
				}
			}
			return 90;//错误信息90 : 跳转目的不存在
			break;
		}
	case 10:     //#
		{//done
			break;
		}
	case 11:     //CALL
		{//done
			break;
		}
	case 12:     //RET
		{//done
			break;
		}
		/////////////////运算命令
	case 13:     //DECR
		{//done
			int No = cmd.GetiVariable(1);//得到欲修改的变量号码
			VARIABLE._R[No] -= 1;        //自减1
			++go;
			break;
		}
	case 14:     //INCR
		{//done
			int No = cmd.GetiVariable(1);//得到欲修改的变量号码
			VARIABLE._R[No] += 1;        //自加1
			++go;
			break;
		}
	case 15:     //R
		{
			VARIABLE.R = cmd.GetiVariable(1);//赋值给全局R变量
			
			++go;
			break;
		}
		//////////////////平移命令
	case 16:     //PR
		{
			if (!strcmp(cmd.GetstrVariable(1), ""))//str2为空,没有+/-
			{
				VARIABLE._PR[cmd.GetiVariable(1)] = VARIABLE._PR[cmd.GetiVariable(2)];
			} 
			else if (!strcmp(cmd.GetstrVariable(1), "+"))//str2为+
			{
				POStype pr = VARIABLE._PR[cmd.GetiVariable(2)];
				pr += VARIABLE._PR[cmd.GetiVariable(3)];
				VARIABLE._PR[cmd.GetiVariable(1)] = pr;
				//VARIABLE._PR[cmd.GetiVariable(1)] += VARIABLE._PR[cmd.GetiVariable(2)];
			}
			else if (!strcmp(cmd.GetstrVariable(1), "-"))//str2为-
			{
				VARIABLE._PR[cmd.GetiVariable(1)] -= VARIABLE._PR[cmd.GetiVariable(2)];
			}
			else//出错
			{
				return 160;//160号错误,PR变量str2有误
			}					
			
			++go;
			break;
		}
	case 17:     //SHIFON
		{
			//////////////////////////////////////////////////////////////////////////
			//在此添加平移开始代码
			//////////////////////////////////////////////////////////////////////////
			++go;
			break;
		}
	case 18:     //SHIFOFF
		{
			//////////////////////////////////////////////////////////////////////////
			//在此添加平移结束代码
			//////////////////////////////////////////////////////////////////////////
			++go;
			break;
		}
	case 19:     //MISHIFT
		{
			int PR_No = cmd.GetiVariable(1);//PR变量号
			int P_No1 = cmd.GetiVariable(2);//P变量1
			int P_No2 = cmd.GetiVariable(3);//P变量2
			
			VARIABLE._P[P_No2] -= VARIABLE._P[P_No1];
			VARIABLE._PR[PR_No] = VARIABLE._P[P_No2];
			
			++go;
			break;
		}
		
	default:break;
	}//switch			
			return result;
}//Excute()
		
		
 /***********************************************
 int Run(Program *pro, Robot &ROBOT, UserVariable &VARIABLE)
 ------------------------------------------------
 全速执行一遍程序
 ------------------------------------------------
 pro       : 欲执行的程序
 &ROBOT    : 此处为全局变量ROBOT
 &VARIABLE : 此处为全局变量VARIABLE
 ------------------------------------------------
 返回 : 
 0 : 程序执行成功
 其他 : 程序执行失败,返回失败代码
 ***********************************************/
 int Run(Program *pro, Robot &ROBOT, UserVariable &VARIABLE)
 {
	 int result = 0;	
	 
	 int i=0;
	 for (; i<pro->GetLineNumber(); )//从第一行命令到最后一行
	 {
		 int Last = i;//保存上次运行的行号
		 result = Excute(pro, pro->commands[i], ROBOT, VARIABLE, i);//执行单行程序
		 if(result>0)//失败退出
		 {
			 break;
		 }		 
	 }	 
	 return result;
 }//Run()
		 
		 
  /***********************************************
  int Step(Program *pro, Robot &ROBOT, UserVariable &VARIABLE)
  单步执行程序
  ------------------------------------------------
  pro    : 欲执行的程序
  ROBOT  : 全局变量::ROBOT
  VARIABLE : 全局变量::VARIABLE
  ------------------------------------------------
  返回:
  0  : 运行成功
  非0 : 运行失败
 ***********************************************/
 int Step(Program *pro, Robot &ROBOT, UserVariable &VARIABLE)
 {		 
	 int result = 0;//运行结果
	 //////////////////////////////////////////////////////
	 static int CurrentLine = 0;//当前执行的命令行
	 
	 bool End = false;//是否执行到最后一行程序
	 if(pro->GetLineNumber() == CurrentLine)//执行到最后一行
	 {
		 End = true;
	 }	
	 
	 result = Excute(pro, pro->commands[CurrentLine], ROBOT, VARIABLE, CurrentLine);
	 //////////////////////////////////////////////////////
	 
	 if(End)//执行到最后一行
	 {
		 CurrentLine = 0;//修改当前成续行
	 }
	 
	 return result;
 }//int Step()
