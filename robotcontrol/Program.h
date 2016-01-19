//Program.h 客户程序
//2010-7-9 23:12:13 
#pragma once

#include "head.h"
//#include "BOX.h"
#include "string.h"

//将字符串转化为int.用于CmdEdit函数
#define   _TOint      atoi(EditSourceOrder.c_str())
//将字符串转化为double.用于CmdEdit函数
#define   _TOdouble   atof(EditSourceOrder.c_str())
//比较字符串是否相同,用于CmdEdit函数
#define   _STRCMP(x)  strcmp(EditSourceOrder.c_str(), x) 

//程序结构体，“客户程序”的数据集是由“命令行”构成的一个数组
struct Program
{
	public:
		CommandLine commands[_VALID_CMDLINE * _NEST_DEEPTH];//最多100 * 10行命令
		
	private:		
		char   Name[_PRO_NAME_LENGTH];      //程序名
		char   Note[_PRO_NOTE_LENGTH];     //程序注释
		int    LineNumber;   //行数
		int    ToolNumber;   //工具号码
		bool   Lock;		 //编辑锁定
		
		int Date[3];     //创建日期
		int Size;        //程序大小

	private:
		/****************************
		bool MoveCmd(int start, int end, int dis, bool Upward)
		-----------------------------
		移动程序中的命令,从start行到end行,依次移动dis距离
		-----------------------------
		start : 移动的起始行
		end   : 移动的结束行
		dis   : 移动的距离
		Upward: 移动方向
		    true : 向程序尾移动
			false: 向程序头移动
		-----------------------------
		返回 :
		true : 移动成功
		false: 移动失败
		****************************/
		bool MoveCmd(int start, int end, int dis, bool Upward)
		{
			for(int i=0; i<dis; ++i)//记录移位的次数
			{
				if(Upward)//向程序尾移动
				{
					for(int j=end; j>=start; --j)//选中需要移动的范围
					{
						this->commands[j+1] = this->commands[j];
						++this->LineNumber;
					}					
				}
				else//向程序头移动
				{
					for(int j=start; j<=end; ++j)
					{
						this->commands[j] = this->commands[j+1];
						--this->LineNumber;
					}
				}			
			}		

			return true;
		}
		
	public:

		/*******************************
		 string GetStack()
		 提出程序堆栈
		 返回 : 本程序所调用的所有子程序名连接成的string
		*******************************/
		string GetStack()
		{
			string result;
			for(int i=0; i<LineNumber; ++i)
			{
				if(!strcmp(commands[i].GetName(), "CALL"))//调用子程序命令
				{
					result += commands[i].GetstrVariable(1);
					result += "\r\n";
				}
			}
			return result;
		}//GetStack()


		Program()                       //构造函数
		{
			this->InitProgram();
		}

		/****************************
		Program(char *name)
		-----------------------------
		带参构造函数
		****************************/
		Program(char *name)
		{
			this->InitProgram();//初始化程序
			strcpy(Name, name);//修改名字
		}

		/****************************
		bool Save()
		-----------------------------
		将本Program写入硬盘保存
		-----------------------------
		返回 : 
		true : 保存成功
		false: 保存失败
		****************************/
		bool Save()
		{
			//保存路径
			char Location[50];
			sprintf(Location, _PROGRAM_LOCATION, this->Name);
			//开文件
			FILE *mypro;
			mypro = fopen(Location, "w");
			if(NULL == mypro)
			{
				return false;
			}

			//写入数据
			fwrite(this, sizeof(*this), 1, mypro);

			//关文件
			fclose(mypro);
			return true;			
		}

		/****************************
		bool Read()
		-----------------------------
		从硬盘中读取同名程序数据
		-----------------------------
		返回 :
		true : 读取成功
		false: 读取失败
		****************************/
		bool Read()
		{
			bool result;
			result = this->Read(this->Name);
			return result;
		}

		/****************************
		bool Read(char *name)
		-----------------------------
		从硬盘中将Program读取到内存
		-----------------------------
		name : 欲打开的程序名
		-----------------------------
		返回 : 
		true : 读取成功
		false: 读取失败
		****************************/
		bool Read(char *name)
		{
			//读取路径
			char Location[50];
			sprintf(Location, _PROGRAM_LOCATION, name);
			//打开文件
			FILE *mypro;
			mypro = fopen(Location, "r");
			if(NULL == mypro)
			{
				return false;
			}

			//读取数据
			fread(this, sizeof(*this), 1, mypro);

			//关闭文件
			fclose(mypro);
			return true;
		}
		
		/****************************
		int GetToolNo()
		-----------------------------
		说明:
		提取工具号码
		-----------------------------
		返回:
		工具号码
		****************************/
		int GetToolNo()
		{
			return ToolNumber;
		}
		
