//Module 1 server
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
		int sockfd[10],client_fd[10],portno[10];
 		char buffer[1024];//this is used to take values from server and send values to the server
		struct sockaddr_in serv_addr[10],cli_addr[10];//to store address of server and client sockets
		int n[10];
 		socklen_t clilen[10];//length of address of sockets
		
	public:
		int stop;
		sock(int portno[]);//to create socket
		void connect_client(int no_of_clients);//connecting with clients
		void listening(int n);
		void change_port(int client,int op);
		void switchcase(int option,int client);
		void fibonacci(int client);
		void factorial(int client);
		void is_prime(int client);//to check a no is prime or not
		void is_perfect_sq(int client);//to check a no is perfect sq or not
		void random_gen(int client);
		void recv_client(int client);
		void chat(int client1,int client2);
		void file_transfer(int client);
		
};

sock::sock(int portno[])     //creating 8 sockets
{
	cout << "\nEnter no: of clients to be connected:";
	cin >> no_of_clients;
	
       for(int i=0;i<=7;i++)
       {
 	sockfd[i]=socket(AF_INET,SOCK_STREAM,0);//creating a socket

 	if(sockfd[i]<0)
 	{
		cout<<"\nerror:opening socket\n";
		exit(1);
 	}

 	bzero((char*)&serv_addr[i],sizeof(serv_addr[i]));//bzero helps in clearing the values already present in serv_addr

 	serv_addr[i].sin_family=AF_INET;//IP address format
 	serv_addr[i].sin_addr.s_addr=INADDR_ANY;//get your address on your own ie this pc address itself
 	serv_addr[i].sin_port=htons(portno[i]);
  
 	if(bind(sockfd[i],(struct sockaddr*)&serv_addr[i],sizeof(serv_addr[i]))<0)
 	{
 		cout<<"error on binding\n";
		exit(1);
 	}
       }
	
	connect_client(no_of_clients);
} 

void sock::connect_client(int no_of_clients)   //connecting all clients to base server
{
	 int no,port;
	 for(int i=0;i<=7;i++)
	 {
	  listen(sockfd[i],no_of_clients);//at a time this server can handle given no_of_clients
	 }
	 
	 no=0;

 	for(int i=1;i<=no_of_clients;i++)//creating base connection
 	{
		clilen[i]=sizeof(cli_addr[i]);
		client_fd[i]=accept(sockfd[0],(struct sockaddr*)&cli_addr[i],&clilen[i]);
 		if(client_fd[i]<0)
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
 		}
 	}
 	if(no==no_of_clients)
 		cout<<"\nall the clients are connected\n";
 	else
 	{
		cout<<"error:some clients are not connected\n";
		exit(1);
 	}
	listening(no_of_clients);
}

void sock::listening(int n)     //Receiving options from clients
{
	int temp,temp1,option[15],o,sum;
	do
	{
	cout<<"receiving client choices..."<<endl;
	for(int index=1;index<=n;index++)
	  {
	      recv(client_fd[index],&o,sizeof o,0);
	      option[index]=ntohl(o);
	  }
	sum=0;
	for(int index=1;index<=n;index++)
	  {
	      sum=sum+option[index];
	  }
	cout << "Sum:" << sum << endl;
	for(int index=1;index<=n;index++)
	  {
	      if(option[index]==8)
	        {
		   change_port(index,5);
		   cout << "cahnging" << endl;
		   change_port(index,9);
		   cout << "completed" << endl;
		   break;
		}
	      else
	      change_port(index,option[index]);
	  }
	
	if(sum==20)
	  {
	     for(int index=1;index<=n;index++)
	      close(client_fd[index]);
	     for(int index=1;index<=7;index++)
	      close(sockfd[index]);
	     
	      exit(1);
	      return;
	  }
	}while(sum!=20);
	
	      
}

