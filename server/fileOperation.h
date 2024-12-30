#include<iostream>
#include <winsock2.h>
#include "socket.h"
#include "utils.h"

using namespace std;

long bufSize = 10*1024;	//缓冲区大小
char* buffer;	//缓冲区保存文件数据
//long recvSize = 10000000;
//char* recvBuf;

bool sendFile(Socket& s, const char* fileName)
{
	FILE* read = fopen(fileName, "rb");
	if (!read)
	{
		perror("file open failed:\n");//输出描述性错误信息
		return false;
	}

//获取文件大小
	fseek(read, 0, SEEK_END);	//将文件位置指针移动到最后
	bufSize = ftell(read);	//ftell(FILE *stream)：返回给定流stream的当前文件位置，获取当前位置相对文件首的位移，位移值等于文件所含字节数
	fseek(read, 0, SEEK_SET);	//将文件位置指针移动到开头
	cout<<"filesize:"<<bufSize<< endl;

//把文件读到内存中来
	buffer= new char[bufSize];
	cout << sizeof(buffer) << endl;
	if (!buffer)
	{
		return false;
	}

	int nCount;
	int ret = 0;
	while ((nCount = fread(buffer, 1, bufSize, read)) > 0)	//循环读取文件进行传送
	{
		ret += s.Send(buffer, nCount);
		if (ret == SOCKET_ERROR)
		{
			perror("sendFile");
			return false;
		}
	}
	s.CloseConnection();
	s.Receive(buffer, bufSize);
	fclose(read);

	cout << "send file success!"<<" Byte:"<<ret << endl;
	// system("pause");
	return true;
}

bool recvFile(Socket& s, const char* fileName)
{

	if (buffer == NULL)
	{
		buffer = new char[bufSize];
		if (!buffer)
			return false;
	}
//	创建空文件
	FILE* write = fopen(fileName, "wb");
	if (!write)
	{
		perror("file write failed:\n");
		return false;
	}

	int ret = 0;
	int nCount;
	while ((nCount = s.Receive(buffer, bufSize)) > 0)	//循环接收文件并保存
	{
		ret += fwrite(buffer,nCount, 1, write);
	}
	if (ret == 0)
	{
		cout << "server offline" << endl;
	}
	else if(ret < 0)
	{
		perror("recv");
		return false;
	}
	cout << "receive file success!" << endl;

	fclose(write);
	cout << "save file success! Filename:"<<fileName << endl;
	// system("pause");
	return true;
}
