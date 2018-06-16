//Module1 client
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
#include<sys/wait.h>

#define BUF_SIZE 1000

using namespace std;

class client
{
 private:
	int sockfd,portno,n;
 	struct sockaddr_in serv_addr;
 	struct hostent *server;
 	char buffer[256];//this is used to take values from server and send values to the server
	char *ser;
	int random_gen_output;

 public:
	client(int portno,char *ser);
	void change_port(int port,int op);
	void change_port1(int port,int op);
	void switchcase();
	void fibonacci();
	void factorial();
	void is_prime();//to check a no is prime or not
	void is_perfect_sq();//to check a no is perfect sq or not
	void random_gen();
	void send_client();
	void chat();
	void file_transfer();
	

};

client::client(int portno,char *ser1)  //connecting to base port initially
{
 	int option;

	ser=ser1;

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
	switchcase();
}

void client::change_port(int port,int op)  //change port
{
	close(sockfd);

	int sockfd=socket(AF_INET,SOCK_STREAM,0);//creating a socket

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
 	serv_addr.sin_port=htons(port);
 	
 	if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
 	{
 		cout<<"error connecting\n";
        	exit(1);
 	}
	
	cout<<"\nconnection established..."<<endl;
	
}


void client::switchcase()  //options
{
	int option;
	int port1=7080;
	do
	{
	   cout<<"enter choices \n1:fibonacci series\n2:factorial\n3:is_prime\n4:is_perfect_sq\n5:random_gen\n6:chat\n7:file_transer\n8:task2(random client and chat)\n9:Dummy\n10:exit"<<endl;
	   cin>>option;
	   int o=htonl(option);
	   send(sockfd,&o,sizeof o,0);
	   
	   switch(option)
	   {
		case 1:
			{
			  change_port(port1+1,1);
			  fibonacci();
			 change_port(port1,0);
			}
			break;
		case 2:
			{
			  change_port(port1+2,2);
			  factorial();
			  change_port(port1,0);
			}
			break;
		case 3:
			{
			  change_port(port1+3,3);
			  is_prime();
			  change_port(port1,0);
			}
			break;
		case 4:
			{
			  change_port(port1+4,4);
			  is_perfect_sq();
			  change_port(port1,0);
			}
			break;
		case 5:
			{
			  change_port(port1+5,5);
			  random_gen();
			  change_port(port1,0);
			}
			break;
		/*case 6:
			{
			  change_port(3600,6);
			  int c;
			  cout << "\nWhom do u want to chat(1-8):";
			  cin >> c;
			  int temp=htonl(c);
			  send(sockfd,&temp,sizeof temp,0);
			  chat();
			  change_port(7080,0);
			}
			break;
		case 7:
			{
			  change_port(3700,7);
			  file_transfer();
			  change_port(7080,0);
			}
			break;*/
		case 8:
			{
			  change_port(port1+5,5);
			  random_gen();
			  change_port(port1,0);
			  
			 change_port(port1+6,6);
			 send_client();
			 chat();
			 change_port(port1,0);
			 change_port(port1+7,7);
			 file_transfer();
			 change_port(port1,0);
			 cout << "base connected" << endl;
			}
			break;
		case 9:
		  	{
			   int o,choice;
		 	   cout << "recieving.." << endl;
			   recv(sockfd,&o,sizeof o,0);
			   choice=ntohl(o);
			   cout << "recv:" << choice << endl;
			   if(choice==1)
			     {
				change_port(port1+6,6);
			        chat();
				change_port(port1,0);
				change_port(port1+7,7);
				//cout << "Entering file fn" << endl;
				file_transfer();
			 	change_port(port1,0);
				cout << "base connected" << endl;
			     }
			   
			}
			break;
		case 10: close(sockfd);
		         break;
	  }
	}while(option!=10);
	
}

void client::fibonacci()  //Fibonacci generation
{
	int n,n1;
	cout<<"enter the value of n for fibonacci"<<endl;
	cin>>n1;
	int buf[1024];
	n=htonl(n1);
	int temp=send(sockfd,&n,sizeof n,0);
	int k;
	cout << "Fibonacci series: ";
	for(int i=0;i<n1;i++)          //series receiving from server
	{
	
	temp=recv(sockfd,&k,sizeof k,0);
	buf[i]=ntohl(k);
	cout <<buf [i]<< " ";
	}
	cout << "\nFibonacci series generation completed" << endl;
}

