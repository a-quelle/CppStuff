#include "Client.h"

using namespace std;

LargeNumber Client::encrypt(const LargeNumber& message){
    return LargeNumber::modularPower(message, publicExp, modulus);
}

void Client::print(){
    cout << recvBuffer << endl;
}

void Client::send(const LargeNumber& message){
    int iResult;
    if(message.getSize()>512)
    {
        cout << "Message cannot be larger than 512 bytes" << endl;
        return;
    }
    // Send an initial buffer
    iResult = ::send(connectSocket, (const char*)message.getData(), (int) message.getSize(), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
    }
}

LargeNumber Client::recv(){
    int iResult;

    iResult = ::recv(connectSocket, recvBuffer, 512, 0);
    if (iResult > 0)
        printf("Bytes received: %d\n", iResult);
    else if (iResult == 0)
        printf("Connection closed\n");
    else
        printf("recv failed: %d\n", WSAGetLastError());
    uint8_t* numberArray = new uint8_t[iResult];
    copy(recvBuffer, recvBuffer + iResult, numberArray);
    return LargeNumber(numberArray, iResult);
}

Client::~Client(){
    std::cout << "Releasing socket and destructing Client..." << std::endl;
    delete[] recvBuffer;
    closesocket(connectSocket);
    std::cout << "done!" << std::endl;
}

Client::Client(const in_addr *addr, LargeNumber& modulus, LargeNumber& exp): modulus(modulus), publicExp(exp){
    std::cout << "Constructing client and initialising socket..." << std::endl;  
    recvBuffer = new char[512]{0};
    addrinfo *result = nullptr,
                hints;
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    #define DEFAULT_PORT "27015"

    char ipString[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, addr, ipString, INET_ADDRSTRLEN);
    // Resolve the server address and port
    int iResult = getaddrinfo(ipString, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    //Connect to first address    
    connectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    
    if (connectSocket == INVALID_SOCKET) 
    {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    exit(EXIT_FAILURE);
    }

    // Connect to server.
    iResult = connect(connectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
    }

    if (connectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    std::cout << "done!" << std::endl;
};