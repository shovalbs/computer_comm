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
int main(int argc, char** argv) {
  //argv  1:sender_port 2:receiver_ip 3:receiver_port
  //global objects
  WSADATA wsaData;
  int iResult;
  char recvbuf[DEFAULT_BUFLEN];
  char sendbuf[DEFAULT_BUFLEN];
  int iSendResult;
  int recvbuflen = DEFAULT_BUFLEN;

  //address structs
  struct sockaddr_in sender_addr;
  struct sockaddr_in recver_addr;
  int sender_addr_size = sizeof(sender_addr);
  int recver_addr_size = sizeof(recver_addr);

  //Winsock init
  iResult = WSAStartup(MAKEWORD(2,2),&wsaData);
  if(iResult != 0){
    printf("WSAStartup failed: %s\n",strerror(iResult));
    return 1;
  }
  printf("winsock initialized\n");

  //fill socket parameters
  struct addrinfo *recv_result = NULL, *send_result = NULL, *ptr = NULL, hints;

  ZeroMemory(&hints, sizeof (hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;
  hints.ai_flags = AI_PASSIVE;

  //resolve socket address
  iResult = getaddrinfo(argv[2], argv[3], &hints, &recv_result);
  if (iResult != 0) {
      printf("getaddrinfo failed: %s\n", strerror(iResult));
      WSACleanup();
      return 1;
  }
    iResult = getaddrinfo(NULL, argv[1], &hints, &send_result);
  if (iResult != 0) {
      printf("getaddrinfo failed: %s\n", strerror(iResult));
      WSACleanup();
      return 1;
  }
  printf("socket address resolved\n");
  //create socket

  SOCKET ChannelRecvSocket = INVALID_SOCKET;
  SOCKET ChannelSendSocket = INVALID_SOCKET;

  ChannelRecvSocket = socket(recv_result->ai_family, recv_result->ai_socktype, recv_result->ai_protocol);
  if (ChannelRecvSocket == INVALID_SOCKET) {
    printf("receiver socket:\n");
    printWSAError();
    freeaddrinfo(recv_result);
    WSACleanup();
    return 1;
  }
  printf("receiver socket created\n");
  ChannelSendSocket = socket(send_result->ai_family, send_result->ai_socktype, send_result->ai_protocol);
  if (ChannelSendSocket == INVALID_SOCKET) {
    printf("sender socket:\n");
    printWSAError();
    freeaddrinfo(send_result);
    WSACleanup();
    return 1;
  }
  printf("sender socket created\n");

  // bind sender socket
  iResult = bind( ChannelSendSocket, send_result->ai_addr, (int)send_result->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
      printWSAError();
      freeaddrinfo(send_result);
      closesocket(ChannelSendSocket);
      WSACleanup();
      return 1;
  }
  printf("sender socket bound\n");
  freeaddrinfo(send_result);


  //connect receiver socket
  iResult = connect( ChannelRecvSocket, recv_result->ai_addr, (int)recv_result->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
      closesocket(ChannelRecvSocket);
      ChannelRecvSocket = INVALID_SOCKET;
      printWSAError();
  }
  freeaddrinfo(recv_result);
  printf("receiver socket found\n");

  if (ChannelRecvSocket == INVALID_SOCKET) {
      printf("Unable to connect to server!\n");
      WSACleanup();
      return 1;
  }
  printf("connected to reciver socket\n");

  // Receive until the peer shuts down the connection
  do
  {
    printf("reciveing data from sender socket\n");
    //receive msg from sender
    if(!recvfrom_safe(&ChannelSendSocket,recvbuf,&sender_addr,&sender_addr_size,&iResult)) return 1;
    if(iResult>0){
      printf("from sender: %s\n",recvbuf);
      //if(!sendto_safe(&ChannelRecvSocket,recvbuf,&recver_addr,recver_addr_size,&iResult)) return 1;
      if(!send_safe(&ChannelRecvSocket,recvbuf,&iResult));
      printf("message sent to receiver\n");
      if(!recvfrom_safe(&ChannelRecvSocket,recvbuf,&recver_addr,&recver_addr_size,&iResult)) return 1;
      if(iResult > 0){
        printf("from receiver: %s\n",recvbuf);
        strcpy(sendbuf,"response to sender from channel\n");
        if(!sendto_safe(&ChannelSendSocket,sendbuf,&sender_addr,sender_addr_size,&iResult)) return 1;
        printf("message sent to sender\n");
      }
      else{
        printf("mesage from receiver empty\n");
        break;
      }
    }
    else{
      printf("message from sender empty\n");
      break;
    }

  } while (TRUE);

  //shutdown channle
  iResult = shutdown(ChannelSendSocket,SD_BOTH);
  if(iResult == SOCKET_ERROR){
      printf("shutdown failed: %d\n", WSAGetLastError());
      closesocket(ChannelSendSocket);
      WSACleanup();
      return 1;  
  }

// cleanup
closesocket(ChannelSendSocket);
closesocket(ChannelRecvSocket);
WSACleanup();
printf("server - client shutdown cleanly\n");

  

  return 0;
}



