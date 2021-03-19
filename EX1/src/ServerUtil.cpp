#include "ServerUtil.h"
//functions

void printWSAError(){
  char errorMessage[ERROR_BUF_LEN];
  int errnum = WSAGetLastError();
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                errnum,
                MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
                errorMessage,
                ERROR_BUF_LEN,NULL);
  printf("%s\n",errorMessage);

}

BOOL send_safe(SOCKET* socket, char* buffer){
  int  iResult = send(*socket,buffer,DEFAULT_BUFLEN,0);
  if(iResult == SOCKET_ERROR){
    printWSAError();
    closesocket(*socket);
    WSACleanup();
    return FALSE;
  }
  return TRUE;
}

BOOL recv_safe(SOCKET* socket, char* buffer){
  int  iResult = recv(*socket,buffer,DEFAULT_BUFLEN,0);
  if(iResult == SOCKET_ERROR){
    printWSAError();
    closesocket(*socket);
    WSACleanup();
    return FALSE;
  }
  return TRUE;
}