void client::factorial()	//Factorial
{
	int n,n1;
	int fact,fact1;
	cout<<"enter the value of n for factorial"<<endl;
	cin>>n;
	n1=htons(n);
	int temp=send(sockfd,&n1,sizeof n1,0);
	
	temp=recv(sockfd,&fact,sizeof fact,0);
	fact1=ntohs(fact);
	cout<<"the value of factorial is  "<< fact1<<endl;
	cout << "Factorial completed" << endl;
}

void client::is_prime()//to check a no is prime or not
{
	cout << "Is_prime().."<< endl;
	int original,temp,result;
	cout << "Enter value of n: ";
	cin >> original;
	cout<<"\nInput:"<<original;	

	temp=htonl(original); 
	write(sockfd,&temp,sizeof(temp));//sending input to server

	read(sockfd,&temp,sizeof(temp));//taking result from server
	result=ntohl(temp);

	cout<<"\nresult:"<<result<<endl;
	if(result==1)
	 cout<<"\nthe given input is a prime number\n";
	else if(result==0)
	 cout<<"\nthe given input is not a prime number\n";
	
	cout<<"\nIs prime completed\n";
}

void client::is_perfect_sq()//to check a no is perfect sq or not
{
 	cout << "Is_perfectsquare().."<< endl;
	int original,temp,result;
	cout << "Enter value of n: ";
	cin >> original;
	
	cout<<"\ninput:"<<original;	

	temp=htonl(original); 
	write(sockfd,&temp,sizeof(temp));//sending input to server

	read(sockfd,&temp,sizeof(temp));//taking result from server
	result=ntohl(temp);

	cout<<"\nresult:"<<result<<endl;
	if(result==1)
	 cout<<"\nthe given input is a perfect square\n";
	else if(result==0)
	 cout<<"\nthe given input is not a perfect square\n";

	cout<<"\ntask completed\n";
}

void client::random_gen()	//Receiving random number from server
{
	cout << "\nRandom number generation..." << endl;
	int temp,n;
	recv(sockfd,&temp,sizeof temp,0);
	n=ntohl(temp);
	cout << "Output: "<< n;
	cout << "\nTask completed" << endl;
	random_gen_output=n;
}

void client::send_client()     //send client
{
	int temp=htonl(random_gen_output);
	write(sockfd,&temp,sizeof temp);
	
}

void client::chat()     //chat...
{
  cout << "Enter chat:" << endl;
  string st;
  char buf[100];
  int ch=1,choice,temp;
  
  do
    {
	cout << "Do u want to chat(1/0):";
  	cin >> choice;
	int o=htonl(choice);
	send(sockfd,&o,sizeof o,0);
	if(choice==1)
	 {
	    cout << "Enter message to another client:" << endl;
	    getline(cin,st);
	    //cout << "ENter.." << endl;
	    getline(cin,st);
	    strcpy(buf,st.c_str());
	    int temp1=send(sockfd,buf,strlen(buf)+1,0);
	    if(temp1==-1)
	     {
	       cout << "Message not sent" << endl;
	     }
	    if(buf[0]=='#')
	     {
	        ch=0;
		return;
	     }
	    
	  }
	else if(choice==0)
	  {
	     temp=recv(sockfd,buf,sizeof buf,0);
	     cout << "Message from other client:" << buf << endl;
	     if(buf[0]=='#')
	     {
	        ch=0;
		return;
	     }
	     
 	  }
	
    }while(ch!=0);
}

void client::file_transfer()          //File_transfer
{
	int bytes_received=0,isend=0;
 		char buff[BUF_SIZE],file_name[1000];
	       cout << "\nEnter file destination:"<< endl;
	       cin >> file_name;
  		FILE *fp1=fopen(file_name,"a+");
		do
    		 {
		    sleep(1);
		    bytes_received = recv(sockfd, buff, sizeof buff,0);
        	    cout << "Bytes received " << bytes_received << endl;
		    fprintf(fp1,"%s",buff);
 		    cout << "buf:" << buff << endl;
		    if(buff[0]=='*' && buff[1]=='#')
		    {
		      isend=1;
		      break;
		     }
    		 }while(isend!=1);
		   if(bytes_received<0)
			cout << "Error" << endl;
		  else
		     cout << "File 1 received successfully\n";
   		  
		fclose(fp1);
	        return;
}


int main(int argc,char*argv[])
{
 	int portno,choice;
 	if(argc<2)//ip addr and port number will be passed during run time
 	{
   		fprintf(stderr,"usage %s hostname port\n",argv[0]);
   		exit(0);
 	}

 	
 	
	client client1(7080,argv[1]);
 return 0;
}
