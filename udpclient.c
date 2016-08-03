/*
    Simple udp client
    gcc udpclient.c -o udpclient && ./udpclient
*/

#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#include<sys/time.h> 

#define SERVER "192.168.1.63"
#define CLIENT "192.168.1.103"
#define BUFLEN 40960	     //Max length of buffer
#define CLIENTPORT 16666	//send data on the port
#define SERVERPORT 17777  //send data to the port   

void die ( char *s )
{
	perror(s);
	exit(1);
}

 int   timeval_subtract(struct   timeval*   result,   struct   timeval*   x,   struct   timeval*   y)  //计算linux时差函数 
  {   
        int   nsec;   
    
        if   (   x->tv_sec>y->tv_sec   )   
                  return   -1;   
    
        if   (   (x->tv_sec==y->tv_sec)   &&   (x->tv_usec>y->tv_usec)   )   
                  return   -1;   
    
        result->tv_sec   =   (   y->tv_sec-x->tv_sec   );   
        result->tv_usec   =   (   y->tv_usec-x->tv_usec   );   
    
        if   (result->tv_usec<0)   
        {   
                  result->tv_sec--;   
                  result->tv_usec+=1000000;   
        }   
    
        return   0;   
  } 

int main(void)
{
     struct   timeval   start,stop,diff; 
	struct sockaddr_in si_serv, si_clnt;
	int s, i, slen=sizeof(si_serv),re;
     char buf[BUFLEN];
	 char buf1[BUFLEN];
     char buf2[BUFLEN];
     char buf3[BUFLEN];
     char buf4[BUFLEN];
	char message1[BUFLEN]="USB2407-0.1.000000010001B1/";
	char message2[BUFLEN]="USB2407-0.1.000000001002A0F01010000D00200/";
	char message3[BUFLEN]="USB2407-0.1.000000002001B1/";
	char message4[BUFLEN]="USB2407-0.1.000000002001B0/";
         
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
       {
        die("socket");
       }
 int   opt   =   1; 
  re=setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt)); 
     // zero out the structure  
     memset((char *) &si_clnt ,0, sizeof(si_clnt));
	 si_clnt.sin_family = AF_INET;
     if (inet_aton(CLIENT , &si_clnt.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
	si_clnt.sin_port = htons(CLIENTPORT);

    memset((char *) &si_serv,0, sizeof(si_serv));
	si_serv.sin_family = AF_INET;
    if (inet_aton(SERVER , &si_serv.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
   si_serv.sin_port = htons(SERVERPORT);

  //bind socket to port
  re=bind(s , (struct sockaddr*)&si_clnt, sizeof(si_clnt) ) ;
  
  //while(1)
	//{
		if (sendto(s, message1, strlen(message1) , 0 , (struct sockaddr *) &si_serv, slen)==-1)
		{
			die("sendto()");
		}
		
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf1,'\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf1, BUFLEN, 0, (struct sockaddr *) &si_serv, &slen) == -1)
		{
			die("recvfrom()");
		}
		puts(buf1);
	
         //send the message2
        if (sendto(s, message2, strlen(message2) , 0 , (struct sockaddr *) &si_serv, slen)==-1)
		{
			die("sendto()");
		}

		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf2,'\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf2, BUFLEN, 0, (struct sockaddr *) &si_serv, &slen) == -1)
		{
			die("recvfrom()");
		}
            puts(buf2);
            /*if(buf2==message2[BUFLEN])
		{
		puts(buf2);
                }*/

        //send the message3
        if (sendto(s, message3, strlen(message3) , 0 , (struct sockaddr *) &si_serv, slen)==-1)
		{
			die("sendto()");
		}
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf3,'\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf3, BUFLEN, 0, (struct sockaddr *) &si_serv, &slen) == -1)
		{
			die("recvfrom()");
		}
           gettimeofday(&start,0);
           gettimeofday(&stop,0);   
           timeval_subtract(&diff,&start,&stop); 
           puts(buf3);
           printf("start : %d\n%d\n", start.tv_sec, start.tv_usec);
 
	//}  

	close(s);
	return 0;
}
