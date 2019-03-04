#include "Server.h"

using namespace std;

LargeNumber Server::decrypt(const LargeNumber& encrypted){
    return LargeNumber::modularPower(encrypted, privateExp, modulus);
}

void Server::print() const{
    cout << recvBuffer << endl;
}

void Server::send(const LargeNumber& message){
    int iResult;
    if(message.getSize()>512)
    {
        cout << "Message cannot be larger than 512 bytes" << endl;
        return;
    }

    iResult = ::send(clientSocket, (const char*)message.getData(), (int) message.getSize(), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
    }
}

LargeNumber Server::recv(){
    int iResult;

    iResult = ::recv(clientSocket, recvBuffer, 512, 0);
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

void Server::accept(){
    std::cout << "Accept" << std::endl;
    
    if ( ::listen( listenSocket, 10 ) == SOCKET_ERROR ) {
    printf( "Listen failed with error: %ld\n", WSAGetLastError() );
    closesocket(listenSocket);
    WSACleanup();
    exit(EXIT_FAILURE);
    }

    clientSocket = ::accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    cout << "Client connection accepted!" << endl;
}

Server::Server(LargeNumber& modulus, LargeNumber& exp) : modulus(modulus), privateExp(exp) {
    std::cout << "Constructing server and initialising WSA and socket..." << std::endl;
    recvBuffer = new char[512]{0};
    #define LOCAL_IP "192.168.1.101"      
    #define DEFAULT_PORT "27015"
    int iResult; 
    
    struct addrinfo *result= nullptr, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(LOCAL_IP, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    ip = ((sockaddr_in*) result->ai_addr)->sin_addr;
    
    freeaddrinfo(result);

    std::cout << "done!" << std::endl;
}

const in_addr* Server::getIp()const {
    return &ip;
}

Server::~Server(){
    cout << "Releasing socket and destructing Server..." << endl;
    delete[] recvBuffer;
    closesocket(listenSocket);
    cout << "done!" << endl;
}