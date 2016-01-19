//: Macaro.h
//宏定义头文件，将所有宏定义的数据全部放在该文件中
//2010-9-28 10:55:48
#include <string>
using namespace std;
#pragma once

//2010-9-28 11:11:49
const int _CMD_SUM = 20;//总的命令条数,即TargetOrderName的行数
//所有命令
const char TargetOrderName[][9]=
{
	{"START"},    //0
	{"END"},      //1
	
	//移动命令
	{"MOVJ"},     //2
	{"MOVL"},     //3
	{"MOVC"},     //4

	//信号处理
	{"SET"},      //5
	{"WAIT"},     //6
	{"DELAY"},    //7

	//流程控制
	{"L"},        //8
	{"GOTO"},     //9
	{"#"},        //10
	{"CALL"},     //11
	{"RET"},      //12

	//运算命令
	{"DECR"},     //13
	{"INCR"},     //14
	{"R"},        //15

	//平移命令
	{"PR"},       //16
	{"SHIFTON"},  //17
	{"SHIFTOFF"}, //18
	{"MISHIFT"},  //19
	
	///////////////////////////在此处添加新的命令
};

const  int      _VALID_CMDLINE     = 20;  //每个程序中,用户最多编辑的命令数
const  int      _NEST_DEEPTH       = 2;   //允许嵌套的程序个数
const  int      _MAXCDMLINE        = _NEST_DEEPTH * _VALID_CMDLINE; //程序最多容纳的命令数

const  int      _PROGRAM_MAXNUMBER = 10;   //允许用户创建的最多程序个数
const  int      _PRO_NAME_LENGTH   = 9;    //程序名称数组的长度
const  int      _PRO_NOTE_LENGTH   = 33;   //程序注释数组的长度
const  int      _CMD_NAME_LENGTH   = 9;    //命令名数组的长度
const  int      _CMD_NOTE_LENGTH   = 17;   //注释命令数组的长度
const  int      _CMD_STR_LENGTH    = 9;    //Command结构体str变量的长度

const  int      _P_MAX             = 127;  //P变量的最大编号
const  int      _P_MIN             = 0;    //P变量的最小编号
const  int      _PR_MAX            = 127;  //PR变量的最大编号
const  int      _PR_MIN            = 0;    //PR变量的最小编号
const  int      _R_MAX             = 127;  //R变量的最大编号
const  int      _R_MIN             = 0;    //R变量的最小编号
const  int      _PL_MAX            = 7;    //位置等级的最大编号
const  int      _PL_MIN            = 0;    //位置等级的最小编号
const  int      _OUT_MAX           = 7;    //OUT端口的最大编号
const  int      _OUT_MIN           = 0;    //OUT端口的最小编号
const  int      _IN_MAX            = 7;    //IN端口的最大编号
const  int      _IN_MIN            = 0;    //IN端口的最小编号
const  double   _TIME_MIN          = 1;    //等待时间的最小值   
const  double   _TIME_MAX          = 999;  //等待时间的最大值

const  int      _R_VARIABLE        = 100;  //R型变量的个数
const  int      _P_VARIABLE        = 100;  //P型变量的个数
const  int      _PR_VARIABLE       = 100;  //PR型变量的个数

const  int      _DELAY_1S          = 65536;//假设延时1s需要的空循环次数

const  double   _MANUAL_HIGH       = 40.0;//高速50cm/s
const  double   _MANUAL_MIDDLE     = 30.0;//高速50cm/s
const  double   _MANUAL_LOW        = 20.0;//高速50cm/s
const  double   _MANUAL_SLOW       = 10.0;//高速50cm/s

#define         _NET_PORT          6000    //网络端口

//#define    _VARIABLE_NO    128   //变量总数

//程序数据的存盘路径
#define    _PROGRAM_LOCATION  "C:\\Data\\%s.pro"
//程序列表的存盘路径
#define    _NAMES_LOCATION    "C:\\Data\\Names.nam"
//主程序程序名的存盘路径
#define    _PRIMARY_LOCATION  "C:\\Data\\Primary.nam"
//ROBOT变量的存盘路径
#define    _ROBOT_LOCATION    "C:\\Data\\ROBOT.rob"
//VARIABLE变量的存盘路径
#define    _VARIABLE_LOCATION "C:\\Data\\VARIABLE.var"

#define    _FRAME_NO       30    //用户坐标系、工具坐标系数目
#define    _IO_SUM         8     //每组IO通道的IO数目

#define    _AXIS_SUM       10    //轴干涉区数目
#define    _CUBE_SUM       10    //立方替干涉区数目
#define    _FRAME_NO       30    //用户坐标系、工具坐标系数目
#define    _DELAY_SUM      8     //继电器数目
#define    _GIN_SUM        8     //通用输入数目
#define    _GOUT_SUM       8     //通用输出数目
#define    _SIN_SUM        8     //专用输入数目
#define    _SOUT_SUM       8     //专用输出数目
#define    _IO_SUM         8     //每组IO通道的IO数目
#define    _ALARM_SUM      30    //每组报警的报警数



