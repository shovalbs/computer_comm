#define UNCODED_LEN 10
#define CODED_LEN 14

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <fstream>


int main(int argv[], int argc[]){
    char readbuf [UNCODED_LEN];
    char codedbuf [CODED_LEN];

    FILE *fp = fopen("../../testFile.txt", "r" );
    int read_count;

    while(true){
        read_count=fread(readbuf,UNCODED_LEN,1,fp);
        printf("read count:%d\n",read_count);
        if (read_count!=1){
            printf("%d\n",feof(fp));
            if (0!=feof(fp)){
                printf("end of file reached\n");
                fclose(fp);
                return 0;
            }
            int errnum=ferror(fp);
            printf("Error in reading file: %s\n",strerror(errnum));
            fclose(fp);
            return 1;
        }

        printf("%s a\n",readbuf);
    }

    return 0;
}


void hamming_encoder(char * uncoded_msg, char* coded_msg){
    // coded_msg[2]=uncoded_msg[0];
    // coded_msg[4]=uncoded_msg[1];
    // coded_msg[5]=uncoded_msg[2];
    // coded_msg[6]=uncoded_msg[3];
    // coded_msg[8]=uncoded_msg[4];
    // coded_msg[9]=uncoded_msg[5];
    // coded_msg[10]=uncoded_msg[6];
    // coded_msg[11]=uncoded_msg[7];
    // coded_msg[12]=uncoded_msg[8];
    // coded_msg[13]=uncoded_msg[9];
    // coded_msg[14]=uncoded_msg[10];
    // coded_msg[15]=uncoded_msg[11];
    // if(0==)
    // coded_msg[0]=


