//:CommandLine.h 命令行类的定义
//2010-9-28 11:12:30
//2010-7-9 23:07:03
#include "Macro.h"
#include "Position.h"
#pragma once

//命令行，客户程序的基本组成单元
class CommandLine
{
private:
	//2010-9-28 10:49:20
	char Name[_CMD_NAME_LENGTH];                                //命令名称
	int iVariable1 ,iVariable2 ,iVariable3 ;                    //int型参数
	double dVariable;                                           //double型参数
	bool bVariable;                                             //bool型变量
	char strVariable1[_CMD_STR_LENGTH] ,strVariable2[_CMD_STR_LENGTH] 
		,strVariable3[_CMD_STR_LENGTH],strVariable4[_CMD_STR_LENGTH]; 
	                                                            //string型参数
	char Note[_CMD_NOTE_LENGTH];                                //“注释”命令的专用参数
public:
	Position Target;                                            //目标位姿
	
public:
	//构造函数
	//2010-9-28 10:49:27
	CommandLine()                   
	{

		strcpy(Name, "");                                        //命令名初始化为空串
		strcpy(strVariable1, "");  strcpy(strVariable2, "");     //string型参数初始化为空串
		strcpy(strVariable3, "");  strcpy(strVariable4, "");
		strcpy(Note, ""); 		 
		dVariable = 0.0;                                         //double型参数初始化为0.0
		iVariable1 = iVariable2 = iVariable3 = 0;                //int型参数初始化为0
			  
		bVariable = true;                                      //bool型参数初始化为true

	}

	/***************************************
	拷贝构造函数
	***************************************/
	CommandLine(CommandLine &cmd)
	{
		strcpy(Name, cmd.GetName());
		iVariable1 = cmd.GetiVariable(1); iVariable1 = cmd.GetiVariable(2); iVariable1 = cmd.GetiVariable(3);
		dVariable = cmd.GetdVariable();
		bVariable = cmd.GetbVariable();
		strcpy(strVariable1, cmd.GetstrVariable(1)); strcpy(strVariable2, cmd.GetstrVariable(2)); 
		strcpy(strVariable3, cmd.GetstrVariable(3)); strcpy(strVariable4, cmd.GetstrVariable(4));
		strcpy(Note, cmd.GetstrVariable(0));
		Target = cmd.Target;
	}//CommandLine(CommandLine& cmd)

	//2010-9-28 10:49:47
	void Clear()                    //清空命令
	{
		strcpy(strVariable1, "");  strcpy(strVariable2, "");          //string型参数初始化为空串
		strcpy(strVariable3, "");  strcpy(Note, ""); 
		this->dVariable = 0.0;                                        //dobule型参数清空为0.0
		this->iVariable1 = this->iVariable2 = this->iVariable3 = 0;   //int型参数初始化为0
		this->bVariable = true;                                       //bool型参数初始化为true
	}

	//2010-9-28 10:49:57
	char * GetName()                                             //提取命令名
	{
		return Name;
	}

	//2010-9-28 10:50:04
	void SetName(char * newName)                                //修改命令名
	{
		strcpy(Name, newName);
		this->Clear();                                          //修改命令名后，清空各参数
		this->InitCommand();                                    //修改命令之后,重新对参数进行初始化
	}

	//2010-9-28 10:50:10
	double GetdVariable()                                       //提取double型参数
	{
		return dVariable;
	}

	//2010-9-28 10:50:16
	void SetdVariable(double value)                            //修改double型参数
	{
		dVariable = value;
	}

	//2010-9-28 10:50:25
	int GetiVariable(int No)                                   //提取int型参数
	{
		int result;
		switch(No)
		{
		case 1:
			result = this->iVariable1;break;
		case 2:
			result = this->iVariable2;break;
		case 3:
			result = this->iVariable3;break;
		default:break;
		}
		return result;
	}

	//2010-9-28 10:50:31
	void SetiVariable(int No, int value)                       //修改int型参数
	{
		switch(No)
		{
		case 1:
			this->iVariable1 = value;break;
		case 2:
			this->iVariable2 = value;break;
		case 3:
			this->iVariable3 = value;break;
		default:break;
		}
	}


	//2010-9-28 10:50:45
	bool GetbVariable()                //提取bool型参数
	{
		return bVariable;
	}


	//2010-9-28 10:52:17
	void SetbVariable(bool value)      //修改bool型参数
	{
		this->bVariable = value;
	}