		/*****************************
		bool SetToolNo(int No)
		------------------------------
		说明:
		修改工具号码
		------------------------------
		参数:
		No : 工具号码
		------------------------------
		返回:
		true : 修改成功
		false: 输入号码越界,修改失败
		*****************************/
		bool SetToolNo(int No)
		{
			if((No>=0) && (No<_FRAME_NO))
			{
				ToolNumber = No;
				return true;
			}
			else
			{
				return false;
			}			
		}
		
		/***********************************
		void InitProgram()
		------------------------------------
		对程序进行初始化
		***********************************/
		void InitProgram()              //初始化客户程序
		{//创建一个只包含两行"命令行"的数组
			commands[0].SetName("START");
			commands[1].SetName("END");
			LineNumber = 2;
			ToolNumber = 0;
			
			strcpy(Name, "");
			strcpy(Note, "");
			Lock = false;
			
			//	return true;
		}

		/***************************************
		bool InsertCommand(int No, CommandLine cmd)
		将一条编辑好的命令新增至程序,在第No行之后
		---------------- 参数 ------------------
		No : 插入位置,命令在commands数组中的下标
		cmd: 新命令
		---------------- 返回 ------------------
		true : 插入成功
		false: 插入失败
		***************************************/
		bool InsertCommand(int No, CommandLine cmd)
		{			
			if(_VALID_CMDLINE == LineNumber)//LineNumer已达到最大，则失败
			{
				return false;
			}
			if(No+1 >= LineNumber)//插入位置超出LineNumer范围，则失败
			{
				return false;
			}
			else
			{
				for(int i=LineNumber-1; i>No; --i)//依次后移
				{
					commands[i+1] = commands[i];
				}
				commands[No+1] = cmd;//填充命令名
				
				++LineNumber;
				return true;
			}
		}//InsertComamnd()
		
		/***************************************
		bool InsertCommand(int No, char  *newcmd)
		为程序插入一条空命令,在第No行之后
		-------------- 参数 --------------------
		No    : 插入位置,命令在数组中的下标
		newcmd: 新增命令的命令名
		-------------- 返回 --------------------
		true : 插入命令成功
		false: 插入命令失败
		***************************************/
		bool InsertCommand(int No, char * newcmd) //插入命令
		{//此处No为命令在数组中的下标
			if(_VALID_CMDLINE == LineNumber)//LineNumer已达到最大，则失败
			{
				return false;
			}
			if(No+1 >= LineNumber)//插入位置超出LineNumer范围，则失败
			{
				return false;
			}
			else
			{
				for(int i=LineNumber-1; i>No; --i)//依次后移
				{
					commands[i+1] = commands[i];
				}
				commands[No+1].SetName(newcmd);//填充命令名
				
				++LineNumber;
				return true;
			}
		}//InsertCommand()

		/**************************************************
		bool DeleteCommand(int No)
		---------------------------------------------------
		删除一行程序
		---------------------------------------------------
		No : 欲删除的命令在commands[]中的下标
		---------------------------------------------------
		返回 :
		true : 删除成功
		false: 删除失败
		**************************************************/
//		bool DeleteCommand(
//		{
//			return true;
//		}
		
		//2010-9-28 11:32:22
		/**************************************************
		bool DeleteCommand(int No)
		---------------------------------------------------
		说明：删除一行命令
		---------------------------------------------------
		返回 :
		true : 删除成功
		false: 删除失败
		**************************************************/
		bool DeleteCommand(int No)         //删除命令
		{//此处No为命令在数组中的下标
			if(No+1 >= LineNumber)//删除位置超出LineNumer范围，则失败
			{
				return false;
			}
			else if(!No)//不能删除第0行程序
			{
				return false;
			}
			else
			{
				for(int i=No; i<LineNumber; ++i)
				{
					commands[i] = commands[i+1];//后续命令覆盖之前命令			
				}
				--LineNumber;
			}
			commands[LineNumber+1].SetName("");//原有的最后一行命令(END),修改为空命令
			return true;
		}

		char * GetName()                //提取程序名
		{
			return Name;
		}

		void SetName(char * newname)            //修改程序名
		{
			strcpy(Name, newname);
		}

		char * GetNote()                //提取程序注释
		{
			return Note;
		}

		void SetNote(char * newnote)            //修改程序注释
		{
			strcpy(Note, newnote);
		}

		int GetLineNumber()             //提取命令行数
		{
			return LineNumber;
		}

		void Clear()                   //清空程序
		{
			this->InitProgram();
		}

