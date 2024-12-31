#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "socket.h"
#include "utils.h"
#include "fileOperation.h"

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
            sendMessage(socket, "高级权限验证成功，请输入指令\n");
            sendMessage(socket, getSupportCommandMsg().c_str());
            return true;
        } else {
            sendMessage(socket, "高级权限验证失败\n");
            return false;
        }
    }

    bool uploadFile(const string& filename) {
       if (!needHighLevelPermission(nowLevelPermission)) {
            sendMessage(socket, "权限不足\n");
            return false;
       }
       
       if (recvFile(socket, filename.c_str())) {
            return true;
       }
       return false;
    }

    bool downloadFile(const string& filename) {
        if (!needLowLevelPermission(nowLevelPermission)) {
            sendMessage(socket, "权限不足");
            return false;

        }

        if (sendFile(socket, filename.c_str())) {
            return true;
        }
        return false;
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
        if (!needHighLevelPermission(nowLevelPermission)) {
            sendMessage(socket, "权限不足");
            return false;
        }

        string command = "dir \"" + path + "\"";  // 添加引号以支持包含空格的路径
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

    // 加密指定文件
    bool encryptFile(const string& filename) {
        if (!needHighLevelPermission(nowLevelPermission)) {
            sendMessage(socket, "权限不足");
            return false;
        }

        // 打开源文件
        FILE* readFile = fopen(filename.c_str(), "rb");
        if (!readFile) {
            sendMessage(socket, "文件打开失败");
            return false;
        }

        // 创建临时文件
        string tempFilename = filename + ".tmp";
        FILE* writeFile = fopen(tempFilename.c_str(), "wb");
        if (!writeFile) {
            fclose(readFile);
            sendMessage(socket, "创建临时文件失败");
            return false;
        }

        const int BLOCK_SIZE = 8; // DES以64位（8字节）为一个块进行加密
        unsigned char buffer[BLOCK_SIZE];
        char hexBuffer[BLOCK_SIZE * 2 + 1]; // 每个字节转为2个16进制字符，加1是为了存储结束符'\0'

        // 按块读取并加密
        size_t bytesRead;
        while ((bytesRead = fread(buffer, 1, BLOCK_SIZE, readFile)) > 0) {
            // 将二进制数据转换为16进制字符串
            for (size_t i = 0; i < bytesRead; i++) {
                sprintf(hexBuffer + (i * 2), "%02X", buffer[i]);
            }
            
            // 如果不足8字节，用0填充
            if (bytesRead < BLOCK_SIZE) {
                for (size_t i = bytesRead; i < BLOCK_SIZE; i++) {
                    sprintf(hexBuffer + (i * 2), "00");
                }
            }

            // 使用DES加密
            string encryptedHex = getDesEncryptResult(hexBuffer);

            // 将加密后的16进制字符串转回二进制
            for (size_t i = 0; i < encryptedHex.length(); i += 2) {
                string byteStr = encryptedHex.substr(i, 2);
                unsigned char byte = (unsigned char)strtol(byteStr.c_str(), NULL, 16);
                fwrite(&byte, 1, 1, writeFile);
            }
        }

        // 清理资源
        fclose(readFile);
        fclose(writeFile);

        // 替换原文件
        remove(filename.c_str());
        rename(tempFilename.c_str(), filename.c_str());

        sendMessage(socket, "文件加密成功");
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
                sendMessage(socket, "用法: download <filename>");
                return false;
            }
            // 处理下载
            return downloadFile(cmd.args[0]);
        }
        // 上传文件
        else if (cmd.operation == "upload") {
            if (cmd.args.size() < 1) {
                sendMessage(socket, "用法: upload <filename>");
                return false;
            }
            // 处理上传
            return uploadFile(cmd.args[0]);
        }
        // 检查权限
        else if (cmd.operation == "check") {
            if (cmd.args.size() < 1) {
                sendMessage(socket, "用法: check <password>");
                return false;
            }
            return checkHighPermission(cmd.args[0]);
        }
        // 查看当前目录下的文件
        else if (cmd.operation == "listcwd") {
            if (cmd.args.size() > 0) {
                sendMessage(socket, "用法: listcwd");
                return false;
            }
            return listCWDFiles();
        }
        // 查看指定目录下的文件
        else if (cmd.operation == "list") {
            if (cmd.args.size() < 1) {
                sendMessage(socket, "用法: list <path>");
                return false;
            }
            return listFiles(cmd.args[0]);
        }
        // 加密指定文件
        else if (cmd.operation == "encrypt") {
            if (cmd.args.size() < 1) {
                sendMessage(socket, "用法: encrypt <filename>");
                return false;
            }
            return encryptFile(cmd.args[0]);
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