	//2010-9-28 10:52:51
	char * GetstrVariable(int No)      //提取str型参数
	{
		char * result;
		switch(No)
		{
		case 0:
			result = Note;break;
		case 1:
			result = strVariable1;break;
		case 2:
			result = strVariable2;break;
		case 3:
			result = strVariable3;break;
		case 4:
			result = strVariable4;break;
		default:break;
		}
		return result;
	}
	void SetstrVariable(int No, char * value) //修改str型参数
	{
		switch(No)
		{
		case 0:
			strcpy(Note, value);break;
		case 1:
			strcpy(strVariable1, value);break;
		case 2:
			strcpy(strVariable2, value);break;
		case 3:
			strcpy(strVariable3, value);break;
		case 4:
			strcpy(strVariable4, value);break;
		default:break;
		}
	}

	//2010-9-28 10:53:12,待完成
	/*******************************************
	bool InitCommand()
	说明：对命令进行初始化,对不同的命令,设置不同的初始参数
	返回：true :初始化成功
	      false:初始化失败
	*******************************************/
	bool InitCommand()              //命令初始化
	{   //对不同的命令，设置不同的初始参数值
		//目标命令名
		//const char TargetOrderName[][9]	
		
		//得到命令名称及其编号，保存在OrderNo
		int OrderNo,OrderTotal;
		OrderTotal = 63;//Total为命令总数，新增命令，需修改此值
		for(int i=0; i<OrderTotal; i++)
		{
			if(strcmp(Name,TargetOrderName[i])==0)
			{
				OrderNo = i;
				break;
			}
		}
		
		//对str1变量的初始化，均为操作说明书中该命令出现的第一个取值
		switch(OrderNo)
		{
		case 0:{break;}//START
		case 1:{break;}//END
	   /*************移动命令***************************/ 
		case 2:     //MOVJ
		case 3:     //MOVL
		case 4:     //MOVC
			{
				this->SetiVariable(1, 0);//int1 = 0,P<00>
				this->SetdVariable(10);//double = 10,V<10>
				this->SetiVariable(2, 0);//int2=0;Z<0>
				break;
			}
	   /*********************信号处理*************************/
		case 5:     //SET
			{
				this->SetiVariable(1, 0);//OUT<00>
				this->SetbVariable(false);//OFF
				break;
			}
		case 6:    //WAIT
			{
				this->SetiVariable(1, 0);//IN<2>
				this->SetbVariable(false);//OFF
				this->SetstrVariable(1,"T");//T
				this->SetdVariable(10);//T<10>
				break;
			}
		case 7:     //DELAY
			{
				this->SetdVariable(10);//T<10>
				break;
			}
		case 8:     //L
			{
				//this->SetiVariable(1,0);//L<0>
				this->SetstrVariable(1, "0");
				break;
			}
		case 9:     //GOTO
			{
				//this->SetiVariable(1, 0);//L<0>
				this->SetstrVariable(1, "0");

				this->SetstrVariable(2, "");//默认没有IF
				this->SetstrVariable(3, "IN");//IN
				this->SetiVariable(1, 0);//IN<0>
				this->SetstrVariable(4, "==");
				this->SetbVariable(true);
				this->SetiVariable(2, 0);
				break;
			}
		case 10:     //#
			{
				break;
			}
		case 11:     //CALL
			{
				this->SetstrVariable(1, "");
				break;
			}
		case 12:     //RET
			{
				break;
			}

		/*****************************运算命令*****************************/
		case 13:     //DECR
		case 14:     //INCR
			{
				this->SetstrVariable(1, "R");
				this->SetiVariable(1, 0);//R<0>假设变量是从0开始编号。
				break;
			}
		case 15:     //R
			{
				this->SetiVariable(1, 0);
				break;
			}

		/**************************平移命令******************************/ 
		case 16:     //PR
			{
				this->SetiVariable(1, 0);
				this->SetiVariable(1, 0);
				this->SetstrVariable(1, "");
				this->SetiVariable(3, 0);
				break;
			}
		case 17:     //SHIFTON
			{
				this->SetiVariable(1, 0);//PR<0>
				break;
			}
		case 18:     //SHIFTOFF
			{
				break;
			}
		case 19:     //MISHIFT
			{
				this->SetiVariable(1, 0);
				this->SetiVariable(2, 0);
				this->SetiVariable(3, 0);
				break;
			}					
			
		default:break;
		}
		return true;
}

};
