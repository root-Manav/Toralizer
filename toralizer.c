#include "toralizer.h"

/*

./toralize <ipaddress> <port>
     0         1         2
*/

Req *request(const char *dstip, const int dstport){
    Req *req;
    req = malloc(reqsize);

    req -> vn = 4;
    req -> cd = 1;
    req -> dstport = htons(dstport);
    req -> dstip = inet_addr(dstip);
    strncpy(req -> userid, USERNAME, 8); //8 is the size allocated
    return req;
}


int main(int argc, char *argv[]){
    char *host;
    int port, s;

    //To access the struct elements easily
    Req *req;
    Res *res;

    char buf[ressize];
    struct sockaddr_in sock;
    int success; //to store boolean value
    char tmp[512];

    if (argc < 3){
        //defining usage
        fprintf(stderr, "usage: %s <host> <port>", argv[0]);
        return -1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0){
        perror("socket");
        return -1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    if(connect(s, (struct sockaddr *)&sock, sizeof(sock))){ //on a succesful connection this returns zero, meaning
        perror("connection failed\n");                      //this if statement will run if it is other than zero
        return -1;                                          //stating the connection failed.
    }

    printf("Connected to proxy\n");
    req = request(host, port);

    write(s, req, reqsize);
    memset(buf, 0 ,ressize);

    if (read(s, buf, ressize) < 1){
        perror("read");
        free(req);
        close(s);
        return -1;
    }

    res = (Res *)buf;
    success = (res->cd == 90); //90 means success, check SOCKS rfc
    if (!success){
        fprintf(stderr, "Unable to traverse the proxy "
        "error code: %d", res->cd);
        close(s);
        free(req);
        return -1;
    }

    //this block will be removed after we are done with the whole project
    printf("Succesfully connected through the proxy to: %s:%d", host,port);

    memset(tmp, 0, 512);
    snprintf(tmp, 511, 
         "HEAD / HTTP/1.0\r\nHost: %s\r\n\r\n", host);

    write(s, tmp, strlen(tmp));
    memset(tmp, 0, 512);
    read(s, tmp, 511);

    close(s);
    free(req);
    return 0;
}