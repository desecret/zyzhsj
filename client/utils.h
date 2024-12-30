#pragma once

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
    // string str = password.substr(1);
    string result = getDesEncryptResult(password);
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

bool needLowLevelPermission(const char nowLevelPermission) {
    return nowLevelPermission != DEFAULT_LOW_LEVEL_PERMISSION;
}

bool needHighLevelPermission(const char nowLevelPermission) {
    return nowLevelPermission == HIGH_LEVEL_PERMISSION;
}
