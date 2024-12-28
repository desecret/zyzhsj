#include "socket.h"
#include <iostream>
#include "des.h"
#include "defineWords.h"
#include "utils.h"
using namespace std;

char NOW_LEVEL_PERMISSION = '-1';

/*
    处理指令
*/
void handleCommand(char permission, const char* command) {
    if (permission == LOW_LEVEL_PERMISSION) {
        cout << "低级权限" << endl;
        cout << "handleCommand: " << command << endl;
    } else if (permission == HIGH_LEVEL_PERMISSION) {
        cout << "高级权限" << endl;
        cout << "handleCommand: " << command << endl;
    }
}

int main() {
    Socket server = initSocket();
    char recvBuff[1024];
    int recvSize;
    // 接收数据
    while ((recvSize = server.Receive(recvBuff, sizeof(recvBuff) - 1)) > 0) {
        cout << "recvSize: " << recvSize << endl;
        cout << recvBuff[0] << endl;
        recvBuff[recvSize] = '\0'; // 确保字符串正确终止
        cout << "Received: " << recvBuff << std::endl;

        switch (recvBuff[0])
        {
            case LOW_LEVEL_PERMISSION:
                //低级权限
                if (checkLowLevelPermission(recvBuff)) {
                    NOW_LEVEL_PERMISSION = LOW_LEVEL_PERMISSION;
                    // TODO：不提示，设置验证高级权限的指令
                    
                    sendMessage(server, "低级权限验证成功，是否继续验证高级权限？(0:不继续，1:继续)");
                    server.Receive(recvBuff, sizeof(recvBuff));

                    if (ifContinueCheckHighPermission(recvBuff)) {
                        sendMessage(server, "高级权限验证开始，请输入密码：");
                        continue;
                    } else {
                        sendMessage(server, "低级权限验证成功，不继续验证高级权限，请输入指令");
                        server.Receive(recvBuff, sizeof(recvBuff));
                        handleCommand(NOW_LEVEL_PERMISSION, recvBuff);
                    }
                } else {
                    sendMessage(server, "低级权限验证失败，请重新输入密码");
                    continue;
                }
                break;
            case HIGH_LEVEL_PERMISSION:
                //高级权限
                if (checkHighLevelPermission(recvBuff)) {
                    sendMessage(server, "高级权限验证成功，请输入指令");
                    NOW_LEVEL_PERMISSION = HIGH_LEVEL_PERMISSION;
                    server.Receive(recvBuff, sizeof(recvBuff));
                    handleCommand(NOW_LEVEL_PERMISSION, recvBuff);
                } else {
                    sendMessage(server, "高级权限验证失败，请重新输入密码");
                    continue;
                }
                break;

            default:
                break;
        }

    }

    // 关闭连接
    server.CloseConnection();

    return 0;
}