void sock::change_port(int client,int op)   //changing port according to client request
{
	int temp,port,server; 
	int port1=7080;
	switch(op)
	 {
	   case 0:
		  {
			port=port1;
			server=0;
		  }
	 	  break;
	   case 1:
		  {
			port=port1+1;
			server=1;
		  }
		  break;
	   case 2:
		  {
			port=port1+2;
			server=2;
		  }
		  break;
	   case 3:
		  {
			port=port1+3;
			server=3;
		  }
		  break;
	   case 4:
		  {
			port=port1+4;
			server=4;
		  }
		 break;
	   case 5:
		  {
			port=port1+5;
			server=5;
		  }
		 break;
	   case 6:
		  {
			port=port1+6;
			server=6;
		  }
		 break;
	   case 7:
		  {
			port=port1+7;
			server=7;
		  }
		  break;
	   case 9:
		  {
			port=port1+6;
			server=6;
		  }
		 break;
	 }

	clilen[client]=sizeof(cli_addr[client]);	  //accepting client
	client_fd[client]=accept(sockfd[server],(struct sockaddr*)&cli_addr[client],&clilen[client]);
 	if(client_fd[client]<0)
 	{
	 cout<<"error:accepting client"<<client<<endl;
	 exit(1);
 	}
 	else
 	{
	  char *ip=inet_ntoa(cli_addr[client].sin_addr);//to get clients ip address it is saved in client address
	  int port=ntohs(cli_addr[client].sin_port);//to get port no with which client got connected to server
 	  cout<<"\nconnected successfully with client"<<client<<". IP of client:"<<ip<<". port:"<<port<<endl;
 	}
	
	switchcase(op,client);
	return;
}

void sock::switchcase(int option,int client)    //performing operations
{
	   switch(option)
	   {
		case 1:
			{
			  fibonacci(client);
			  change_port(client,0);
			}
			break;
		case 2:
			{
			  factorial(client);
			  change_port(client,0);
			}
			break;
		case 3:
			{
			  is_prime(client);
			  change_port(client,0);
			}
			break;
		case 4:
			{
			  is_perfect_sq(client);
			  change_port(client,0);
			}
			break;
		case 5:
			{
			  random_gen(client);
			  change_port(client,0);
			}
			break;
		case 7:
			{
			  file_transfer(client);
			  change_port(client,0);
			}
			break;
		case 9:
			{
			  cout << "case 9" << endl;
			  recv_client(client);
			  //change_port(client,0);
			}
			break;
		/*case 0: cout << "returned" << endl;
			return;
			//break;*/
	  }
}

void sock::fibonacci(int client)    //fibonacci generation
{
	cout << "Fibonacci series.." << endl;
	int buf[1024];
	int n,n1;
	int temp=recv(client_fd[client],&n,sizeof n ,0);
	
	n1=ntohl(n);	
	cout<<"input: "<< n1 <<endl;
	buf[0]=0;
	buf[1]=1;
	for(int i=2;i<n1;i++)
	{
		buf[i]=buf[i-1]+buf[i-2];
	}
	cout << "Output: "; 
	int k;
	for(int i=0;i<n1;i++)       //sending series to client
	{
		cout<< buf[i] << " ";
		k=htonl(buf[i]);
		temp=send(client_fd[client],&k,sizeof k,0);
	}
	cout << "\nFibonacci series generation completed" << endl;
}

void sock::factorial(int client)   //factorial
{
	cout << "Factorial..." << endl;
	int n,n1,i,j,fact=1,fact1,temp;
	temp=recv(client_fd[client],&n,sizeof n,0);
	n1=ntohs(n);
	i=1;
	cout<<"Input: "<< n1 <<endl;
	while(i<=n1)
	{
		fact=fact*i;	
		i++;
	}
	cout<<"Output: "<< fact <<endl;
	fact1=htons(fact);
	temp=send(client_fd[client],&fact1,sizeof fact1 ,0);
	cout << "Factorial completed" << endl;
}

