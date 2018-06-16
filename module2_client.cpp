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
	

 public:
	client(int portno,char *ser);
	void lrc();
	void vrc();
	int * four_bit_addition(int a[5],int b[5]);
	int full_adder_sum(int a,int b,int c);
	int full_adder_carry(int a,int b,int c);
	void chk_sum();
	void crc8();
	void crc12();
	int max();
	int err_lrc,err_vrc,err_crc_8,err_crc_12,err_chksum;
};

client::client(int portno,char *ser)
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
}

void client:: lrc()
{
	int i,temp,lrc_msg[1000],parity[100];
	int n,n1;
	temp=recv(sockfd,&n1,sizeof n1,0);
	n=ntohs(n1);
	int k,k1;
	for(i=1;i<=n+4;i++)
	{
		temp=recv(sockfd,&k1,sizeof k1,0);
		k=ntohs(k1);
		lrc_msg[i]=k;
	}
	
	for(int index=4;index>=1;index--)
	  {
	     int sum=0;
	     for(int index1=index;index1<=index+((n/4-1)*4);index1=index1+4)
	       {
		  sum=sum+lrc_msg[index1];
		  
	       }
	     if(sum%2==0)
	       parity[index]=0;
	     else
	       parity[index]=1;
	  }
	
        int count_lrc=0;
	for(i=1;i<=4;i++)
	{
		if(parity[i]!=lrc_msg[n+i])
		   count_lrc++;
	}
	
        if(count_lrc!=0)
		{
			cout <<"error in lrc " <<endl;
			err_lrc++;
		}
	else 	
		cout<<"no error in lrc "<<endl;
 
	
}

void client::vrc()
{
	int n1,vrc_msg[1000],i=1;
	int msg_recv[1000];
	recv(sockfd,&n1,sizeof n1,0);	
	n=ntohs(n1);
	int k,k1;
	for(int index=1;index<=n+n/4;index++)
	{
		recv(sockfd,&k1,sizeof k1,0);
		k=ntohs(k1);
		vrc_msg[index]=k;
	}
	
	int sum;
	
	for(int index=1;index<=n+n/4;index=index+4)
	  {
	    sum=0;
	    int index1;
	     for(index1=index;index1<=index+3;index1++)
	       {
	          msg_recv[index1]=vrc_msg[index1];
		  sum=sum+vrc_msg[index1];
		  
		 
	       }
		index++;	
	     if(sum%2==0){
	       msg_recv[index1]=0;
	      }
	     else
		{
	       msg_recv[index1]=1;
		
		}
	     
	  }
	
	int count_vrc=0;
	for( i=5;i<=n+n/4;i=i+5)
	{
		
		if(msg_recv[i]!=vrc_msg[i]){
		
		 count_vrc++;}
		
	}
	
	
	if(count_vrc!=0)
	 {
	 cout <<"error  in vrc "<<endl;
	 err_vrc++;
	 }
	else
	 cout<<"error not detected in vrc"<<endl;
}


int client::full_adder_sum(int a,int b,int c)
  {
	return a ^ b ^ c;
  }

int client::full_adder_carry(int a,int b,int c)
  {
	return (a & b) | (b&c) | (c&a);
  }

int * client:: four_bit_addition(int a[5],int b[5])
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

void client::chk_sum()
{
	int t,a[5],b[5],t1;
	int *ans;
	recv(sockfd,&t,sizeof t,0);
	n=ntohl(t);
	
	a[1]=0;
	a[2]=0;
	a[3]=0;
	a[4]=0;
	int msg_to_be_transmitted[1000],i;
	
	for(int index=1;index<=n+4;index++)	
	{
		recv(sockfd,&t1,1,0);
		msg_to_be_transmitted[index]=t1;
	
	}
	
	for(int index=1;index<=n+4;index=index+4)
	  {
	     i=1;
	     for(int index1=index;index1<=index+3;index1++)
		{
		    b[i]=msg_to_be_transmitted[index1];
		    i++;
		}
	     ans=four_bit_addition(a,b);
	     for(int index1=1;index1<=4;index1++)
	        a[index1]=*(ans+index1);
	  }
	int sum=0;
	
	for(int index=1;index<=4;index++)
	{
	 
	 sum=sum+(*(ans+index));
	}
	cout << endl;
	
         if(sum==4)
	 	 cout <<"Error not detected in checksum" << endl;	 
	else
	 {
	  cout << "Error detected in checksum" << endl;
	  err_chksum++;
	 }
}

