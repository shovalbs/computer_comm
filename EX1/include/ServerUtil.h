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
BOOL send_safe(SOCKET* socket, char* buffer);
BOOL recv_safe(SOCKET* socket, char* buffer);