		//2010-9-28 15:38:32，待完成
		/*****************************************
		string Show()
		------------------------------------------
		说明:
		将程序转化为string返回,用于显示
		------------------------------------------
		返回:
		所有命令组合而成的string对象
		*****************************************/
		string Show()                    //程序显示
		{//程序显示,将程序转化为字符串返回
			string Result;

			for(int No_Buffer=0; No_Buffer<LineNumber; ++No_Buffer)
			{


				//得到命令名称及其编号，保存在OrderNo
				int OrderNo;
				CommandLine *command;
				for(int i=0; i<_CMD_SUM; i++)
				{
					if(strcmp(commands[No_Buffer].GetName(), TargetOrderName[i])==0)
					{
						command = &commands[No_Buffer];
						OrderNo = i;
						break;
					}
				}

				//根据命令名称及编号，确定要显示的内容
				char strOrder[2000]; 
				switch(OrderNo)
				{
				case 0:		//NOP
					{
						//strOrder.Format(_T("%d	NOP"),No_Buffer);
						sprintf(strOrder, "%d	START\r\n", No_Buffer);
						break;
					}
				case 1:		//END
					{
						//	strOrder.Format(_T("%d END"),No_Buffer);
						sprintf(strOrder, "%d	END", No_Buffer);

						//到达END行，强制结束，直接送显
						Result += strOrder;
						return Result;
						break;
					}			
					/////////////////////////////移动命令//////////////////////////////////
				case 2:     //MOVJ
				case 3:     //MOVL
				case 4:     //MOVC
					{
						//获取命令名
						/*char name[9];*/
						sprintf(strOrder, "%d	%s  ", No_Buffer, command->GetName());
						//获取点、速度、精度
						char detail[100];
						sprintf(detail, "P%d,  V%.2f,  Z%d\r\n", command->GetiVariable(1), command->GetdVariable(), command->GetiVariable(2));
						strcat(strOrder, detail);
						break;
					}

					////////////////////////////////信号处理//////////////////////////////
				case 5:     //SET
					{
						//通道号
						sprintf(strOrder, "%d	SET  OUT%d,  ", No_Buffer, command->GetiVariable(1));
						//ON/OFF
						char detail[10];
						if(command->GetbVariable())
						{
							strcpy(detail, "ON\r\n");
						}
						else
						{
							strcpy(detail, "OFF\r\n");
						}

						strcat(strOrder, detail);
						break;
					}
				case 6:     //WAIT
					{
						//通道号
						sprintf(strOrder, "%d	WAIT  IN%d == ", No_Buffer, command->GetiVariable(1));
						//ON/OFF
						char in[10];
						if(command->GetbVariable())
						{
							strcpy(in, "ON");
						}
						else
						{
							strcpy(in, "OFF");
						}
						strcat(strOrder, in);

						//T<10>
						char T[10];
						if(!strcmp(command->GetstrVariable(1), ""))//为空
						{
							strcpy(T, "\r\n");
						}
						else if(!strcmp(command->GetstrVariable(1), "T"))//T
						{
							sprintf(T, ",  T%.2f\r\n", command->GetdVariable()); 
						}
						else
						{
							sprintf(strOrder, "%d	WAIT  error1\r\n");
							break;
						}						
						strcat(strOrder, T);

						break;
					}

				case 7:     //DELAY
					{
						sprintf(strOrder, "%d	DELAY  T%.2f\r\n", No_Buffer, command->GetdVariable());
						break;
					}

					//////////////////////////流程控制////////////////////////////////
				case 8:     //L
					{
						sprintf(strOrder, "%d  L %s :\r\n", No_Buffer, command->GetstrVariable(1));
						break;
					}
				case 9:     //GOTO
					{
						//GOTO L0
						sprintf(strOrder, "%d	GOTO  L%s", No_Buffer, command->GetstrVariable(1));
						//
						char IF[30];
						if(!strcmp(command->GetstrVariable(2), ""))//为空,没有IF
						{
							strcpy(IF,"\r\n");
						}
						else if(!strcmp(command->GetstrVariable(2), "IF"))//有IF
						{
							sprintf(IF, ",  IF  %s%d %s ", command->GetstrVariable(3), command->GetiVariable(1), command->GetstrVariable(4));

							//ON/4
							char end[10];
							if(!strcmp(command->GetstrVariable(3), "IN"))
							{
								if(command->GetbVariable())//ON
								{
									strcpy(end, "ON\r\n");
								}
								else
								{
									strcpy(end, "OFF\r\n");
								}
							}
							else if(!strcmp(command->GetstrVariable(3), "R"))
							{
								sprintf(end, "%d\r\n", command->GetiVariable(2));
							}
							else//出错
							{
								sprintf(strOrder, "%d	GOTO  error1", No_Buffer);
								break;
							}

							strcat(IF, end);
						}

						strcat(strOrder, IF);
						break;
					}

				case 10:     //#
					{
						sprintf(strOrder, "%d  #%s\r\n", No_Buffer, command->GetstrVariable(0));
						break;
					}
				case 11:     //CALL
					{
						sprintf(strOrder, "%d	CALL  %s\r\n", No_Buffer, command->GetstrVariable(1));
						break;
					}
				case 12:     //RET
					{
						sprintf(strOrder, "%d	RET\r\n", No_Buffer);
						break;
					}

					//////////////////////////运算命令///////////////////////////////
				case 13:     //DECR
				case 14:     //INCR
					{
						sprintf(strOrder, "%d	%s  R%d\r\n", No_Buffer, command->GetName(), command->GetiVariable(1));
						break;
					}
				case 15:     //R
					{
						sprintf(strOrder, "%d	R = %d\r\n", No_Buffer, command->GetiVariable(1));
						break;
					}

					/////////////////////////平移指令/////////////////////////////////
				case 16:     //PR
					{
						//PR0 = PR0
						sprintf(strOrder, "%d	PR%d = PR%d", No_Buffer, command->GetiVariable(1), command->GetiVariable(2));

						//PR0 = PR0 - PR0
						char end[20];
						if(!strcmp(command->GetstrVariable(1), ""))//空
						{
							strcpy(end, "\r\n");
						}
						else if(!strcmp(command->GetstrVariable(1), "-"))
						{
							sprintf(end, " - PR%d\r\n", command->GetiVariable(3));
						}
						else if(!strcmp(command->GetstrVariable(1), "+"))
						{
							sprintf(end, " + PR%d\r\n", command->GetiVariable(3));
						}
						else
						{
							sprintf(strOrder, "%d	PR error1\r\n", No_Buffer);
							break;
						}

						strcat(strOrder, end);
						break;
					}
				case 17:     //SHIFTON
					{
						sprintf(strOrder, "%d	SHIFTON  PR%d\r\n", No_Buffer, command->GetiVariable(1));
						break;
					}
				case 18:     //SHIFTOFF
					{
						sprintf(strOrder, "%d	SHIFTOFF\r\n", No_Buffer);
						break;
					}
				case 19:     //MISHIFT
					{
						sprintf(strOrder, "%d	MISHIFT  PR%d,  P%d,  P%d\r\n", No_Buffer, command->GetiVariable(1), command->GetiVariable(2), command->GetiVariable(3));
						break;
					}
							 



					/////在此处添加新的case，以编辑新的命令//////////////////////////

				default:break;
				}
				Result += strOrder;
			}
			return Result;
		}

