#pragma once

#include <winsock2.h>
#include <iostream>

#define PORT 8888

class Socket {
private:
    WSADATA wsaData;
    SOCKET serverSocket;
    SOCKET newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen;

public:
    Socket() {
        serverSocket = 0;
        newSocket = 0;
    }

    ~Socket() {
        CloseConnection();
    }

    // 初始化Winsock
    bool Initialize() {
        if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
            std::cerr << "Winsock Initialization failed." << std::endl;
            return false;
        }
        return true;
    }

    // 创建socket
    bool CreateSocket() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Could not create socket : " << WSAGetLastError() << std::endl;
            return false;
        }
        return true;
    }

    // 绑定socket到端口
    bool BindSocket(unsigned short port) {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed with error code : " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }
        return true;
    }

    // 监听socket
    bool Listen() {
        if (listen(serverSocket, 3) == SOCKET_ERROR) {
            std::cerr << "Listen failed with error code : " << WSAGetLastError() << std::endl;
            return false;
        }
        return true;
    }

    // 接受客户端连接
    SOCKET Accept() {
        addrLen = sizeof(struct sockaddr_in);
        newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (newSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error code : " << WSAGetLastError() << std::endl;
            return INVALID_SOCKET;
        }
        return newSocket;
    }

    // 接收数据
    int Receive(char *recvBuff, int len) {
        return recv(newSocket, recvBuff, len, 0);
    }

    // 发送数据
    int Send(const char *sendBuff, int len) {
        return send(newSocket, sendBuff, len, 0);
    }

    // 关闭连接
    void CloseConnection() {
        closesocket(newSocket);
        closesocket(serverSocket);
        WSACleanup();
    }

    
};



/*
    server: 服务器对象
    sendBuff: 发送的数据
    len: 发送的数据长度
*/
// int sendMessage(Socket server, char *sendBuff) {
//     return server.Send(sendBuff, sizeof(sendBuff));
// }

void initServer(Socket& server) {
    // 创建Socket对象
    // Socket server;

    // 初始化Winsock
    if (!server.Initialize()) {
        throw std::runtime_error("Socket initialization failed");
    }

    // 创建socket
    if (!server.CreateSocket()) {
        throw std::runtime_error("Socket creation failed");
    }

    // 绑定socket到端口
    unsigned short port = PORT; // 你可以选择任何未被占用的端口
    if (!server.BindSocket(port)) {
        throw std::runtime_error("Socket binding failed");
    }

    // 监听socket
    if (!server.Listen()) {
        throw std::runtime_error("Socket listening failed");
    }

    std::cout << "Server is listening on port " << port << std::endl;

    // 接受客户端连接
    SOCKET newSocket = server.Accept();
    if (newSocket == INVALID_SOCKET) {
        throw std::runtime_error("Socket accepting failed");
    }

    // return server;
}


bool init_Socket()
{
	//初始化代码
	WORD wVersion = MAKEWORD(2, 2);
	//MAKEWORD：将两个byte型合成一个word型，一个在高八位，一个在低八位
	//MAKEWORD(1,1)只能一次接收一次，不能马上发送，只支持TCP/IP协议，不支持异步
	//MAKEWORD(2,2)可以同时接收和发送，支持多协议，支持异步
	WSADATA wsadata;
	if (0 != WSAStartup(wVersion, &wsadata))	//WSA:widows socket ansyc	windows异步套接字
	{
		perror("WSAStartup");
		return false;
	}
	//return true;
}


SOCKET create_clientSocket(const char *ip)
{
	//1.创建一个空的socket
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == fd)
	{
		perror("socket"); 
		return INVALID_SOCKET;
	}

//2.给socket绑定服务端ip地址和端口号
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);	//htons()：把本地字节序转成网络字节序
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.36.1");	//绑定服务器ip，此处与本机另一进程通信，故ip为本机地址127.0.0.1
	if (INVALID_SOCKET == connect(fd, (struct sockaddr*)&addr, sizeof(addr)))
	{
		perror("connect");
		return INVALID_SOCKET;
	}

	return fd;
}
