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