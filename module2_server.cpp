//uni chat between server and client

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include <ctype.h>
using namespace std;

class sock
{
 	private:
		int no_of_clients,arr[1000],count;	
		int sockfd,client_fd[10],portno;
 		char buffer[1024];//this is used to take values from server and send values to the server
		struct sockaddr_in serv_addr,cli_addr[10];//to store address of server and client sockets
		int n,err_bits;
 		socklen_t clilen[10];//length of address of sockets

	public:
		sock(int portno);//to create socket
		void connect_client(int no_of_clients);//to connect with clients
		void random_gen(); // to generate random numbers
		void error(int arr[1000],int size); // to currupt required number of bits
		void initialization();
		void lrc_gen(int lrc_arr[1000],int n);     //to perform lrc 
		void vrc_gen(int vrc_arr[1000],int n);     // to perform vrc
		void checksum_gen(int chk_arr[1000],int n); // to perform chechsum
		void crc8_gen(int crc8_arr[1000],int n);    // to perform crc8
		void crc12_gen(int crc12_arr[1000],int n);
		int * four_bit_addition(int a[5],int b[5]);
		int full_adder_sum(int a,int b,int c);
		int full_adder_carry(int a,int b,int c);
};


int sock::full_adder_sum(int a,int b,int c)
  {
	return a ^ b ^ c;
  }

int sock::full_adder_carry(int a,int b,int c)
  {
	return (a & b) | (b&c) | (c&a);
  }



sock::sock(int portno)
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

	connect_client(1);
} 