		/**************************************************
		bool CmdEdit(int CommandNo, int MemberNo, string EditSourceOrder, const Robot &robot)
		---------------------------------------------------
		说明:
		修改程序中的命令
		---------------------------------------------------
		参数:
		CommandNo       : 命令行号码
		MemberNo        : 欲修改的参数位置
		EditSourceOrder : 修改源命令
		cons &robot     : 全局变量ROBOT
		---------------------------------------------------
		返回:
		true : 修改成功
		false: 修改失败
		**************************************************/
		bool CmdEdit(int CommandNo, int MemberNo, string EditSourceOrder,  Robot &robot)
		{//输入命令行号、欲修改参数位置号、修改源命令

			CommandLine *command = & this->commands[CommandNo];//指针指向欲修改的命令

			const char *InputOrder = EditSourceOrder.c_str();
			char InputOrder_Buffer[9];
			strcpy(InputOrder_Buffer, InputOrder);

			//得到命令名称及其编号，保存在OrderNo
			int OrderNo;
			for(int i=0; i<_CMD_SUM; i++)
			{
				if(strcmp(command->GetName(), TargetOrderName[i])==0)
				{
					OrderNo = i;
					break;
				}
			}

			//根据命令名称编号及参数成员编号，进修修改操作
			switch(OrderNo)
			{				
			case 0:     //START
				{
					break;
				}
			case 1:     //END
				{break;}

				//////////////////////移动命令////////////////////////////
			case 2:     //MOVJ
			case 3:     //MOVL
			case 4:     //MOVC
				{
					{
						switch(MemberNo)
						{
						case 1:    //int1, P00
							{
								int num = _TOint;
								if(num>=_P_MIN && num<=_P_MAX)
								{
									command->SetiVariable(1, num);
								}								
								break;
							}
						case 2:    //double, V10
							{
								double num = _TOdouble;
								command->SetdVariable(num);
								break;
							}
						case 3:    //int2, Z2
							{
								int num = _TOint;
								if(num>=_PL_MIN && num<=_PL_MAX)
								{
									command->SetiVariable(2, num);
								}
								break;
							}
						default:break;
						}
					}

					//修改目标位姿
					command->Target = robot.GetCurrentPosition();
					//修改位姿标志
					if( !strcmp(command->GetName(), "MOVJ"))//MOVJ
					{
						command->Target.SetFlag(false);//关节坐标
					}
					else
					{
						command->Target.SetFlag(true);//直角坐标
					}

					break;
				}

				///////////////////////////信号处理////////////////////////
			case 5:     //SET
				{
					{
						switch(MemberNo)
						{
						case 1:    //int1, OUT01
							{
								int num = _TOint;
								if(num>=_OUT_MIN && num<=_OUT_MAX)
								{
									command->SetiVariable(1, num);
								}
								break;
							}
						case 2:    //bool ,ON/OFF
							{
								if(!_STRCMP("OFF"))//OFF
								{
									command->SetbVariable(false);
								}
								else if(!_STRCMP("ON"))//ON
								{
									command->SetbVariable(true);
								}
								break;
							}
						default:break;
						}
					}
					break;
				}
			case 6:     //WAIT
				{
					{
						switch(MemberNo)
						{
						case 1:    //int1, IN02
							{
								int num = _TOint;
								if(num>=_IN_MAX && num<=_IN_MAX)
								{
									command->SetiVariable(1, num);
								}
								break;
							}
						case 2:    //OFF/ON
							{
								if(!_STRCMP("OFF"))//OFF
								{
									command->SetbVariable(false);
								}
								else if(!_STRCMP("ON"))//ON
								{
									command->SetbVariable(true);
								}
								break;
							}
						case 3:    //str1, T
							{
								if(!_STRCMP(""))//为空,则没有T参数
								{
									command->SetstrVariable(1, "");//设置没有T
									command->SetdVariable(_TIME_MAX);//等待时间设定为最大
								}
								else if(!_STRCMP("T"))//有T
								{
									command->SetstrVariable(1, "T");//设置T
								}
								
								break;
							}
						case 4:    //double, 等待时间T999
							{
								if(!strcmp(command->GetstrVariable(1), "T"))//有T
								{
									double num = _TOdouble;
									if(num>=_TIME_MIN && num<=_TIME_MAX)
									{
										command->SetdVariable(num);
									}
								}
								break;
							}
						default:break;
						}
					}
					break;
				}
			case 7:     //DELAY
				{
					{
						switch(MemberNo)
						{
						case 1:    //double, T10
							{
								double num = _TOdouble;
								if(num>=_TIME_MIN && num<=_TIME_MAX)
								{
									command->SetdVariable(num);
								}
								break;
							}
						default:break;
						}
					}
					break;
				}

				/////////////////////////////流程控制/////////////////////////
			case 8:     //L
				{
					switch(MemberNo)
					{
					case 1:  //L<>
						{
							if(strlen(EditSourceOrder.c_str())<=8 && strlen(EditSourceOrder.c_str()) >=0)//长度适中							
							{
								char str[9];
								strcpy(str, EditSourceOrder.c_str());
								command->SetstrVariable(1, str);
							}
						}
					default:break;					
					}
					break;
				}
			case 9:     //GOTO
				{
					break;
				}
			case 10:     //#
				{
					switch(MemberNo)
					{
					case 1:  //L<>
						{
							if(EditSourceOrder.length()<=_CMD_NOTE_LENGTH-1 && EditSourceOrder.length()>=0)//长度适中							
							{
								char str[33];
								strcpy(str, EditSourceOrder.c_str());
								command->SetstrVariable(0, str);
							}
						}
					default:break;					
					}
					break;
				}
			case 11:     //CALL
				{
					{
						switch(MemberNo)
						{
						case 1:  //CALL<>
							{
								if(strlen(EditSourceOrder.c_str())<=8 && strlen(EditSourceOrder.c_str()) >=0)//长度适中							
								{
									char str[9];
									strcpy(str, EditSourceOrder.c_str());
									command->SetstrVariable(1, str);
								}
							}
						default:break;					
						}
					}
					break;
				}
			case 12:     //RET
				{
					break;
				}
				/////////////////////////////运算命令/////////////////////////
			case 13:     //DECR
			case 14:     //INCR
				{
					{
						switch(MemberNo)
						{
						case 1:    //int1, R01
							{
								int num = _TOint;
								if(num>=_R_MIN && num<=_R_MAX)
								{
									command->SetiVariable(1, num);
								}
								break;
							}
						default:break;
						}
					}
					break;
				}
			case 15:     //R
				{
					{
						switch(MemberNo)
						{
						case 1:    //int1, R=01
							{
								int num = _TOint;								
								command->SetiVariable(1, num);								
								break;
							}
						default:break;
						}
					}
					break;
				}

				/////////////////////////////平移命令/////////////////////////
			case 16:     //PR
				{
					{
						switch(MemberNo)
						{
						case 1:    //int1, PR01
							{							
								int num = _TOint;
								if(num>=_PR_MIN && num<=_PR_MAX)
								{
									command->SetiVariable(1, num);
								}								
								break;
							}
						case 2:    //int2, PR02
							{
								int num = _TOint;
								if(num>=_PR_MIN && num<=_PR_MAX)
								{
									command->SetiVariable(2, num);
								}								
								break;
							}
						case 3:    //str1 -/+
							{
								if(!_STRCMP("-"))//-
								{
									command->SetstrVariable(1, "-");
								}
								else if(!_STRCMP("+"))//+
								{
									command->SetstrVariable(1, "+");
								}
								break;
							}
						case 4:    //int3 PR03
							{
								if(strcmp(command->GetstrVariable(1), ""))//str1必须非空
								{
									int num = _TOint;
									if(num>=_PR_MIN && num<=_PR_MAX)
									{
										command->SetiVariable(3, num);
									}
								}
								break;
							}
						default:break;
						}
					}
					break;
				}
			case 17:     //SHIFTON
				{
					{
						switch(MemberNo)
						{
						case 1:    //int1, PR01
							{
								int num = _TOint;
								if(num>=_PR_MIN && num<=_PR_MAX)
								{
									command->SetiVariable(1, num);
								}
							}
						default:break;
						}
					}
					break;
				}
			case 18:     //SHIFTOFF
				{
					break;
				}
			case 19:     //MISHIFT
				{
					{
						switch(MemberNo)
						{
						case 1:    //int1, PR01
							{
								int num = _TOint;
								if(num>=_PR_MIN && num<=_PR_MAX)
								{
									command->SetiVariable(1, num);
								}
								break;
							}
						case 2:    //int2, P01
							{
								int num = _TOint;
								if(num>=_P_MIN && num<=_P_MAX)
								{
									command->SetiVariable(2, num);
								}
								break;
							}
						case 3:    //int3, P02
							{
								int num = _TOint;
								if(num>=_P_MIN && num<=_P_MAX)
								{
									command->SetiVariable(3, num);
								}
								break;
							}
						default:break;
						}
					}
					break;
				}

				/////////////////////////////命令结束/////////////////////////
			default :break;
			}
			return true;
		}//CmdEdit()

