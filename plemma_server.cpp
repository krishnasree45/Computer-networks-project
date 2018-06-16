//pumping_lemma server
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

class sock
 {
	private :
		int sockfd;
		 struct sockaddr_in my_addr;
		 struct sockaddr_in client_addr;
		 int client_fd[2];
		 
	public:
		void reg_lang();
		sock(int port_no);

 };

void sock::reg_lang()
{
	int value,value1;
  	do{
  	int cli1,cli2;
	  int ch,ch1;
	  int ch_2,ch1_2;
	  recv(client_fd[1],&ch1,sizeof ch1,0);//from client 1 receiving 
	  ch=ntohl(ch1);
	  cout<<"ch value "<<ch<<endl;
	   recv(client_fd[2],&ch1_2,sizeof ch1_2,0);//from client 2 receiving 
	  ch_2=ntohl(ch1_2);
	   cout<<"ch_2 value "<<ch_2<<endl;
	  int r,r1;
	  recv(client_fd[1],&r1,sizeof r1,0);//to choose which reg lang  from client 1
	  r=ntohl(r1);
	  int r2,r1_2;
	  recv(client_fd[2],&r1_2,sizeof r1_2,0);//to choose which reg lang  from client 2
	  r2=ntohl(r1_2);
	  if(ch==1)
	  {
	    cli1=1;
	    cli2=2;
	  }
	  else
	  {
		cli1=2;
		cli2=1;
	  }
  if(r==1)//a^nb^n
   {
	
  	int n;
	cout<<"started"<<endl;
	recv(client_fd[cli1],&n,sizeof n,0);//receiving n value 
	cout<<"recvd"<<endl;
	int n1;
	n1=ntohl(n);	
	cout<<"n value in server"<<n1<<endl;
	send(client_fd[cli2],&n,sizeof n,0);//sending n value 
  
	char str[100];
	for(int i=0;i<=2*n1-1;i++){
	recv(client_fd[cli2],&str[i],sizeof str[i],0);}//receiving msg 
	cout<<"msg value \n";
	for(int i=0;i<=2*n1-1;i++)
	 cout<<str[i]<< " ";
	for(int i=0;i<=2*n1-1;i++){
	send(client_fd[cli1],&str[i],sizeof str[i],0);}//sending msg 
	
	char u[100],v[100],w[100];
	recv(client_fd[cli1],u,sizeof u,0);//receving u value 
	cout<<"value of u in server:"<<u<<endl;
	send(client_fd[cli2],u,sizeof u,0);//sending u value 
	
	
	recv(client_fd[cli1],v,sizeof v,0);//receving v value 
	cout<<"value of v in server:"<<v<<endl;
	send(client_fd[cli2], v,sizeof v,0);//sending  v value 
	

	recv(client_fd[cli1],w,sizeof w,0);//receving w value
	cout<<"value of w in server:"<<w<<endl;
	send(client_fd[cli2],w,sizeof w,0);//sending  w value 
	
	int index,index1;
	recv(client_fd[cli2],&index,sizeof index,0);//receiving i value
	send(client_fd[cli1],&index,sizeof index,0);//sending i value
	index1=ntohl(index);
	
	recv(client_fd[cli2],u,sizeof u,0);//receiving uv^iw value 
		for(int i=1;i<=index1;i++)
		{
			
			 recv(client_fd[cli2],v,sizeof v,0);
		}	
		recv(client_fd[cli2],w,sizeof w,0);
	
	send(client_fd[cli1],u,sizeof u,0);//sending uv^iw msg
		for(int i=1;i<=index1;i++)
		{
			
			 send(client_fd[cli1],v,sizeof v,0);
		}	
		send(client_fd[cli1],w,sizeof w,0);
	
		
		recv(client_fd[cli1],&value1,sizeof value1,0);
		value=ntohl(value1);
  }
  if(r==2)//a^p lang 
  {
	
	int p,p1;
	cout<<"started"<<endl;
	recv(client_fd[cli1],&p1,sizeof p1,0);//receiving p value 
	cout<<"recvd"<<endl;
	
	p=ntohl(p1);	
	cout<<"p value in server"<<p<<endl;
	send(client_fd[cli2],&p1,sizeof p1,0);
  
	char str[100];
	for(int i=0;i<=p-1;i++){
	recv(client_fd[cli2],&str[i],sizeof str[i],0);}//receiving msg 
	cout<<"msg value \n";
	for(int i=0;i<=p-1;i++)
	 cout<<str[i]<< " ";
	for(int i=0;i<=p-1;i++){
	send(client_fd[cli1],&str[i],sizeof str[i],0);}//sending msg 
	
	char u[100],v[100],w[100];
	recv(client_fd[cli1],u,sizeof u,0);//u vlaue 
	cout<<"value of u in server:"<<u<<endl;
	send(client_fd[cli2],u,sizeof u,0);
	
	
	recv(client_fd[cli1],v,sizeof v,0);//v value 
	cout<<"value of v in server:"<<v<<endl;
	send(client_fd[cli2], v,sizeof v,0);
	

	recv(client_fd[cli1],w,sizeof w,0);//w value 
	cout<<"value of w in server:"<<w<<endl;
	send(client_fd[cli2],w,sizeof w,0);
	
	int index,index1;
	recv(client_fd[cli2],&index,sizeof index,0);//index value 
	send(client_fd[cli1],&index,sizeof index,0);
	index1=ntohl(index);
	
	recv(client_fd[cli2],u,sizeof u,0);//uv^iw vlaue 
	send(client_fd[cli1],u,sizeof u,0);
		for(int i=1;i<=index1;i++)
		{
			
			 recv(client_fd[cli2],v,sizeof v,0);
			send(client_fd[cli1],v,sizeof v,0);
		}	
		recv(client_fd[cli2],w,sizeof w,0);
		send(client_fd[cli1],w,sizeof w,0);
	
		
		
		recv(client_fd[cli2],&value1,sizeof value1,0);
		value=ntohl(value1);
  }	
  }while(value!=0);
}

sock::sock(int port_no)
{
	sockfd=socket(PF_INET,SOCK_STREAM,0);  //Creating the socket

	  my_addr.sin_family=AF_INET;
	  my_addr.sin_port=htons(port_no);
	  my_addr.sin_addr.s_addr=INADDR_ANY;
	  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	  int temp=bind(sockfd, (struct sockaddr *)&my_addr, (sizeof my_addr)); //Binding the socket to the address

	  if(temp==0)
	    cout << "Server started running at "<< inet_ntoa(my_addr.sin_addr)  << ":" << port_no<< endl;
	  else
	    {
	      cout << "Failed to bind socket" << endl;
	     // return -1;
		exit(1);
	    }

	  listen(sockfd,10);//listening
	  
	 
	  socklen_t client_size=sizeof client_addr;
	  
	  client_fd[1]	=accept(sockfd,(struct sockaddr*)&client_addr, &client_size);//accepting client1 
	  client_fd[2]=accept(sockfd,(struct sockaddr*)&client_addr, &client_size);//accepting client 2

	  cout << "Accepted incoming connection of two clients" << endl;
	  
  
}

int main(int argc,char* argv[])
{
  if(argc !=2)
    {
      cerr << "Usage syntax: ./server <port_no>" << endl;
      return 0;
    }
    int port_no=atoi(argv[1]);
    sock sock1(port_no);
    
    sock1.reg_lang();
 
  
  return 0;
}