void client::crc8()
{
	 int r[1000],divisor[1000],add=0,k=0,errmsg[1000],i,t,t1;
	int n1;
	recv( sockfd,&n1,sizeof n1,0 );
	n=ntohs(n1);
	for(i=1;i<=n+8;i++)
	{
		
		recv(sockfd,&t,sizeof t,0);
		t1=ntohs(t);
		errmsg[i]=t1;
	}
	
	do
	{
	add=0;
		for(int i=1;i<=n+8;i++)
		divisor[i]=0;

		for(int i=1;i<=n+8;i++)
		{
		if(errmsg[i]==1)
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
		if(divisor[i]==errmsg[i])
		r[i]=0;
		else
		r[i]=1;
		}
		

		for(int i=1;i<=n+8;i++)
		{
		errmsg[i]=r[i];
		
		}
		for(int i=1;i<=n;i++)
		{
		if(r[i]==0)
		{
		add=add+1;
		}
		}

	}while(add!=n);

	
	int w=0;
	for(int i=1;i<=n+8;i++)
	{
		
		if(r[i]!=0)
			w++;
	}
        cout << endl;
	if(w==0)
		cout <<"no error in crc 8"<<endl;
	else
	 {
		cout<<"error in crc 8"<<endl;
		err_crc_8++;
	 }
	



}

void client::crc12()
{
	 int r[1000],divisor[1000],add=0,k=0,errmsg[1000],i,t,t1;
	int n1;
	recv( sockfd,&n1,sizeof n1,0 );
	n=ntohs(n1);
	for(i=1;i<=n+12;i++)
	{
		
		recv(sockfd,&t,sizeof t,0);
		t1=ntohs(t);
		errmsg[i]=t1;
	}
	
	do
	{
	add=0;
		for(int i=1;i<=n+12;i++)
		divisor[i]=0;

		for(int i=1;i<=n+12;i++)
		{
		if(errmsg[i]==1)
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
		if(divisor[i]==errmsg[i])
		r[i]=0;
		else
		r[i]=1;
		}
		

		for(int i=1;i<=n+12;i++)
		{
		errmsg[i]=r[i];
		
		}
		for(int i=1;i<=n;i++)
		{
		if(r[i]==0)
		{
		add=add+1;
		}
		}

	}while(add!=n);

	
	int w=0;
	for(int i=1;i<=n+12;i++)
	{
		
		if(r[i]!=0)
			w++;
	}
        cout << endl;
	if(w==0)
		cout <<"no error in crc 12"<<endl;
	else
	 {
		cout<<"error in crc 12"<<endl;
		err_crc_12++;
	 }
	



}


int client::max()      // calculates number of times error is detected in techniques
{
	int maximum;
	cout<<"max errors in lrc" << err_lrc <<endl;
	cout<<"max errors in vrc" << err_vrc <<endl;
	if(err_lrc>=err_vrc)
	{
		maximum=err_lrc;
		
	}
	
	else
	{	
		
		maximum=err_vrc;
	}
	cout<<"max errors in checksum" << err_chksum<<endl;
	if(maximum<=err_chksum)
	{
	 
	 maximum=err_chksum;
	}
	 cout<<"max errors in crc12 " << err_crc_12 <<endl;
	if(maximum<=err_crc_12)
	{
	
	 maximum=err_crc_12;
	}
	 cout<<"max errors in crc 8 " << err_crc_8 <<endl;
	if(maximum<=err_crc_8)
	{
		maximum=err_crc_8;
	}
	
	return maximum;
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
 
 int count=0;
 client client1(portno,argv[1]);
 client1.err_lrc=0;
 client1.err_vrc=0;
 client1.err_crc_8=0;
 client1.err_crc_12=0;
 client1.err_chksum=0;
 for(int i=1;i<=100;i++)
 {
	 client1.lrc();
	client1.vrc();

	 client1.crc8();
	 client1.crc12();
	 client1.chk_sum();
	

 }


int perf=client1.max();
	
	if(perf==client1.err_lrc)
		printf("lrc performs better\n");
	if(perf==client1.err_vrc)
		printf("vrc performs better\n");
	if(perf==client1.err_crc_8)
		printf("crc 8 performs better\n");
	if(perf==client1.err_crc_12)
		printf("crc 12 performs better\n");
	if(perf==client1.err_chksum)
		printf("chk_sum performs better\n");
 client1.err_lrc=0;
 client1.err_vrc=0;
 client1.err_crc_8=0;
 client1.err_crc_12=0;
 client1.err_chksum=0;
for(int i=1;i<=100;i++)
 {
	 client1.lrc();
	 client1.vrc();

	 client1.crc8();
	 client1.crc12();
	 client1.chk_sum();
	
 }
 
   perf=client1.max();
	
	if(perf==client1.err_lrc)
		printf("lrc performs better\n");
	if(perf==client1.err_vrc)
		printf("vrc performs better\n");
	if(perf==client1.err_crc_8)
		printf("crc 8 performs better\n");
	if(perf==client1.err_crc_12)
		printf("crc 12 performs better\n");
	if(perf==client1.err_chksum)
		printf("chk_sum performs better\n");	
  

}
