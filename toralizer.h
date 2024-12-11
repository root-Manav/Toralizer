#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//linux header files
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>



#define PROXY       "127.0.0.1"
#define PROXYPORT    9050
#define USERNAME "torliz"
#define reqsize sizeof(struct proxy_request)
#define ressize sizeof(struct proxy_response)


typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

/*
    Connect protocal in SOCKS (Socket Secure)

+----+----+----+----+----+----+----+----+----+----+....+----+
| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
+----+----+----+----+----+----+----+----+----+----+....+----+
   1    1      2              4           variable       1

*/


struct proxy_request{
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[8]; //username torlize defined in macro

};

typedef struct proxy_request Req;

/*

VN always zero, cd contains requests output, other fields are ignored

+----+----+----+----+----+----+----+----+
| VN | CD | DSTPORT |      DSTIP        |
+----+----+----+----+----+----+----+----+
   1    1      2              4

*/

struct proxy_response{
    int8 vn;
    int8 cd;
    int16 _;
    int32 __;
};

typedef struct proxy_response Res;

Req *request(const char *,const int);
int main(int , char **); //writing main fuction here so request function is above main function
