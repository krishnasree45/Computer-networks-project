//guessing game between two clients via server
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
using namespace std;

class client
{
 private:
	int sockfd,portno,n;
 	struct sockaddr_in serv_addr;
 	struct hostent *server;
 	char buffer[256];//this is used to take values from server and send values to the server
	int secret;

 public:
	client(int portno,char *ser);
	void decide_client();
	int  first_client();
	void dualchat_client1(int secret);
	void dualchat_client2();
	int check(char buffer[1024]);
	int factorial(int num);	
};

client::client(int portno,char *ser)//connecting with socket
{
 	int option;

 	sockfd=socket(AF_INET,SOCK_STREAM,0);//creating a socket

 	if(sockfd<0)
 	{
		cout<<"error:opening socket\n";
		exit(1);
 	}
 
 	server=gethostbyname(ser);//servers ip address
 	if (server==NULL)
 	{
 		fprintf(stderr,"error no such host\n");
   		exit(0);
 	}

 	bzero((char*)&serv_addr,sizeof(serv_addr));//clear values in serv_addr

 	serv_addr.sin_family=AF_INET;
 	bcopy((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);//copying server address
 	serv_addr.sin_port=htons(portno);
 
 	if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
 	{
 		cout<<"error connecting\n";
        	exit(1);
 	}
	
	cout<<"\nconnection established..."<<endl;

	decide_client();
}

void client::decide_client()//assigning the client no as per its connection
{
	int temp,temp1;

	read(sockfd,&temp,sizeof(temp));
	temp1=ntohl(temp);
	
	if(temp1==1)
	{
	 int secret= first_client();
	 dualchat_client1(secret);
	 
	}

	else if(temp1==2)
	{
	 dualchat_client2();
	}
	
}

int client::first_client()//intial operations to be performed by first client
{
	int original,rem,q,sum=100;

	srand(time(NULL));
	original=rand()%4000+1;
	cout<<"original:"<<original<<endl;
	
	q=original;
	while(sum>=10)
	{
	 sum=0;
	 while(q!=0)
	 {
		rem=q%10;
		sum=sum+rem;
		q=q/10;
	 }
	 q=sum;
	}
	cout<<"sum1="<<sum<<endl;
	sum=sum*9;

	q=sum;
	while(sum>=10)
	{
	 sum=0;
	 while(q!=0)
	 {
		rem=q%10;
		sum=sum+rem;
		q=q/10;
	 }
	 q=sum;
	}
	cout<<"sum2="<<sum<<endl;
	secret=sum;
	return secret;
}

int client::check(char buffer[1024])//checking input and performing next operation based on this
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
	else if (strncmp(buffer,"bye",3)==0)
		result=5;
	else if (strncmp(buffer,"guess",5)==0)
		result=6;
	else 
		result=0;
	return result;
}

int client::factorial(int num)//performing factorial
{
 	int f=1,i;
	for(i=1;i<=num;i++)
	{
	 f=f*i;
	}
	return f;
}

void client::dualchat_client1(int secret)//chat block for client 1
{
	bool isexit;
	int choice;
	isexit=false;
	cout<<"enter # to end the connection and * to end message"<<endl;

	do{
	   recv(sockfd,buffer,1024,0);//recv instruction from server what to do
	   cout<<buffer<<endl;
	   if(strncmp(buffer,"typemsg",7)==0)
	   {
	   cout<<"\nclient:";
	   do{
	      cin>>buffer;
	      send(sockfd,buffer,1024,0);
	      if(*buffer=='#')
	      {
		send(sockfd,buffer,1024,0);
		*buffer='*';
		isexit=true;
	      }
	   }while(*buffer!='*');
	   }

	   else if(strncmp(buffer,"takemsg",7)==0)
	   {
	   cout<<"\nother client:";
	   
	   do{
	      recv(sockfd,buffer,1024,0);//receiving instruction from guessing client
	      cout<<buffer<<" ";
	      if(*buffer=='#')
 	      {
		*buffer='*';
		isexit=true;
	      }

	      int result=check(buffer);		
	      if(result!=0)		//performing operation
	      {
		int temp,temp1;
		read(sockfd,&temp,sizeof(temp));
		temp1=ntohl(temp);
		//cout<<"temp1:"<<temp1<<endl;
		//cout<<"secret:"<<secret<<endl;
		if(result==1)
		secret=secret+temp1;
		else if(result==2)
		secret=secret-temp1;
		else if(result==3)
		secret=secret*temp1;
		else if(result==4)
		{
		 secret=factorial(secret);
		}
		else if(result==6)
		{
		 if(secret==temp1)
		 {
		  cout<<temp1<<" ";
		  cout<<"\nwoww...your guess is correct\n";
		 }
		  /*cout<<"\ndo you want to play again(1/0):";
		  cin>>choice;
		  if(choice==1)
			first_client();*/
		  else
		  {
			cout<<"\nyour guess is wrong\n";
		  }
		  *buffer='*';
		  secret=first_client();
		 }
		
		cout<<"secret:"<<secret<<endl;
		
	      }	
	   }while(*buffer!='*');
	   }	  
	   cout<<endl; 
	}while(!isexit);
	cout<<"connection terminated"<<endl;
	cout<<"goodbye..."<<endl;
	//close(sockfd);
	return;
}