void sock::is_prime(int client)//to check a no is prime or not
{
	cout << "Is_prime..." << endl;
 	int temp,input,flag=0,result;
	
	read(client_fd[client],&temp,sizeof(temp));//taking input from client1
	input=ntohl(temp);
	cout<<"\ngiven input:"<<input<<endl;
	
	for(int i=2;i<input;i++)//checking it is prime or not
	{
	 if(input%i==0)
	 {
		flag=1;
		break;
	 }
	}

	if(flag==0)
	{
	 result=1;
	 temp=htonl(result); 
	 write(client_fd[client],&temp,sizeof(temp));//sending  result to client
	 cout<<"\nthe given input is a prime number\n";
	}

	else if(flag==1)
	{
	 result=0;
	 temp=htonl(result); 
	 write(client_fd[client],&temp,sizeof(temp));//sending  result to client
	 cout<<"\nthe given input is not a prime number\n";
	}
	cout<<"\nIs prime completed\n";
}

void sock::is_perfect_sq(int client)//to check a no is perfect sq or not
{
	cout << "is_perfectsquare..."<< endl;
 	int temp,input,flag=0,result;
	
	read(client_fd[client],&temp,sizeof(temp));//taking input from client1
	input=ntohl(temp);
	cout<<"\ngiven input:"<<input<<endl;
	
	int i=1;
	while(input>0)//to check whether input is perfect square or not
	{
	 input=input-i;
	 i=i+2;
	}

	if(input==0)
	{
	 result=1;
	 temp=htonl(result); 
	 write(client_fd[client],&temp,sizeof(temp));//sending  result to client
	 cout<<"\nthe given input is a perfect square\n";
	}

	else 
	{
	 result=0;
	 temp=htonl(result); 
	 write(client_fd[client],&temp,sizeof(temp));//sending  result to client
	 cout<<"\nthe given input is not a perfect square\n";
	}
	cout << "is_perferctsquare completed" << endl;
	
}

void sock::random_gen(int client)
{
	cout << "Random number generation..." << endl;
	srand(time(NULL));
	int n=rand() % no_of_clients + 1;
	//n=2;
	if(n==client && n==no_of_clients)
	  n=n-1;
	else if(n==client && n==1)
	  n=n+1;
	else if(n==client)
	  n=n-1;

	cout << "Random number: " << n << endl;
	int o=htonl(n);
	send(client_fd[client],&o,sizeof o,0);
	cout << "Random generation completed" << endl;
}

void sock::recv_client(int client)   //receiving client to perform chat
{
	cout << "Enter.." << endl;
	int client1,client2;
	int temp,n;
	client1=client;
	read(client_fd[client1],&temp,sizeof(temp));
	int temp1=ntohl(temp);
	client2=temp1;
	client=temp1;
	cout << "no:" << no_of_clients << endl;
	for(int index=1;index<=no_of_clients;index++)
	  {
	    if(index!=client1)
	    {
	     if(index==temp1)
	      {
		 n=1;
	      }
	     else
	      {
		 n=0;
	      }
	      int o=htonl(n);
	         send(client_fd[index],&o,sizeof o,0);
	    }
	   cout << "Enter.." << index << endl;
	  }
	cout << "Enter.." << endl;
	sleep(3);
	int port,server; 
	int port1=7080;
	port=port1+6;
	server=6;
	cout << "Accepting client 2...." << endl;
	
	clilen[client]=sizeof(cli_addr[client]);
	client_fd[client]=accept(sockfd[server],(struct sockaddr*)&cli_addr[client],&clilen[client]);
 	if(client_fd[client]<0)
 	{
	 cout<<"error:accepting client"<<client<<endl;
	 exit(1);
 	}
 	else
 	{
	  char *ip=inet_ntoa(cli_addr[client].sin_addr);//to get clients ip address it is saved in client address
	  int port=ntohs(cli_addr[client].sin_port);//to get port no with which client got connected to server
 	  cout<<"\nconnected successfully with client"<<client<<". IP of client:"<<ip<<". port:"<<port<<endl;
 	}
	cout << "Calling chat fn" << endl;
	
	chat(client1,client2);
}
	

