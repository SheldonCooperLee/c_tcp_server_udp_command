/*
    C socket server example
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#define MAX_LINE 40960

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
    
 
int main(int argc , char *argv[])
{
   // int i;
    int socket_desc , client_sock , c , read_size,re;
    struct sockaddr_in server , client;
  //  unsigned char client_message[40960];
 unsigned char recvBuf[40960], tmpBuf[13000];	
	struct   timeval   start,stop,diff,everytime; 
    int tmp[3600];
	double avrData = 0;
	int length=0;
    int counter=0;
    int i,num,line;
     char filename[14]="";
     FILE *file;
      
    int a,s,cs;
	//double wztem[20];
	//int tmp1, tmp2, tmp0;
	//初始化//
	line=1; 
    num=0; 
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
  int   opt   =   1; 
 re=setsockopt( socket_desc,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt)); 
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 6007 );
     
    
   re=bind(socket_desc,(struct sockaddr *)&server , sizeof(server));
    
     /*   //print the error message
        perror("bind failed. Error");
        return 1;
    }*/
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
     
    //Receive a message from client
  //  while( (read_size = recv(client_sock , client_message , 1536 , 0)) > 0 )
     gettimeofday(&start,0);
        sprintf(filename,"%d.dat",start.tv_sec);
      file=fopen(filename,"a"); 
 for(cs=0;cs<10;cs++){
    while(counter<1441){
       length=recv(client_sock,recvBuf,14410,0);
	   memmove(tmpBuf+counter,recvBuf,length);
       counter=length+counter;
	    }
	/*receive hex RecvBuf*/
	 /* printf("%02X\n ",tmpBuf[0]);
        for(i=1;i<counter;i++){
               if(num==0){
                     printf("the line is %d:",line);
                }
               if(num<4) num++;
              printf("%02X ",tmpBuf[i]);
		      
               if(num==4) {
                       num=0;       
					   line++;
                       printf("\n");          
					   }   
            }*/
    for(a=0;a<3600;a++)
      {
      gettimeofday(&everytime,0);
      tmp[a]= ((int)(tmpBuf[4*a+1]+tmpBuf[4*a+2]+tmpBuf[4*a+3]))*4.096 *2.979 / 1.25 /8388607;
      printf("the line is %d:%f\n",a+1,tmp[a]);
        fprintf(file,"%d%d:%f\n",everytime.tv_sec,everytime.tv_usec,tmp[a]);

          }
      gettimeofday(&stop,0);   
      timeval_subtract(&diff,&start,&stop); 
      }
      fclose(file);
      printf("\nget data end!total length is:%d\n",counter*cs); 
	  printf("start : %d\n%d\n", start.tv_sec, start.tv_usec);
      printf("stop  : %d\n%d\n", stop.tv_sec, stop.tv_usec);
	  printf("timeaccount:%d \n %d\n",diff.tv_sec,diff.tv_usec); 
       //counter=0;
  

// printf("%02X\n ",tmpBuf[7205]);
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
     
    return 0;
}
