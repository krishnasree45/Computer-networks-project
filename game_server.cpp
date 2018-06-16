//guessing game between two clients with server
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
using namespace std;

class sock
{
 	private:
		int no_of_clients;	
		int sockfd,newsockfd[10],portno;
 		char buffer[1024];//this is used to take values from server and send values to the server
		struct sockaddr_in serv_addr,cli_addr[10];//to store address of server and client sockets
		int n[10];
 		socklen_t clilen[10];//length of address of sockets
	public:
		sock(int portno);//to create socket
		void connect_client(int no_of_clients);//to connect with clients
		void dualchat();
		int check(char buffer[1024]);
};

sock::sock(int portno)//creaitng socket
{
 	sockfd=socket(AF_INET,SOCK_STREAM,0);//creating a socket

 	if(sockfd<0)
 	{
		cout<<"\nerror:opening socket\n";
		exit(1);
 	}

 	bzero((char*)&serv_addr,sizeof(serv_addr));//bzero helps in clearing the values already present in serv_addr

 	serv_addr.sin_family=AF_INET;//IP address format
 	serv_addr.sin_addr.s_addr=INADDR_ANY;//get your address on your own ie this pc address itself
 	serv_addr.sin_port=htons(portno);
  
 	if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
 	{
 		cout<<"error on binding\n";
		exit(1);
 	}

	connect_client(2);
} 

void sock::connect_client(int no_of_clients)//connecting with clients
{
	 int no,port;
	 listen(sockfd,no_of_clients);//at a time this server can handle given no_of_clients
	 no=0;
 	for(int i=1;i<=no_of_clients;i++)
 	{
		clilen[i]=sizeof(cli_addr[i]);
		newsockfd[i]=accept(sockfd,(struct sockaddr*)&cli_addr[i],&clilen[i]);
 		if(newsockfd[i]<0)
 		{
	 	 cout<<"error:accepting client"<<i<<endl;
	 	 exit(1);
 		}
 		else
 		{
	 	 char *ip=inet_ntoa(cli_addr[i].sin_addr);//to get clients ip address it is saved in client address
	 	 int port=ntohs(cli_addr[i].sin_port);//to get port no with which client got connected to server
 	 	 cout<<"\nconnected successfully with client"<<i<<". IP of client:"<<ip<<". port:"<<port<<endl;
	 	 no++;

		 int temp=htonl(i);
		 write(newsockfd[i],&temp,sizeof(temp));
 		}
 	}
 	if(no==no_of_clients)
 		cout<<"\nall the clients are connected\n";
 	else
 	{
		cout<<"error:some clients are not connected\n";
		exit(1);
 	}
}

int sock::check(char buffer[1024])//checking the input and receiving the next input based on this
{
	int result;

	if(strncmp(buffer,"add",3)==0)
		result=1;
	else if(strncmp(buffer,"sub",3)==0)
		result=2;
	else if(strncmp(buffer,"mul",3)==0)
		result=3;
	else if (strncmp(buffer,"fac",3)==0)
		result=4;
	/*else if (strncmp(buffer,"bye",3)==0)
		result=5;*/
	else if (strncmp(buffer,"guess",5)==0)
		result=6;
	else 
		result=0;
	return result;
}


void sock::dualchat()//chat  between two clients
{
	int i=1;
	bool isexit;
	isexit=false;
 	
	while(newsockfd[1]>0 && newsockfd[2]>0)
 	{
		cout<<"enter # to end the connection"<<endl;
		
		do{
		   cout<<"\nclient1: ";
		   send(newsockfd[1],"typemsg\n",9,0);
		   send(newsockfd[2],"takemsg\n",9,0);
		   do{
		      recv(newsockfd[1],buffer,1024,0);//msg from client1
		      cout<<buffer<<" ";
		      send(newsockfd[2],buffer,1024,0);//msg sent to client2
		      if(*buffer=='#')
 		      {
			send(newsockfd[2],buffer,1024,0);
			*buffer='*';
			isexit=true;
		      }

		      int result=check(buffer);
		      if(result!=0)
		      {
			int temp,temp1;
			read(newsockfd[1],&temp,sizeof temp);//taking int value
			temp1=ntohl(temp);
			cout<<temp1<<" ";
			temp=htonl(temp1);
			write(newsockfd[2],&temp,sizeof temp);//sending int value
		      }
		   }while(*buffer!='*');
 

		   cout<<"\nclient2: ";
		   send(newsockfd[2],"typemsg\n",1024,0);
		   send(newsockfd[1],"takemsg\n",1024,0);
		   do{
		      recv(newsockfd[2],buffer,1024,0);//msg from client2
		      cout<<buffer<<" ";
		      send(newsockfd[1],buffer,1024,0);//msg sent to client1
		      if(*buffer=='#')
 		      {
			send(newsockfd[1],buffer,1024,0);
			*buffer='*';
			isexit=true;
		      }
		      int result=check(buffer);
		      
		      if(result!=0)
		      {
			int temp,temp1;
			read(newsockfd[2],&temp,sizeof temp);//taking int value
			temp1=ntohl(temp);
			cout<<temp1<<" ";
			temp=htonl(temp1);
			write(newsockfd[1],&temp,sizeof temp);//sending int value
		        if(result==6)
			{
			 *buffer='*';
			}
		      }
		   }while(*buffer!='*');
		   cout<<"\n";
		}while(!isexit );
	 	cout<<"\nconnection terminated..."<<endl;
		cout<<"\ngoodbye..."<<endl;
		isexit=false;
		exit(1);	
	}
	close(newsockfd[1]);
	close(newsockfd[2]);
}



int main(int argc,char *argv[])
{
 	int portno;
 	if(argc<2)//port number will be passed during run time
 	{
   		fprintf(stderr,"error: no port provided\n");
   		exit(1);
 	}

 	portno=atoi(argv[1]);
 
 	sock sock1(portno);
	sock1.dualchat();
	return 0;
}

