#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "ServerUtil.h"

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char** argv) {
  //params order 1:ip 2:port 3:filename
  //global objects
  WSADATA wsaData;
  int iResult;
  int errnum;
  char sendbuf[DEFAULT_BUFLEN];
  char recvbuf[DEFAULT_BUFLEN];

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

  struct addrinfo *result = NULL, *ptr = NULL, hints;

  ZeroMemory( &hints, sizeof(hints) );
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;

  // Resolve the server address and port
  //argv[1] = ip argv[2] = port
  iResult = getaddrinfo(argv[1],argv[2], &hints, &result);
  if (iResult != 0) {
      printWSAError();
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

  // Connect to server on specified port
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

  //send startup message 
  strcpy(sendbuf,"hello server\n");
  //if(!sendto_safe(&ConnectSocket,sendbuf,&channel_addr,channel_addr_size,&iResult)) return 1;
  if(!send_safe(&ConnectSocket,sendbuf,&iResult)) return 1;
  printf("connection message sent\n");
  if(!recvfrom_safe(&ConnectSocket,recvbuf,&channel_addr,&channel_addr_size,&iResult)) return 1;
  if(iResult > 0){
    printf("%s",recvbuf);
  }
  //wait for response

  //cleanup
  closesocket(ConnectSocket);
  WSACleanup();
  printf("client closed cleanly\n");
  return 0;
}

