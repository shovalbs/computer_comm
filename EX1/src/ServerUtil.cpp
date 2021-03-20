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

BOOL send_safe(SOCKET* socket, char* buffer,int* iResult){
  *iResult = send(*socket,buffer,DEFAULT_BUFLEN,0);
  if(*iResult == SOCKET_ERROR){
    printWSAError();
    closesocket(*socket);
    WSACleanup();
    return FALSE;
  }
  return TRUE;
}

BOOL sendto_safe(SOCKET* socket, char* buffer,sockaddr_in* to,int tolen,int* iResult){
  *iResult = sendto(*socket,buffer,DEFAULT_BUFLEN,0,(SOCKADDR*)to,tolen);
  if(*iResult == SOCKET_ERROR){
    printWSAError();
    closesocket(*socket);
    WSACleanup();
    return FALSE;
  }
  return TRUE;
}

BOOL recv_safe(SOCKET* socket, char* buffer,int* iResult){
  *iResult = recv(*socket,buffer,DEFAULT_BUFLEN,0);
  if(*iResult == SOCKET_ERROR){
    printWSAError();
    closesocket(*socket);
    WSACleanup();
    return FALSE;
  }
  return TRUE;
}

BOOL recvfrom_safe(SOCKET* socket, char* buffer,sockaddr_in* from,int* fromlen,int* iResult){
  *iResult = recvfrom(*socket,buffer,DEFAULT_BUFLEN,0,(SOCKADDR*) from,fromlen);
  if(*iResult == SOCKET_ERROR){
    printWSAError();
    closesocket(*socket);
    WSACleanup();
    return FALSE;
  }
  printf("sender ip: %s\tsender port: %d\n",inet_ntoa(from->sin_addr),from->sin_port);
  return TRUE;
}


//print colors
void text_green () {
  printf("\033[0;32m");
}
void text_red() {
  printf("\033[0;31m");
}
void text_reset () {
  printf("\033[0m");
}
