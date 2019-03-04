#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "..\LargeNumber\LargeNumber.h"
#include <stdio.h>
#include <string>
#include <iostream>

class Client{
    SOCKET connectSocket = INVALID_SOCKET;
    char *recvBuffer;
    LargeNumber& modulus;
    LargeNumber& publicExp;
public:
    Client(const in_addr* addr, LargeNumber& modulus, LargeNumber& publicExp);
    ~Client();
    void send(const LargeNumber& message);
    LargeNumber recv();
    LargeNumber encrypt(const LargeNumber& message);
    void print();
};