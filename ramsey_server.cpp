#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

using namespace std;

class server
  {
     public:
	server(int argc,char *argv[]);
	void chat();
     private:
	int sockfd;
  	struct sockaddr_in my_addr;
	struct sockaddr_in client_addr;
	int client_fd[3],client_temp;
	
  };

server::server(int argc,char *argv[])  //waits until both clients are connected
  {
	if(argc !=2)
    	 {
           cerr << "Usage syntax: ./server <port_no>" << endl;
      	   return;
    	}
  	
        sockfd=socket(PF_INET,SOCK_STREAM,0);  //Creating the socket

  	my_addr.sin_family=AF_INET;
  	my_addr.sin_port=htons(atoi(argv[1]));
  	my_addr.sin_addr.s_addr=INADDR_ANY;
  	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  	int temp=bind(sockfd, (struct sockaddr *)&my_addr, (sizeof my_addr)); //Binding the socket to the address
	if(temp==0)
   	  cout << "Server started running at "<< inet_ntoa(my_addr.sin_addr)  << ":" << argv[1] << endl;
  	else
   	 {
      	   cout << "Failed to bind socket" << endl;
      	   return;
    	  }
	
        listen(sockfd,2);
   	socklen_t client_size=sizeof client_addr;
  
 	client_fd[1]=accept(sockfd,(struct sockaddr*)&client_addr, &client_size);
	cout << "Accepted incoming connection from client 1" << endl;

	client_fd[2]=accept(sockfd,(struct sockaddr*)&client_addr, &client_size);
	cout << "Accepted incoming connection from client 2" << endl;
	
        chat();
  }

void server::chat()    //Game starts...
  {
	char buf[1024];
	int temp,o;
	
	temp=recv(client_fd[1],buf,sizeof buf,0);
	cout << "Message from client 1" << buf << endl;
	
        temp=send(client_fd[2],buf,strlen(buf)+1,0);
	if(temp!=1)
	 cout << "Message sent to client 2" << endl;
	
        temp=recv(client_fd[2],buf,sizeof buf,0);
	cout << "Message from client 2" << buf << endl;
	
        temp=send(client_fd[1],buf,strlen(buf)+1,0);
	if(temp!=1)
	 cout << "Message sent to client 1" << endl;
	
        while(1)
	         {
			sleep(1);
			temp=recv(client_fd[1],buf,sizeof buf,0);
			cout << "Message from client 1" << buf << endl;
			//sleep(1);

			temp=send(client_fd[2],buf,strlen(buf)+1,0);
			if(temp!=1)
			cout << "Message sent to client 2" << endl;

			for(int index=1;index<=6;index++)
			{
				for(int index1=1;index1<=6;index1++)
		        	{
			           temp=recv(client_fd[1],&o,sizeof o,0);  //receives graph from client1
		                   int choice=ntohl(o);
			
                                   if(temp==-1)
			              cout << "message not received " << endl;
			
                                   o=htonl(choice);
			           temp=send(client_fd[2],&o,sizeof o,0);  //sends graph to client2
			           if(temp==1)
			              cout << "Message not sent" << endl;
			        }
			}
			
                        cout << "graph recv and sent" << endl;
	
			temp=recv(client_fd[2],buf,sizeof buf,0);
			cout << "Message from client 2" << buf << endl;
	
			temp=send(client_fd[1],buf,strlen(buf)+1,0);
			if(temp!=1)
			 cout << "Message sent to client 1" << endl;
	
	
                  }
  
		  for(int index=1;index<=2;index++)
			   {
			      close(client_fd[index]);  
			   }

       close(sockfd);
  }

int main(int argc,char *argv[])
  {
	server obj(argc,argv);
	return 0;
  }



