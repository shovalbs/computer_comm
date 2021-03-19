#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <errno.h>
#include <winerror.h>
#include "ServerUtil.h"
#include "time.h"

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
int main(int argc, char** argv) {

  //global objects
  WSADATA wsaData;
  int iResult;
  char recvbuf[DEFAULT_BUFLEN];
  int iSendResult;
  int recvbuflen = DEFAULT_BUFLEN;

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
 

  // Setup the UDP listening socket
  iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
      printf("bind failed with error: %d\n", WSAGetLastError());
      freeaddrinfo(result);
      closesocket(ListenSocket);
      WSACleanup();
      return 1;
  }
  printf("socket bound\n");
  freeaddrinfo(result);

  //listen on socket for client
  // printf("listening for client\n");
  // if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
  // printf( "Listen failed with error: %ld\n", WSAGetLastError());
  // printWSAError();
  // closesocket(ListenSocket);
  // WSACleanup();
  // return 1;
  // }
  // printf("accept client\n");
  // //client socket
  // SOCKET ClientSocket;

  // // Accept a client socket
  // ClientSocket = accept(ListenSocket, NULL, NULL);
  // if (ClientSocket == INVALID_SOCKET) {
  //     printf("accept failed: %d\n", WSAGetLastError());
  //     closesocket(ListenSocket);
  //     WSACleanup();
  //     return 1;
  // }
   printf("client connected reciving data\n");

  // Receive until the peer shuts down the connection
  do
  {
    printf("reciveing data from socket\n");
    iResult = recv(ListenSocket,recvbuf,recvbuflen,0);
    if(iResult > 0){
      printf("%s",recvbuf);
    } 
    else if(iResult == 0){
      printf("connection closing\n");
      break;
    }
    else{
      printf("connection failed: %d\n",WSAGetLastError());
      closesocket(ListenSocket);
      WSACleanup();
      return 1;
    }
  } while (TRUE);

  //shutdown channle
  iResult = shutdown(ListenSocket,SD_BOTH);
  if(iResult == SOCKET_ERROR){
      printf("shutdown failed: %d\n", WSAGetLastError());
      closesocket(ListenSocket);
      WSACleanup();
      return 1;  
  }

// cleanup
closesocket(ListenSocket);
WSACleanup();
printf("server - client shutdown cleanly\n");

  

  return 0;
}