		/***************************************
		bool Explain()
		----------------------------------------
		程序解释,将嵌套调用的程序整合在一个程序
		----------------------------------------
		返回 :
		true : 程序解释成功
		false: 程序解释失败
		***************************************/
		bool Explain()
		{
			for(int i=0; i<this->LineNumber; ++i)//依次检查每条命令
			{
				if(!strcmp(this->commands[i].GetName(), "CALL"))//遇到CALL指令
				{
					if(!this->Nest(i, this->commands[i].GetstrVariable(1)))//嵌入子程序失败
					{
						this->Read(this->Name);//还原本程序
						return false;
					}
				}
			}
			return true;
		}//Explain()

		/***************************************
		bool Nest(int Line, char *name)
		----------------------------------------
		将子函数解释后,嵌入当前函数
		----------------------------------------
		Line : CALL指令出现的位置在commands[]数组中的下标
		name : 子函数的函数名
		----------------------------------------
		返回 :
		true : 嵌套成功
		false: 嵌套失败
		***************************************/
		bool Nest(int Line, char *name)
		{
			//对子函数进行解释
			
			Program Sub;//子程序
			if(!Sub.Read(name))//得到子程序
			{
				return false;
			}

			if(!Sub.Explain())
			{
				return false;
			}

			for(int i=1; i<Sub.GetLineNumber()-1; ++i)//去掉子程序的头和尾
			{
				if(!strcmp(Sub.commands[i].GetName(), "RET"))//遇到RET命令,则嵌套结束
				{
					break;
				}
				if(_MAXCDMLINE == this->LineNumber)//命令已达到最大限度
				{
					return false;
				}
				if(i>1)//从第二次开始移动主程序的命令
				{
					//主程序中CALL命令之后的所有命令,依次后移一位
					for(int j=this->LineNumber-1; j>Line; --j)//从尾命令,到CALL命令的下一条命令
					{
						this->commands[j+1] = this->commands[j];//移位
					}
					++this->LineNumber;//修改主程序的命令行数
					++Line;//修改嵌入位置
				}
				this->commands[Line] = Sub.commands[i];//嵌入一行命令				
			}
			return true;
		}//Nest()