void sock::connect_client(int no_of_clients)  // to connect to client 
{
	 int no,port;
	 listen(sockfd,no_of_clients);//at a time this server can handle given no_of_clients
	 no=0;
 	
            for(int i=1;i<=no_of_clients;i++)
 	{
		clilen[i]=sizeof(cli_addr[i]);
		client_fd[i]=accept(sockfd,(struct sockaddr*)&cli_addr[i],&clilen[i]);
 		
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
}

void sock::initialization()
{

	cout<<"enter the size "<<endl;
	cin>>n;
	cout<<"enter #of bits to corrupt"<<endl;
	cin>>err_bits;
	
}

void sock::random_gen()     //  generates  random numbers
{
	int i;
	int sec_arr[1000];
	srand(time(NULL));
	
	count=0;
	for(int loop_1=1;loop_1<=100;loop_1++)
	{
		
		for(int loop_index=1;loop_index<=n;loop_index++)
		{
			srand(time(NULL));
			arr[loop_index]=rand()%2;
			//cout<< arr[i] <<" ";
		}
		
		lrc_gen(arr,n);
		vrc_gen(arr,n);
		crc8_gen(arr,n);
		crc12_gen(arr,n);
		checksum_gen(arr,n);
				
	}
	
	initialization();     
	
	
	for(i=1;i<=100;i++)
	{
		for(int j=1;j<=n;j++)
		{
		sec_arr[j]=rand()%2;
		}
		lrc_gen(sec_arr,n);
		vrc_gen(sec_arr,n);
		crc8_gen(sec_arr,n);
		crc12_gen(sec_arr,n);
		checksum_gen(sec_arr,n);
	}
	
}


void sock:: error(int arr[1000],int size)
{
	int err_pos[1000],i;
	
	for(i=1;i<=err_bits;i++)
	{
		err_pos[i]=(rand()%size)+1;
	}
	for(i=1;i<=size;i++)
	{
		for(int j=1;j<=err_bits;j++)
		{
			if(err_pos[j]==i)
			{
				if(arr[i]==1)
					arr[i]=0;
				else
					arr[i]=1;
				
			}
		}
	}	
	
	
}

void sock::lrc_gen(int lrc_arr[1000],int n)        // performs lrc
{
	int sum;
	int parity[10];
	int n1=htons(n);
	int temp=send(client_fd[1],&n1,sizeof n1,0);     // sending array size to client
	for(int index=4;index>=1;index--)
	  {
	     sum=0;
	     for(int index1=index;index1<=index+((n/4-1)*4);index1=index1+4)
	       {
		  sum=sum+lrc_arr[index1];
		  
	       }
	     if(sum%2==0)
	       parity[index]=0;
	     else
	       parity[index]=1;
	  }
	
       
	for(int index=1;index<=4;index++)
	   lrc_arr[n+index]=parity[index];
	 
	
        int k,k1;
	error(lrc_arr,n+4);       // corrupts generated bits
	
   for(int index=1;index<=n+4;index++)
	{
		 k=lrc_arr[index];
		 k1=htons(k);
		 send(client_fd[1],&k1,sizeof k1,0); // sends corrupted array to client
	}

	
}

void sock ::vrc_gen(int vrc_arr[1000],int n) // performs vrc
{
	int msg_to_transmit[1000];
	int n1=htons(n);
	send(client_fd[1],&n1,sizeof n1,0);    // sends array size to client
	
        int i=1;
	for(int index=1;index<=n;index=index+4)
	  {
	     int sum=0;
	     for(int index1=index;index1<=index+3;index1++)
	       {
	          msg_to_transmit[i]=vrc_arr[index1];
		  sum=sum+vrc_arr[index1];
		  i++;
	       }
	     if(sum%2==0)
	       msg_to_transmit[i]=0;
	     else
	       msg_to_transmit[i]=1;
	     i++;
	  }
	
	
        int p=n+n/4,k;
	error(msg_to_transmit,p);   // corrupts generated bits
	
        
	
         for(int index=1;index<=p;index++)
	{
		k=htons(msg_to_transmit[index]);
		send(client_fd[1],&k,sizeof k,0);   // sending corrupted array to client
	}
}

int *sock:: four_bit_addition(int a[5],int b[5])    // performs 4-bit addition 
  {
	int c[5],temp[5];
	static int result[5];
	 
	result[4]=full_adder_sum(a[4],b[4],0);
	c[1]=full_adder_carry(a[4],b[4],0);
	result[3]=full_adder_sum(a[3],b[3],c[1]);
	c[2]=full_adder_carry(a[3],b[3],c[1]);
	result[2]=full_adder_sum(a[2],b[2],c[2]);
	c[3]=full_adder_carry(a[2],b[2],c[2]);
	result[1]=full_adder_sum(a[1],b[1],c[3]);
	c[4]=full_adder_carry(a[1],b[1],c[3]);
	
         if(c[4]==1)
	 {
	   b[1]=0;
	   b[2]=0;
	   b[3]=0;
	   b[4]=1;
	 
           for(int index=1;index<=4;index++)
	   a[index]=result[index];
	  
        result[4]=full_adder_sum(a[4],b[4],0);
	c[1]=full_adder_carry(a[4],b[4],0);
	result[3]=full_adder_sum(a[3],b[3],c[1]);
	c[2]=full_adder_carry(a[3],b[3],c[1]);
	result[2]=full_adder_sum(a[2],b[2],c[2]);
	c[3]=full_adder_carry(a[2],b[2],c[2]);
	result[1]=full_adder_sum(a[1],b[1],c[3]);
	c[4]=full_adder_carry(a[1],b[1],c[3]);
	
	return result;
	}
	 
        else
	 return result;
	
  }

void sock::checksum_gen(int chk_arr[1000],int n)    // performs checksum
{
	
	int array[50],i,a[5],b[5],msg_to_be_transmitted[1000];
	int *ans;
	int t;
	t=htonl(n);
	send(client_fd[1],&t,sizeof t,0);   // sends array size to client
 
	a[1]=0;
	a[2]=0;
	a[3]=0;
	a[4]=0;
	
        for(int index=1;index<=n;index=index+4)
	  {
	     i=1;
	    
	     for(int index1=index;index1<=index+3;index1++)
		{
		    b[i]=chk_arr[index1];
		    i++;
		}
	     
	     ans=four_bit_addition(a,b);
	     for(int index1=1;index1<=4;index1++)
	        a[index1]=*(ans+index1);
	  }
	
	for(int index=1;index<=n;index++)
	  {
	      msg_to_be_transmitted[index]=chk_arr[index];
	     
	  }
	for(int index=1;index<=4;index++)
	  {
	      if(*(ans+index)==1)
	        msg_to_be_transmitted[n+index]=0;
	      else
		 msg_to_be_transmitted[n+index]=1;
	     
	  }
	
	error(msg_to_be_transmitted,n+4);     // corrupts generated bits
	
	
	int p,k;
	
	for(int index=1;index<=n+4;index++)
	{
		p=msg_to_be_transmitted[index];
		
		send(client_fd[1],&p,1,0);       // sends corrupted array to client
	}
	

}

void sock:: crc8_gen(int dividend[1000],int n)         // performs crc 8
{
	int a[30],errmsg[30],k=0,add=0,r[30],divisor[30],divid[1000];
	int n1;
	n1=htons(n);
	send(client_fd[1],&n1,sizeof n1,0);//sending the value of n
	
	
	
	
	for(int i=n;i<=n+8;i++)           // appending with 8 zeroes
	 dividend[i]=0;
	
	for(int i=1;i<=n+8;i++)
	divid[i]=dividend[i];
	
	do
	{
	add=0;                           // divides with divisor polynomial
		for(int i=1;i<=n+8;i++)
		divisor[i]=0;

		for(int i=1;i<=n+8;i++)
		{
		if(divid[i]==1)
		{
		k=i;
		divisor[k]=1;
		divisor[k+1]=0;
		divisor[k+2]=0;
		divisor[k+3]=0;
		divisor[k+4]=0;
		divisor[k+5]=0;
		divisor[k+6]=1;
		divisor[k+7]=1;
		divisor[k+8]=1;
		break;
		}
		}

		for(int i=1;i<=n+8;i++)
		{
		if(divisor[i]==divid[i])
		r[i]=0;
		else
		r[i]=1;
		}
		

		for(int i=1;i<=n+8;i++)
		{
		divid[i]=r[i];
		
		}
		for(int i=1;i<=n;i++)
		{
		if(r[i]==0)
		{
		add=add+1;
		}
		}

	}while(add!=n);
	

	for(int i=1;i<=n+8;i++)
	{
	if(dividend[i]==r[i])
	dividend[i]=0;
	else
	dividend[i]=1;
	}
	

	error(dividend,n+8);



	int x;
	for(int i=1;i<=n+8;i++)
	{
	x=htons(dividend[i]);
	send(client_fd[1],&x,sizeof x,0);
	}
  
}


void sock:: crc12_gen(int dividend[1000],int n)         // performs crc 12
{
	int a[30],errmsg[30],k=0,add=0,r[30],divisor[30],divid[1000];
	int n1;
	n1=htons(n);
	send(client_fd[1],&n1,sizeof n1,0);//sending the value of n
	
	
	
	
	for(int i=n;i<=n+12;i++)       // appending with 12 zeroes
	 dividend[i]=0;
	
	for(int i=1;i<=n+12;i++)
	divid[i]=dividend[i];
	
	do
	{                                   // dividing with divisor polynomial
	add=0;
		for(int i=1;i<=n+12;i++)
		divisor[i]=0;

		for(int i=1;i<=n+12;i++)
		{
		if(divid[i]==1)
		{
		k=i;
		divisor[k]=1;
		divisor[k+1]=1;
		divisor[k+2]=0;
		divisor[k+3]=0;
		divisor[k+4]=0;
		divisor[k+5]=0;
		divisor[k+6]=0;
		divisor[k+7]=0;
		divisor[k+8]=0;
		divisor[k+9]=1;
		divisor[k+10]=1;
		divisor[k+11]=1;
		break;
		}
		}

		for(int i=1;i<=n+12;i++)
		{
		if(divisor[i]==divid[i])
		r[i]=0;
		else
		r[i]=1;
		}
		

		for(int i=1;i<=n+12;i++)
		{
		divid[i]=r[i];
		
		}
		for(int i=1;i<=n;i++)//n+7-2
		{
		if(r[i]==0)
		{
		add=add+1;
		}
		}

	}while(add!=n);
	

	for(int i=1;i<=n+12;i++)       
	{
	if(dividend[i]==r[i])
	dividend[i]=0;
	else
	dividend[i]=1;
	}
	

	error(dividend,n+12);



	int x;
	for(int i=1;i<=n+12;i++)
	{
	x=htons(dividend[i]);
	send(client_fd[1],&x,sizeof x,0);     // sending corrupted array to client
	}
  
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
	
	sock1.initialization();
	
	sock1.random_gen();
	
	return 0;
}
