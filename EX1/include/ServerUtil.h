#pragma once
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <errno.h>
#include <winerror.h>

#define ERROR_BUF_LEN 512
#define DEFAULT_BUFLEN 512

//functions
void printWSAError();
BOOL send_safe(SOCKET* socket, char* buffer,int* iResult);
BOOL sendto_safe(SOCKET* socket, char* buffer,sockaddr_in* to,int tolen,int* iResult);
BOOL recvfrom_safe(SOCKET* socket, char* buffer,sockaddr_in* from,int* fromlen,int* iResult);
BOOL recv_safe(SOCKET* socket, char* buffer,int* iResult);

//text color
void text_green();
void text_red();
void text_reset();