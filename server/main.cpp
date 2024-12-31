// #include "socket.h"
#include <iostream>
// #include "des.h"
// #include "defineWords.h"
#include "utils.h"
#include "CommandParser.h"
using namespace std;

char NOW_LEVEL_PERMISSION = DEFAULT_LOW_LEVEL_PERMISSION;


int main() {
    Socket server;
    initServer(server);
    char recvBuff[1024];
    int recvSize;

    sendMessage(server, "低级权限验证开始，请输入密码\n");


    // 接收数据
    while ((recvSize = server.Receive(recvBuff, sizeof(recvBuff) - 1)) > 0) {
        cout << "recvSize: " << recvSize << endl;
        cout << recvBuff[0] << endl;
        recvBuff[recvSize] = '\0'; // 确保字符串正确终止
        cout << "Received: " << recvBuff << std::endl;


        //低级权限
        if (checkLowLevelPermission(recvBuff)) {
            NOW_LEVEL_PERMISSION = LOW_LEVEL_PERMISSION;
            sendMessage(server, "低级权限验证成功，请输入指令\n");
            sendMessage(server, getSupportCommandMsg().c_str());
        } else {
            sendMessage(server, "低级权限验证失败，请重新输入密码\n");
            continue;
        }

        if (NOW_LEVEL_PERMISSION != '2') {
            break;
        }

    }

    CommandParser commandParser(server, NOW_LEVEL_PERMISSION);

    while ((recvSize = server.Receive(recvBuff, sizeof(recvBuff) - 1)) > 0) {
        cout << "recvSize: " << recvSize << endl;
        cout << recvBuff[0] << endl;
        recvBuff[recvSize] = '\0'; // 确保字符串正确终止
        cout << "Received: " << recvBuff << std::endl;

        auto cmd = commandParser.parse(recvBuff);
        bool result = commandParser.execute(cmd);
        if (result) {
            sendMessage(server, "命令执行成功\n");
        } else {
            sendMessage(server, "命令执行失败\n");
            sendMessage(server, getSupportCommandMsg().c_str());
        }
    }

    // 关闭连接
    server.CloseConnection();

    system("pause");
    return 0;
}