		/***********************************************
		int Run(Robot &ROBOT, UserVariable &VARIABLE)
		------------------------------------------------
		全速执行一遍程序
		------------------------------------------------
	 	&ROBOT    : 此处为全局变量ROBOT
		&VARIABLE : 此处为全局变量VARIABLE
		------------------------------------------------
		返回 : 
		0 : 程序执行成功
		其他 : 程序执行失败,返回失败代码
		***********************************************/
		//int Run(Robot &ROBOT, UserVariable &VARIABLE, bool &SEND_FLAG, bool &RECV_FLAG)
		//{
		//	int result = 0;
		//	this->Explain();

		//	//int next = 0;
		//	int i=0;
		//	for (; i<this->LineNumber; )//从第一行命令到最后一行
		//	{
		//		int Last = i;//保存上次运行的行号
		//		result = this->CmdExcute(this, this->commands[i], ROBOT, VARIABLE, i, SEND_FLAG, RECV_FLAG);//执行单行程序
		//		if(result>0)//失败退出
		//		{
		//			break;
		//		}
		//	}

		//	this->Read();//还原本程序
		//	return result;
		//}


		/***********************************************
		int Step(Robot &ROBOT, UserVariable &VARIABLE, bool &SEND_FLAG=false, bool &RECV_FALG = false)
		单步执行程序
		------------------------------------------------
		  ROBOT  : 全局变量::ROBOT
		VARIABLE : 全局变量::VARIABLE
		SEND_FLAG: 全局变量::SEND_FLAG用于向PC发送程序
		RECV_FLAG: 全局变量::RECV_FLAG用于接收PC端的数据
		------------------------------------------------
		返回:
	 	 0  : 运行成功
		非0 : 运行失败
		***********************************************/
		//int Step(Robot &ROBOT, UserVariable &VARIABLE, bool& SEND_FLAG, bool& RECV_FLAG)
		//{			
		//	

