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

  //fill socket parameters
  struct addrinfo *result = NULL, *ptr = NULL, hints;

  ZeroMemory(&hints, sizeof (hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;
  hints.ai_flags = AI_PASSIVE;

  iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
  if (iResult != 0) {
      printf("getaddrinfo failed: %s\n", strerror(iResult));
      WSACleanup();
      return 1;
  }
  printf("socket created\n");

  //create socket

  SOCKET ListenSocket = INVALID_SOCKET;
  ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (ListenSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    return 1;
  }
  printf("socket created\n");

      // Setup the UDP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    printf("socket binded\n");

  return 0;
}



