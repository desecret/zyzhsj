#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "socket.h"
#include "utils.h"

using namespace std;

#define HIGH_LEVEL_PASSWORD "85E813540F0AB405"      // 明文：0123456789ABCDEF，秘钥：133457799BBCDFF1


class CommandParser {
private:
    Socket& socket;
    char& nowLevelPermission;

    char recvBuff[1024];
    // 检查权限
    bool checkHighPermission(const string& password) {
        if (!needLowLevelPermission(nowLevelPermission)) {
            sendMessage(socket, "权限不足");
            return false;

        }

        if (checkHighLevelPermission(password)) {
            nowLevelPermission = HIGH_LEVEL_PERMISSION;
            sendMessage(socket, "高级权限验证成功，请输入指令");
            return true;
        } else {
            sendMessage(socket, "高级权限验证失败");
            return false;
        }
    }

    bool uploadFile(const string& filename) {
        return true;
    }

    bool downloadFile(const string& filename) {
        return true;
    }

    // 查看当前目录下的文件
    bool listCWDFiles() {
        if (!needLowLevelPermission(nowLevelPermission)) {
            sendMessage(socket, "权限不足");
            return false;

        }

        string command = "dir";
        FILE* pipe = _popen(command.c_str(), "r");
        if (!pipe) {
            sendMessage(socket, "执行dir命令失败");
            return false;
        }

        string result;
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        _pclose(pipe);

        sendMessage(socket, result.c_str());
        return true;
    }

    // 查看指定目录下的文件
    bool listFiles(const string& path) {
        return true;
    }
public:
    CommandParser(Socket& socket, char& nowLevelPermission) : socket(socket), nowLevelPermission(nowLevelPermission) {}

    ~CommandParser() {}
    // 命令结构体
    struct Command {
        string operation;
        vector<string> args;
    };

    Command parse(const string& input) {
        Command cmd;
        istringstream iss(input);
        string token;
        
        // 读取操作
        iss >> cmd.operation;
        
        // 读取参数
        while (iss >> token) {
            cmd.args.push_back(token);
        }
        
        return cmd;
    }
    
    bool execute(const Command& cmd) {
        // 下载文件
        if (cmd.operation == "download") {
            if (cmd.args.size() < 1) {
                cout << "用法: download <filename>" << endl;
                return false;
            }
            // 处理下载
            return downloadFile(cmd.args[0]);
        }
        // 上传文件
        else if (cmd.operation == "upload") {
            if (cmd.args.size() < 1) {
                cout << "用法: upload <filename>" << endl;
                return false;
            }
            // 处理上传
            return uploadFile(cmd.args[0]);
        }
        // 检查权限
        else if (cmd.operation == "check") {
            return checkHighPermission(cmd.args[0]);
        }
        // 查看当前目录下的文件
        else if (cmd.operation == "listcwd") {
            return listCWDFiles();
        }
        // 查看指定目录下的文件
        else if (cmd.operation == "list") {
            return listFiles(cmd.args[0]);
        }
        return false;
    }
};

// 使用示例
// void handleCommand(const char* command) {

//     auto cmd = CommandParser::parse(command);
//     if (CommandParser::execute(cmd)) {
//         cout << "命令执行成功" << endl;
//     } else {
//         cout << "命令执行失败" << endl;
//     }
// }