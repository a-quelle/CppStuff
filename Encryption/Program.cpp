#pragma comment(lib, "Ws2_32.lib")
#include "Client\Client.h"
#include "Server\Server.h"
#include "LargeNumber\LargeNumber.h"
#include <iostream>
#include <windows.h>
#include <thread>


using namespace std;

LargeNumber n({char(0x15), char(0xab), char(0x0c), char(0x30), char(0x60), char(0x60), char(0x86), char(0xfd), char(0x7d), char(0xd3), char(0x03), char(0xb8),
 char(0x15), char(0x6e), char(0x12), char(0xc7), char(0xb7), char(0xe8), char(0x96), char(0x11), char(0x34), char(0x45), char(0x73), char(0x83), char(0xf0),
  char(0xd1), char(0x27), char(0x4b), char(0x2d), char(0x4f), char(0x66), char(0xdd)});

LargeNumber d({char(0x81), char(0x98), char(0xda), char(0xdd), char(0xa3), char(0xdd), char(0x93), char(0xf9), char(0x03), char(0x2b), char(0x1c), char(0x0e),
 char(0x50), char(0x34), char(0xd3), char(0xad), char(0x23), char(0xa2), char(0x60), char(0x82), char(0xe5), char(0x81), char(0xeb), char(0x1c), char(0x69),
  char(0x1b), char(0x29), char(0x3b), char(0x42), char(0x7c), char(0xa9), char(0x02)});

LargeNumber e({char(0x01), char(0x00), char(0x01)});

void initWinsock(WSADATA& wsaData){ 
     
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        exit(EXIT_FAILURE);
    } 
}

void threadFunction(const Server& server){
    Client client(server.getIp(),n,e);
    LargeNumber message("Client says: Hello the server.");
    cout << "Before encyption: " << message << endl;
    LargeNumber encrypted(client.encrypt(message));
    cout << "After encryption: " << encrypted << endl;
    client.send(encrypted);
}

int main(){
    // Initialize Winsock    
    WSADATA wsaData;
    initWinsock(wsaData);

    Server server(n, d);
    thread clientThread(threadFunction, ref(server));
    server.accept();
    LargeNumber result = server.recv();
    cout << "Before decryption: "<< result << endl;
    LargeNumber decrypted(server.decrypt(result));
    cout << "After decryption: " << decrypted << endl;
    clientThread.join();
    WSACleanup();
}