void client::dualchat_client2()//chat block for client 2
{
	bool isexit;
	isexit=false;
	int guess=9,send_guess=0;
	cout<<"enter # to end the connection"<<endl;

	do{
	   recv(sockfd,buffer,1024,0);
	   cout<<buffer<<endl;
	   if(strncmp(buffer,"typemsg",7)==0)//sending instruct to be performed by other client
	   {
	    int temp,temp1;
	    cout<<"\nclient:";
	   do{
	      if(send_guess==1)
	      {
		/*buffer[0]='g';
		buffer[1]='u';
		buffer[2]='e';
		buffer[3]='s';
		buffer[4]='s';
		buffer[5]='\0';
		cout<<buffer<<" ";*/
		send_guess=0;
		string st="guess";
		strcpy(buffer,st.c_str());
		cout<<buffer<<" ";
	     }
	     else
	     {
	      cin>>buffer;
	     }
	      send(sockfd,buffer,1024,0);
	      if(*buffer=='#')
	      {
		send(sockfd,buffer,1024,0);
		*buffer='*';
		isexit=true;
	      }
	
	      int result=check(buffer);	//checking input to perform operations on guess
              if(result!=0)
	      {
		if(result==6)
		{
		 temp1=guess;
		 temp=htonl(temp1);
		 write(sockfd,&temp,sizeof(temp));
		 cout<<guess<<" ";
		 bzero(buffer,1024);
		 *buffer='*';
		 guess=9;

		 /*buffer[0]=' ';
		 buffer[1]='*';
		 buffer[2]='\0';
		 string st="end *";
		 strcpy(buffer,st.c_str());
		 send(sockfd,buffer,1024,0);
		 cout<<buffer<<endl;
		 dualchat_client2();*/
		}
		else
		{
		 cin>>temp1; 
		 temp=htonl(temp1);
		 write(sockfd,&temp,sizeof(temp));//taking int value from user and perform appropriate operation
		 if(result==1)
		 guess=guess+temp1;
		 else if(result==2)
		 guess=guess-temp1;
		 else if(result==3)
		 guess=guess*temp1;
		 else if(result==4)
		 {
		  guess=factorial(guess);
		 }
		}
	      }	
		
	   }while(*buffer!='*');
	   }

	else if(strncmp(buffer,"takemsg",7)==0)
	   {
	   cout<<"\nother client:";
	   
	   do{
	      recv(sockfd,buffer,1024,0);
	      cout<<buffer<<" ";
	      if(*buffer=='#')
 	      {
		*buffer='*';
		isexit=true;
	      }
	      int result=check(buffer);
	      if(result!=0)
	      {
		if(result==5)//if other client wants to end the game then send him the guess value
			send_guess=1;
	      }
	   }while(*buffer!='*');
	   }	  
	   cout<<endl; 
	}while(!isexit);
	cout<<"connection terminated"<<endl;
	cout<<"goodbye..."<<endl;
	//close(sockfd);
	return;
}


int main(int argc,char*argv[])
{
 int portno,choice;
 if(argc<3)//ip addr and port number will be passed during run time
 {
   	fprintf(stderr,"usage %s hostname port\n",argv[0]);
   	exit(0);
 }

 portno=atoi(argv[2]);
 //choice=atoi(argv[3]);
 
 client client1(portno,argv[1]);
 //client1.dualchat();

 return 0;
}
