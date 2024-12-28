#include <string>
#include <winsock2.h>
#include <iostream>
#include "socket.h"
#include "defineWords.h"
#include "des.h"
using namespace std;

void sendMessage(Socket& server, const char* message) {
    server.Send(message, strlen(message));
}

bool checkPassword(const char* password, const char* correctPassword) {
    return strcmp(password, correctPassword) == 0;
}

bool checkLowLevelPermission(const char* password) {
    password++;
    cout << "password: " << password << endl;
    return checkPassword(password, LOW_LEVEL_PASSWORD);
}

bool checkHighLevelPermission(const string& password) {
    // 去掉第一个字符
    string str = password.substr(1);
    string result = getDesEncryptResult(str);
    cout << "result: " << result << endl;
    return HIGH_LEVEL_PASSWORD == result;
}
/*
    低级权限验证成功后，是否继续验证高级权限，收到0则不继续验证，收到1则继续验证
    receiveBuff: 接收到的数据
*/
bool ifContinueCheckHighPermission(const char* receiveBuff) {
    return receiveBuff[0] == '1';
}

Socket initSocket() {
    // 创建Socket对象
    Socket server;

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

    return server;
}