		//	int result = 0;//运行结果
		//	//////////////////////////////////////////////////////
		//	static int CurrentLine = 0;//当前执行的命令行

		//	bool End = false;//是否执行到最后一行程序
		//	if(this->GetLineNumber() == CurrentLine)//执行到最后一行
		//	{
		//		End = true;
		//	}	

		//	result = CmdExcute(this, this->commands[CurrentLine], ROBOT, VARIABLE, CurrentLine, SEND_FLAG, RECV_FLAG);
		//	//////////////////////////////////////////////////////

		//	if(End)//执行到最后一行
		//	{
		//		CurrentLine = 0;//修改当前成续行
		//	}

		//	return result;
		//}//int Step()



		/***********************************************
		int CmdExcute(Program *pro, CommandLine cmd, int cmdNo, Robot &ROBOT, UserVariable &VARIABLE, int &go)
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
		//int CmdExcute(Program *pro, CommandLine cmd, Robot &ROBOT, UserVariable &VARIABLE, int &go, bool &SEND_FLAG, bool &RECV_FLAG)
		//{
		//	int result = 0;//执行结果,预先赋值为0

		//	//得到命令名称及其编号，保存在OrderNo
		//	int OrderNo;
		//	for(int i=0; i<_CMD_SUM; i++)
		//	{
		//		if(strcmp(cmd.GetName(), TargetOrderName[i])==0)
		//		{
		//			OrderNo = i;
		//			break;
		//		}
		//	}

		//	//根据命令名称编号,执行不同的动作
		//	switch(OrderNo)
		//	{
		//	case 0:     //START
		//		{//done
		//			++go;
		//			break;
		//		}
		//	case 1:     //END
		//		{//done
		//			go = pro->GetLineNumber();//保证程序正常退出
		//			break;
		//		}
		//		//////////////////运动命令
		//	case 2:     //MOVJ
		//		{//done
		//			//////////////////////////////////////////////////////////////////////////
		//			//在此添加移动机器人的代码
		//			//////////////////////////////////////////////////////////////////////////
		//			++go;
		//			break;
		//		}
		//	case 3:     //MOVL
		//		{//done
		//			//////////////////////////////////////////////////////////////////////////
		//			//在此处添加移动机器人的代码
		//			//////////////////////////////////////////////////////////////////////////
		//			++go;
		//			break;
		//		}
		//	case 4:     //MOVC
		//		{//done
		//			//////////////////////////////////////////////////////////////////////////
		//			//在此处添加移动机器人的代码
		//			//////////////////////////////////////////////////////////////////////////
		//			++go;
		//			break;
		//		}
		//		//////////////////信号处理
		//	case 5:     //SET
		//		{//done
		//			bool value = cmd.GetbVariable();//得到ON/OFF
		//			int No = cmd.GetiVariable(1);
		//			ROBOT._GOUT.io[No].SetValue(value);//修改IO

		//			++go;
		//			break;
		//		}
		//	case 6:     //WAIT
		//		{//done
		//			int No = cmd.GetiVariable(1);//io号码
		//			if (!strcmp(cmd.GetstrVariable(1), ""))//没有T,无限时等待
		//			{
		//				for (;;)
		//				{
		//					if ((cmd.GetbVariable() && ROBOT._SIN.io[No].GetValue())//同为true
		//						 || (!cmd.GetbVariable() && !ROBOT._SIN.io[No].GetValue()))//同为false
		//					{//已经满足等待条件
		//						++go;
		//						break;
		//					}
		//				}
		//			} 
		//			else if (!strcmp(cmd.GetstrVariable(1), "T"))//有T,限时等待
		//			{
		//				for (int i=0; i<_DELAY_1S*cmd.GetdVariable(); ++i)
		//				{
		//					if ((cmd.GetbVariable() && ROBOT._SIN.io[No].GetValue())//同为true
		//						 || (!cmd.GetbVariable() && !ROBOT._SIN.io[No].GetValue()))//同为false
		//					{//已经满足IN通道等待条件
		//						break;
		//					}
		//				}

		//				//为满足IN通道等待条件,但等待时间已到
		//				++go;
		//			} 
		//			else//出错
		//			{
		//				return 60;//60号错误,WAIT命令的str1参数有误
		//			}					

		//			//++go;
		//			break;
		//		}
		//	case 7:     //DELAY
		//		{
		//			double t = cmd.GetdVariable();//得到延时的时长
		//			this->Time(t);//延时

		//			++go;
		//			break;
		//		}
		//		/////////////////流程控制
		//	case 8:     //L
		//		{//done
		//			break;
		//		}
		//	case 9:     //GOTO
		//		{//done
		//			char Target[_CMD_STR_LENGTH];
		//			strcpy(Target, cmd.GetstrVariable(1));//得到跳转目的
		//			for (int i=0; i<pro->GetLineNumber(); ++i)
		//			{
		//				if (!strcmp(Target, pro->commands[i].GetName()))//判断是否为跳转地址
		//				{
		//					go = i;
		//					break;//执行成功
		//				}
		//			}
		//			return 90;//错误信息90 : 跳转目的不存在
		//			break;
		//		}
		//	case 10:     //#
		//		{//done
		//			break;
		//		}
		//	case 11:     //CALL
		//		{//done
		//			break;
		//		}
		//	case 12:     //RET
		//		{//done
		//			break;
		//		}
		//		/////////////////运算命令
		//	case 13:     //DECR
		//		{//done
		//			int No = cmd.GetiVariable(1);//得到欲修改的变量号码
		//			VARIABLE._R[No] -= 1;        //自减1
		//			++go;
		//			break;
		//		}
		//	case 14:     //INCR
		//		{//done
		//			int No = cmd.GetiVariable(1);//得到欲修改的变量号码
		//			VARIABLE._R[No] += 1;        //自加1
		//			++go;
		//			break;
		//		}
		//	case 15:     //R
		//		{
		//			VARIABLE.R = cmd.GetiVariable(1);//赋值给全局R变量

		//			++go;
		//			break;
		//		}
		//		//////////////////平移命令
		//	case 16:     //PR
		//		{
		//			if (!strcmp(cmd.GetstrVariable(1), ""))//str2为空,没有+/-
		//			{
		//				VARIABLE._PR[cmd.GetiVariable(1)] = VARIABLE._PR[cmd.GetiVariable(2)];
		//			} 
		//			else if (!strcmp(cmd.GetstrVariable(1), "+"))//str2为+
		//			{
		//				POStype pr = VARIABLE._PR[cmd.GetiVariable(2)];
		//				pr += VARIABLE._PR[cmd.GetiVariable(3)];
		//				VARIABLE._PR[cmd.GetiVariable(1)] = pr;
		//				//VARIABLE._PR[cmd.GetiVariable(1)] += VARIABLE._PR[cmd.GetiVariable(2)];
		//			}
		//			else if (!strcmp(cmd.GetstrVariable(1), "-"))//str2为-
		//			{
		//				VARIABLE._PR[cmd.GetiVariable(1)] -= VARIABLE._PR[cmd.GetiVariable(2)];
		//			}
		//			else//出错
		//			{
		//				return 160;//160号错误,PR变量str2有误
		//			}					

		//			++go;
		//			break;
		//		}
		//	case 17:     //SHIFON
		//		{
		//			//////////////////////////////////////////////////////////////////////////
		//			//在此添加平移开始代码
		//			//////////////////////////////////////////////////////////////////////////
		//			++go;
		//			break;
		//		}
		//	case 18:     //SHIFOFF
		//		{
		//			//////////////////////////////////////////////////////////////////////////
		//			//在此添加平移结束代码
		//			//////////////////////////////////////////////////////////////////////////
		//			++go;
		//			break;
		//		}
		//	case 19:     //MISHIFT
		//		{
		//			int PR_No = cmd.GetiVariable(1);//PR变量号
		//			int P_No1 = cmd.GetiVariable(2);//P变量1
		//			int P_No2 = cmd.GetiVariable(3);//P变量2
		//			 
		//			VARIABLE._P[P_No2] -= VARIABLE._P[P_No1];
		//			VARIABLE._PR[PR_No] = VARIABLE._P[P_No2];

		//			++go;
		//			break;
		//		}
		//		
		//	default:break;
		//	}

		//	return result;
		//}//CmdExcute()


		/************************************
		void Time(double time)
		-------------------------------------
		延时程序
		-------------------------------------
		time : 延时的时长
		************************************/
		void Time(double time)
		{

		}//Time()
};//struct Program
