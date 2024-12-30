#include <iostream>
#include "fileOperation.h"
#include "socket.h"
#include "utils.h"

int main() {
    init_Socket();
    SOCKET socket = create_clientSocket("192.168.36.1");
    char buffer[1024] = { 0 };
    recv(socket, buffer, 1024, 0);
    cout << buffer << endl;
    
    send(socket, "123456", strlen("123456"), 0);
    recv(socket, buffer, 1024, 0);
    cout << buffer << endl;

    send(socket, "check 0123456789ABCDEF", strlen("check 0123456789ABCDEF"), 0);
    recv(socket, buffer, 1024, 0);
    cout << buffer << endl;

    send(socket, "upload 1.txt", strlen("upload 1.txt"), 0);
    sendFile(socket, "1.txt");
    recv(socket, buffer, 1024, 0);
    cout << buffer << endl;


    // char fileName[100] = { 0 };
	// cout << "input filename to save:" << endl;
	// cin>>fileName;
	// recvFile(socket, fileName, buffer);

    closesocket(socket);
    system("pause");
    return 0;
}

