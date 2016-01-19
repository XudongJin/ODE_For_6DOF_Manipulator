//Variable.h 变量头文件
//2010-7-11 11:13:08
#include "Position.h"
#pragma once

//变量总数
#define _VARIABLE_NO 128

template <typename T>  //变量类模板
class Variable
{
	private:
		T value;       //取值
		char Note[33]; //注释

	public:
		Variable()//构造函数
		{
			value = 0;
			strcpy(Note, "");
		}
		char * GetNote()//提取Note
		{
			return Note;
		}
		void SetNote(char * newnote)//修改Note
		{
			strcpy(Note, newnote);
		}
		T GetValue()//提取value
		{
			return value;
		}
		void SetValue(T newvalue)//修改value
		{
			value = newvalue;
		}
		void Clear()//清空变量
		{
			value = 0;
			strcpy(Note, "");
		}
		//重载运算符
		Variable operator = (T right)//=
		{
			value = right;
			return *this;
		}
		//+=
		Variable operator += (Variable right)
		{
			value += right.value;
			return *this;
		}
		Variable operator += (T right)
		{
			value += right;
			return *this;
		}
		//-=
		Variable operator -= (Variable right)
		{
			value -= right.value;
			return *this;
		}
		Variable operator -= (T right)
		{
			value -= right;
			return *this;
		}
		//*=
		Variable operator *= (Variable right)
		{
			value *= right.value;
			return *this;
		}
		Variable operator *= (T right)
		{
			value *= right;
			return *this;
		}
		// /=
		Variable operator /= (Variable right)
		{
			value /= right.value;
			return *this;
		}
		Variable operator /= (T right)
		{
			value /= right;
			return *this;
		}
		// &
		Variable operator & (Variable &right)
		{
			value = (value & right.value);
			return *this;
		}
		Variable operator & (T right)
		{
			value = (value & (right%256));
			return *this;
		}
		// |
		Variable operator | (Variable &right)
		{
			value = (value | right.value);
			return *this;
		}
		Variable operator | (T right)
		{
			value = (value | (right%256));
			return *this;
		}
		// ~
		Variable operator ~ ()
		{
			value = ~value;
	        return *this;
		}
		// ^ 
		Variable operator ^ (Variable &right)
		{
			value = (value ^ right.value);
	        return *this;
		}
		Variable operator ^ (T right)
		{
			value = (value ^ (right%256));
        	return *this;
		}
};

//字符串型
typedef struct STRINGtype
{
	private:
		char value[17]; //取值
		char Note[33];  //注释
		
	public:
		STRINGtype()          //构造函数
		{
			strcpy(value, "");
			strcpy(Note, "");
		}
		char * GetValue()     //提取value
		{
			return value;
		}
		void SetValue(char * newvalue) //修改value
		{
			strcpy(value, newvalue);
		}
		char * GetNote()      //提取Note
		{
			return Note;
		}
		void SetNote(char * newnote)  //修改Note
		{
			strcpy(Note, newnote);
		}
		void Clear()          //清空
		{
			strcpy(value, "");
			strcpy(Note, "");
		}
}_STRINGtype;


//用户变量结构体
typedef struct UserVariable
{
	public:
		//根据安川机器人说明书设定的变量
		Variable<unsigned char> _BYTE[100];    //字节型
		Variable<int>           _INT[100];     //整型
		Variable<long int>      _LONGINT[100]; //长整型
		Variable<float>         _FLOAT[100];   //实数型
		STRINGtype              _STRING[100];  //字符串
		POStype                 _POS[128];     //P位置变量
		POStype                 _BPOS[128];    //BP位置变量
		POStype                 _EXPOS[128];   //EX位置变量

		//根据广数的说明书添加的变量
		Variable<int>           _R[_R_VARIABLE];          //R型变量
		POStype                 _P[_P_VARIABLE];          //p型变量
		POStype                 _PR[_PR_VARIABLE];        //PR型变量
		int                     R;                        //控制循环的全局R变量
		
	public:
		unsigned char GetBYTEValue(int No)
		{
			return _BYTE[No].GetValue();
		}
		void SetBYTEValue(int No, int value)
		{
			value %= 256;
			_BYTE[No].SetValue(value);
		}
		char * GetBYTENote(int No)
		{
			return _BYTE[No].GetNote();
		}
		void SetBYTENote(int No, char * note)
		{
			_BYTE[No].SetNote(note);
		}
}_UserVariable;
