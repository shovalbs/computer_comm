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
  char sendbuf[DEFAULT_BUFLEN];
  int iSendResult;

  //channel address
  struct sockaddr_in channel_addr;
  int channel_addr_size = sizeof(channel_addr);

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
  char* arg = argv[1];
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

  printf("client connected reciving data\n");
  freeaddrinfo(result);
  // Receive until the peer shuts down the connection
  do
  {
    printf("reciveing data from socket\n");

    iResult = recvfrom(ReceiverSocket,recvbuf,DEFAULT_BUFLEN,0,(SOCKADDR*) &channel_addr,&channel_addr_size);
    if(iResult == SOCKET_ERROR){
      printWSAError();
      closesocket(ReceiverSocket);
      WSACleanup();
      return 1;
    }
    printf("channel address: %s\n",inet_ntoa(channel_addr.sin_addr));
    if(iResult > 0){
      text_green();
      printf("%s",recvbuf);
      text_reset();
      strcpy(sendbuf,"message recieved by receiver\n");
      iResult = sendto(ReceiverSocket,sendbuf,DEFAULT_BUFLEN,0,(SOCKADDR*) &channel_addr,channel_addr_size);
      if(iResult == SOCKET_ERROR){
        printWSAError();
        closesocket(ReceiverSocket);
        WSACleanup();
        return 1;
      }
      printf("message sent to channel\n");
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