void sock::chat(int client1,int client2)   //performing chat...
{
	cout << "Enter chat" << endl;
	int client_temp;
	char buf[1000];
	int array[3],temp,temp1;
	array[1]=client1;
	array[2]=client2;
	int ch=1,op,b[5];
  	string st;
	cout << "client1:" << array[1] << endl;
	cout << "client2:" << array[2] << endl;
	FILE *fp=fopen("/home/user/chatm1.txt","a+");
  
  do
    {
	for(int i=1;i<=2;i++)
         {
           temp=recv(client_fd[array[i]],&op,sizeof op,0);
	   b[i]=ntohl(op);
           cout << "index" << i << ":" << b[i] << endl;
         }
	if(b[1]==1)
	 {
	    int temp1=recv(client_fd[array[1]],buf,sizeof buf,0);
	    if(temp1!=-1)
	     {
	       cout << "Client1: " << buf << endl;
	       char msg[1000]="Client 1:";           //writing messages into a file
	       fprintf(fp,"%s",msg);
	       fprintf(fp,"%s",buf);
		fprintf(fp,"\n");
	     }
	    else
	     {
	       cout << "Not received" << endl;
	     }
	    if(buf[0]=='#')
	     {
	       ch=0;
	       char m[10]="*#";
	       fprintf(fp,"%s",m);
	       temp=send(client_fd[array[2]],buf,strlen(buf)+1,0);
	       break;
	     }
	    
	    temp=send(client_fd[array[2]],buf,strlen(buf)+1,0);
	    if(temp==-1)
	     {
	        cout << "Message not sent to client2" << endl;
	     }
	  }
	else if(b[2]==1)
	  {
	     int temp1=recv(client_fd[array[2]],buf,sizeof buf,0);
	     if(temp1!=-1)
	     {
	       cout << "Client2: " << buf << endl;        //writing messages into a file
	       char msg[1000]="Client 2:";
	       fprintf(fp,"%s",msg);
	       fprintf(fp,"%s",buf);
	       fprintf(fp,"\n");
	     }
	    else
	     {
	       cout << "Not received" << endl;
	     }
	    if(buf[0]=='#')
	     {
	       ch=0;
	       char m[10]="*#";
	       fprintf(fp,"%s",m);
		temp=send(client_fd[array[1]],buf,strlen(buf)+1,0);
		cout << "exited" << endl;
	        break;
	     }
	   
	    temp=send(client_fd[array[1]],buf,strlen(buf)+1,0);
	    if(temp==-1)
	     {
	        cout << "Message not sent to client1" << endl;
	     }
	  }
	
    }while(ch!=0);
    
    fclose(fp);
    
    change_port(client1,0);
    change_port(client2,0);
    change_port(client1,7);    //changing ports to perform file transfer
    cout << "port 7" << endl;
    change_port(client2,7);
    return;
}

void sock::file_transfer(int client)
{
	cout << "File transfer..." << endl;
	char buff[255];
	    sleep(20);
	    FILE *fp=fopen("/home/user/chatm1.txt","r"); //reading chat file and sending to c1,c2
	    cout << "\nEnter.." << endl;
	    fseek(fp,0,SEEK_SET);
	    int x;
	    char tt;
	    while(1)
	      
  	       { 
		       fgets(buff,sizeof buff,fp);   //messages sending line by line
		   	sleep(1);
		       int temp=send(client_fd[client],buff,sizeof buff,0);
		       cout << "buf:" << buff << endl;
		       if(buff[0]=='*' && buff[1]=='#')   //*# denotes end of file
			{
			   cout << "ended" << endl;
			   break;
			}
  		}
	    fclose(fp);
		cout << "file sent successfully" << endl;
		
          return;
  
	 
}
       



		   
int main(int argc,char *argv[])
{
 	int portno[10],option;
	portno[0]=7080;
	portno[1]=portno[0]+1;
	portno[2]=portno[0]+2;
	portno[3]=portno[0]+3;
	portno[4]=portno[0]+4;
	portno[5]=portno[0]+5;
	portno[6]=portno[0]+6;
	portno[7]=portno[0]+7;

 	if(argc<1)//port number will be passed during run time
 	{
   		fprintf(stderr,"error: no port provided\n");
   		exit(1);
 	} 
 	sock sock1(portno);	
	return 0;
}

