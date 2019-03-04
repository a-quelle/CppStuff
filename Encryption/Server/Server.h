#pragma once
#include <winsock2.h>
#include "..\LargeNumber\LargeNumber.h"
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Server{
    WSADATA wsadata;
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET clientSocket;
    char *recvBuffer;
    LargeNumber& modulus;
    LargeNumber& privateExp;
    in_addr ip;

public:
    void send(const LargeNumber& message);
    LargeNumber recv();
    const in_addr* getIp()const;
    Server(LargeNumber& modulus, LargeNumber& privateExp);
    void accept();
    LargeNumber decrypt(const LargeNumber& encrypted);
    ~Server();
    void print() const;

};