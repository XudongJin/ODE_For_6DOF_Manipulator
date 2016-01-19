//PC.h
//PC端用于接收数据的结构体

#include <Winsock2.h>
#include <memory.h>
#include "DataPacket.h"

#define _BOX_ADDR "127.0.0.1"//BOX的IP地址
#define NET_PORT 6000//网络端口号

struct PC
{
private:
	SOCKET PC_socket;     //用于通信的SOCKET

public:
	DataPacket m_Data;           //从BOX接收的数据
	C_information send_Data;     //向BOX发送的数据

public:
	/**************************************
	bool Comunicate()
	与BOX进行通信,不使用静态SOCKET
	**************************************/
	bool Comunicate()
	{
		//SOCKADDR_IN addrBOX;//BOX的地址
		//addrBOX.sin_addr.S_un.S_addr = inet_addr(_BOX_ADDR);
		//addrBOX.sin_family = AF_INET;
		//addrBOX.sin_port = htons(6000);

		//int result = connect(this->PC_socket, (SOCKADDR*)&addrBOX, sizeof(SOCKADDR));

		//char* Recv_buffer = new char[sizeof(this->m_Data)];//开辟一个数据缓冲区
		//int size = recv(this->PC_socket, Recv_buffer, sizeof(this->m_Data), 0);//接收数据

		//if(sizeof(this->m_Data) != size)//接收失败
		//{
		//	delete Recv_buffer;
		//	return false;
		//}
		//else
		//{
		//	memcpy(&this->m_Data, Recv_buffer, sizeof(this->m_Data));//把接收到的数据拷贝至变量
		//	delete Recv_buffer;
		//	return true;
		//}
		return false;

	}//Comunicate()

	/**************************************
	PC()
	构造函数, 对套接字进行版本协商
	对"欲发送的数据"进行初始化
	**************************************/
	PC()
	{
		//套接字的版本协商
		WORD version = MAKEWORD(1,1);//1.1版本
		WSADATA wsaData;

		WSAStartup(version, &wsaData);//启动套接字
		//欲发送数据的初始化
	}//PC()

	/**************************************
	~PC()
	析构函数,对数据进行清理
	**************************************/
	~PC()
	{
	//	WSACleanup();//关闭套接字
	}//~PC()

	/**************************************
	bool CreateSocket()
	创建套接字,并赋值给PC_socket
	---------------------------------------
	true  : 创建成功
	false : 创建失败
	**************************************/
	bool CreateSocket()
	{
		this->PC_socket = socket(AF_INET, SOCK_STREAM, 0);

		if(INVALID_SOCKET == this->PC_socket)
		{
			return false;
		}
		else
		{
			return true;
		}
	}//CreateSocket()

	/**************************************
	bool Connect()
	与BOX进行连接
	---------------------------------------
	true  : 连接成功
	false : 连接失败
	**************************************/
	bool Connect()
	{
		SOCKADDR_IN addrBOX;//BOX的地址
		addrBOX.sin_addr.S_un.S_addr = inet_addr(_BOX_ADDR);
		addrBOX.sin_family = AF_INET;
		addrBOX.sin_port = htons(NET_PORT);

		int result = connect(this->PC_socket, (SOCKADDR*)&addrBOX, sizeof(SOCKADDR));

		if(SOCKET_ERROR == result)
		{
			return false;
		}
		else
		{
			return true;
		}
	}//Accept()

	/**************************************
	bool Send()
	向BOX发送数据
	---------------------------------------
	true  : 发送成功
	false : 发送失败
	**************************************/
	bool Send()
	{
		int const len = sizeof(this->m_Data);
		char* Send_buffer = new char[len];//开辟数据缓冲区
		memcpy(Send_buffer, &this->m_Data, len);//将要发送的数据,拷贝至缓冲区

		int size = send(this->PC_socket, Send_buffer, len, 0);//发送
		delete Send_buffer;//关闭缓冲区
		
		if(len == size)
		{
			return true;
		}
		else
		{
			return false;
		}
	}//Send()

	//bool Send(bool Flag)
	//向BOX发送数据,不发送Mission数据,只发送位姿和用户变量VARIABLE
	//Flag : 只是用来与原有Send()函数区分,暂无任何意义
	//返回 :
	//true  : 发送成功
	//false : 发送失败
	bool Send(bool Flag)
	{
		//int const size_Variable = sizeof(this->m_Data.VARIABLE);
		int const size_C = sizeof(this->m_Data.C_Mission);
		//int const len = size_Variable + size_C; //数据缓冲区大小
		int const len = size_C;

		char *Send_buffer = new char[len];//数据缓冲区
		//memcpy(Send_buffer, &this->m_Data.VARIABLE, size_Variable);//拷贝用户变量VARIABLE
		//memcpy(Send_buffer+size_Variable, &this->m_Data.C_Mission, size_C);//拷贝C_Mission
		memcpy(Send_buffer, &this->send_Data, len);

		int Send_size = send(this->PC_socket, Send_buffer, len, 0);//发送
		delete Send_buffer;//关闭数据缓冲区

		if(len == Send_size)
		{
			return true;
		}
		else
		{
			return false;
		}

	}//Send()

	/**************************************
	bool Receive()
	从BOX接收数据
	---------------------------------------
	true  : 接收成功
	false : 接收失败
	**************************************/
	bool Receive()
	{
		char* Recv_buffer = new char[sizeof(this->m_Data)];//开辟数据缓冲区
		int size = recv(this->PC_socket, Recv_buffer, sizeof(this->m_Data), 0);//接收数据
		memcpy(&this->m_Data, Recv_buffer, sizeof(this->m_Data));//保存数据
		delete Recv_buffer;//关闭数据缓冲区

		if(sizeof(this->m_Data) == size)
		{
			return true;
		}
		else
		{
			return false;
		}
	}//Receive()

};//PC