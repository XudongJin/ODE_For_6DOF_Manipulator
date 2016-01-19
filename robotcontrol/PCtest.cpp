//PCtest.cpp

#include <iostream>
#include "Execute.h"
#include "PC.h"

using namespace std;

SOCKET PC::PC_socket;
DataPacket PC::m_Data;

bool SEND_FLAG;
bool RECV_FLAG;
extern DWORD WINAPI RECV4BOX(void * Data);
extern DWORD WINAPI SEND2BOX(void * Data);

//测试
void Check();

void main()
{
	cout<<sizeof(DataPacket)<<endl;
	PC myPC;

	myPC.CreateSocket();
	if(myPC.Connect())
	{
		cout<<"Connect Successed."<<endl;
	}

	//新开用于发送的线程
	HANDLE hThreadSend = ::CreateThread(NULL, 0, SEND2BOX, NULL, 0, NULL);
	CloseHandle(hThreadSend);
	//新开用于接收的线程
	HANDLE hThreadRecv = ::CreateThread(NULL, 0, RECV4BOX, NULL, 0, NULL);	
	CloseHandle(hThreadRecv);

	::Run(&myPC.m_Data.m_program, myPC.m_Data.ROBOT, myPC.m_Data.VARIABLE);

	while(1);
}

//从BOX接收数据
DWORD WINAPI RECV4BOX(void * Data)
{
	while(true)
	{
		PC pc;
		pc.Receive();		
		
		Check();//测试	
	}
	return 0;
}


//向BOX发送数据
DWORD WINAPI SEND2BOX(void * Data)
{
	while(true)
	{
		if(true == SEND_FLAG)
		{
			PC pc;
			pc.Send();

			SEND_FLAG = false;
		}		
	}

	return 0;
}

void Check()
{
	PC pc;
	char mission = pc.m_Data.GetMission();
	cout<<"Mission: "<<mission<<endl;

	switch(mission)
	{
	case 'A'://mission A
		{
			cout<<"Key= "<<pc.m_Data.A_Mission.GetKey()<<endl;
			break;
		}
	case 'B'://到达一个已知点
		{
			int point = pc.m_Data.B_Mission.GetTarget();
			if(0 == point)
			{
				cout<<"绝对原点"<<endl;
			}
			else if(1 == point )
			{
				cout<<"第二原点"<<endl;
			}
			else if(2 == point)
			{
				cout<<"作业原点"<<endl;
			}
			else
			{
				cout<<"原点错误"<<endl;
			}
			break;
		}
	case 'D'://前进
		{
			char key = pc.m_Data.D_Mission.GetKey();
			if('A' == key)
			{
				cout<<"前进"<<endl;
			}
			else if('B' == key)
			{
				cout<<"连锁、前进"<<endl;
			}
			else if('C' == key)
			{
				cout<<"转换、前进"<<endl;
			}

			break;
		}

	case 'C':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
		{
			break;
		}
	default:
		{
			cout<<"Mission error"<<endl;
			break;
		}
	}
}