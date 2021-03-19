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

int main(int argc, char** argv) {
  //argv - 1:port
  //global objects
  WSADATA wsaData;
  int iResult;
  char recvbuf[DEFAULT_BUFLEN];
  int iSendResult;

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

  iResult = getaddrinfo(NULL, argv[1], &hints, &result);
  if (iResult != 0) {
      printf("getaddrinfo failed: %s\n", strerror(iResult));
      WSACleanup();
      return 1;
  }
  printf("socket created\n");

  //create socket

  SOCKET ReceiverSocket = INVALID_SOCKET;
  ReceiverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (ReceiverSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    return 1;
  }
 

  // Setup the UDP listening socket
  iResult = bind( ReceiverSocket, result->ai_addr, (int)result->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
      printf("bind failed with error: %d\n", WSAGetLastError());
      freeaddrinfo(result);
      closesocket(ReceiverSocket);
      WSACleanup();
      return 1;
  }
  printf("socket bound\n");
  freeaddrinfo(result);

  printf("client connected reciving data\n");

  // Receive until the peer shuts down the connection
  do
  {
    printf("reciveing data from socket\n");
    //iResult = recv(ReceiverSocket,recvbuf,DEFAULT_BUFLEN,0);
    if(!recv_safe(&ReceiverSocket,recvbuf)) return 1;
    if(iResult > 0){
      printf("%s",recvbuf);
    } 
    else if(iResult == 0){
      printf("connection closing\n");
      break;
    }
    else{
      printf("connection failed: %d\n",WSAGetLastError());
      closesocket(ReceiverSocket);
      WSACleanup();
      return 1;
    }
  } while (TRUE);

  //shutdown channle
  iResult = shutdown(ReceiverSocket,SD_BOTH);
  if(iResult == SOCKET_ERROR){
      printf("shutdown failed: %d\n", WSAGetLastError());
      closesocket(ReceiverSocket);
      WSACleanup();
      return 1;  
  }

// cleanup
closesocket(ReceiverSocket);
WSACleanup();
printf("server - client shutdown cleanly\n");

  

  return 0;
}



