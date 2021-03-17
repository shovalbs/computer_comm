#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <errno.h>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "27015"
int main(int argc, char** argv) {

  //global objects
  WSADATA wsaData;
  int iResult;
  int errnum;

  //Winsock init
  iResult = WSAStartup(MAKEWORD(2,2),&wsaData);
  if(iResult != 0){
    printf("WSAStartup failed: %s\n",strerror(iResult));
    return 1;
  }
  printf("winsock initialized\n");

  struct addrinfo *result = NULL, *ptr = NULL, hints;

  ZeroMemory( &hints, sizeof(hints) );
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;

  // Resolve the server address and port
  iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
  if (iResult != 0) {
      printf("getaddrinfo failed: %d\n", iResult);
      WSACleanup();
      return 1;
  }
  printf("server adress and port resolved\n");

  SOCKET ConnectSocket = INVALID_SOCKET;

  // Attempt to connect to the first address returned by
  // the call to getaddrinfo
  ptr=result;

  // Create a SOCKET for connecting to server
  ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
      ptr->ai_protocol);

  if (ConnectSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    return 1;
}
printf("socket object created\n");

// Connect to server.
iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
if (iResult == SOCKET_ERROR) {
    closesocket(ConnectSocket);
    ConnectSocket = INVALID_SOCKET;
}
freeaddrinfo(result);
printf("socket found\n");

if (ConnectSocket == INVALID_SOCKET) {
    printf("Unable to connect to server!\n");
    WSACleanup();
    return 1;
}
printf("connected to server\n");

  return 0;
}

