//
// CAN BE COMPILED AND USED FOR TESTING FIRST PACKET
//


#include <stdio.h>      
#include <stdlib.h>    
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <netdb.h>      
#include <string.h>
#include <errno.h>


int sendMessage(int, char*, int, struct sockaddr*, socklen_t);
int readMessage(int, char*, int, struct sockaddr*, socklen_t);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
	printf("usage: client <IPaddress> <Port>\n");
	exit(1);
    }

    int sockfd, msg, attempts, success;
    char recvbuf[1024];
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
	printf("Call to socket failed\n");
	exit(1);
    }
    
    //setsockopt
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));


    //message to be sent to server
/*
   char sendbuf[] = {
0x5a, 0xe7, 0xfe, 0xff, 0xcf, 0xe0, 0x21, 0x5a, 
0xe7, 0x05, 0x00, 0x20, 0x01, 0x00, 0xfb, 0x06, 
0x01, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 
0xfb, 0x3e, 0x02, 0x00, 0x04, 0x09, 0x00, 0x03, 
0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x45, 
0x51, 0x4f, 0x41, 0x0a, 0x00, 0x00, 0x00, 0x6b, 
0x69, 0x65, 0x73, 0x68, 0x61, 0x65, 0x73, 0x68, 
0x61, 0x01, 0xfa, 0x10, 0x69, 0x22, 0x1c, 0xd4, 
0x45, 0xbc, 0xfd, 0x68, 0x3c, 0x56, 0x22, 0x87, 
0xd9, 0x70, 0xb7, 0x1c, 0x12, 0xae, 0x76, 0xc4, 
0x98, 0xfd, 0xf3, 0xce, 0xeb, 0x44, 0x4a, 0x0a, 
0x49, 0xb5, 0xf6, 0xbe, 0x24, 0xe4 }; 
*/
char sendbuf[] = {
0x16, 0x06, 0xfe, 0xff, 0xcf, 0xe0, 0x21, 0x16, 0x06, 0x03, 0x00, 0x20, 0x01, 0x00, 0xfb, 0x06, 0x01, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0xfb, 0x3e, 0x02, 0x00, 0x04, 0x09, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x45, 0x51, 0x4f, 0x41, 0x0a, 0x00, 0x00, 0x00, 0x6b, 0x69, 0x65, 0x73, 0x68, 0x61, 0x65, 0x73, 0x68, 0x61, 0x01, 0xfa, 0x10, 0x69, 0x22, 0x1c, 0xd4, 0x45, 0xbc, 0xfd, 0x68, 0x3c, 0x56, 0x22, 0x87, 0xd9, 0x70, 0xb7, 0x1c, 0x12, 0xae, 0x76, 0xc4, 0x98, 0xfd, 0xf3, 0xce, 0xeb, 0x44, 0x4a, 0x0a, 0x49, 0xb5, 0xcb, 0x2e, 0xa2, 0x49 };
    //arbitrary choice of attempts before giving up 
    attempts = 5;
    success = 0;
    while(attempts > 0 && !success)
    {
	//send a message to the server
	if ((msg = sendMessage(sockfd, sendbuf, sizeof(sendbuf), (struct sockaddr*) &servaddr,
	    sizeof(servaddr))) != 0) 
	{
	    printf("Function sendMessage failed\n");
	    exit(1);
	}

	//read the response from the server
	if ((msg = readMessage(sockfd, recvbuf, sizeof(recvbuf), (struct sockaddr*) &servaddr,
	    sizeof(servaddr))) == 0)
	//we received a response from the server and are finished
	//set success to 1 to terminate attempt loop
	{
	    printf("Success!\n");
	    success = 1;
	}
	//there was an error within the readMessage function
	else if (msg == 1)
	{
	    printf("Function readMessage failed\n");
	    exit(1);
	}
	//else  we timed out, so we subtract 1 from attempts
	//and try the above process again
	
	attempts -= 1;
    }

    //we tried 5 times and failed
    close(sockfd);
    if (attempts == 0)
    {
	printf("Your attempts were unsuccessful\n");
	return 1;
    }
    else
    {
	return 0;
    }

}

//sendMessage takes a socket descriptor, buffer, and buffer length
//prompts for input and sends input to server
int sendMessage(int sockfd, char* buf, int buflen, struct sockaddr *servaddr, socklen_t servlen)
{
    int msg;
    if ((msg = sendto(sockfd, buf, buflen, 0, servaddr, servlen)) < 0)
    {
	printf("Call to sendto failed\n");
	exit(1);
    }
    printf("Message sent\n");
    return 0;
}

//readMessage takes a socket descriptor, buffer, and buffer length
//reads and prints the response sent from the server
int readMessage(int sockfd, char* buf, int buflen, struct sockaddr *servaddr, socklen_t servlen)
{
    int msg;
    if ((msg = recvfrom(sockfd, buf, buflen, 0, servaddr, &servlen)) < 0)
    {
	//check for recvfrom timeout
	if (errno == EWOULDBLOCK)
	{
	    fprintf(stderr,"socket timeout\n");
	    return(2);
	}
	else
	{
	    printf("Call to recvfrom failed\n");
	    return(1);
	}
    }
    printf("Received message: %s\n", buf);
    return 0;
}

