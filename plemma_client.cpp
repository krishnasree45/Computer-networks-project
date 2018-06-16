//pumping_lemma client 
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
class client 
{
	private :
		int sockfd;
  		struct sockaddr_in my_addr;
		
	public:
		client(int port_no,char *ser);
		void reg_lang();
};

client ::client(int port_no,char *ser)//constructor for connecting 
{
	sockfd=socket(PF_INET,SOCK_STREAM,0);

  my_addr.sin_family=AF_INET;
  my_addr.sin_port=htons(port_no);
  my_addr.sin_addr.s_addr=inet_addr(ser);
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  int temp=connect(sockfd,(struct sockaddr*)&my_addr, sizeof my_addr);
  
  if(temp==0)
    cout << "Connection established with server" << endl;
  else
    {
      cout << "Could not establish connection!" << endl;
     // return -1;
      exit(1);
    }
}

void client::reg_lang()//determining a language is non reg using pumping lemma
{
	int value=0;
  do{
  int ch,ch1;
  cout<<"do u wish to claim(1/0)"<<endl;
  cin>>ch;
  ch1=htonl(ch);
  send(sockfd,&ch1,sizeof ch1,0);
  int r,r1;
  cout<<"which lang(a^nb^n)/a^p u wish to choose(1/2) "<<endl;//to choose a^nb^n or a^p
	cin>>r;
  r1=htonl(r);
	send(sockfd,&r1,sizeof r1,0);//sending this lang to server
  if(ch==1)//client1
  {
	
	
	if(r==1)
	{
		int n,n1;
		cout<<"enter the value of n"<<endl;
		cin>>n;
		n1=htonl(n);
		cout<<"started in cli1"<<endl;
		send(sockfd,&n1,sizeof n1,0);//sending n value to cli2
		cout<<"ended"<<endl;
		
		char str[100];
		for(int i=0;i<=2*n-1;i++)
		{
		recv(sockfd,&str[i],sizeof str[i],0);//receiving string generated from cli2
		}
		cout<<"msg value "<<endl;
		for(int i=0;i<=2*n-1;i++)
		 cout<<str[i] <<" ";
		cout<<"decomposing based on the constraints and enter values of uvw"<<endl;
		char u[100],v[100],w[100];
		cin>>u;//decomposed u
		send(sockfd,u,sizeof u,0);//sending to cli2
		cin>>v;//decomposed v
		send(sockfd,v,sizeof v,0);//sending to cli2
		cin>>w;//decomposed w
		send(sockfd,w,sizeof w,0);//sending to cli2
		
		int index,index1;
		recv(sockfd,&index1,sizeof index1,0);	//receiving i value 
		index=ntohl(index1);
		cout<<"value of index "<<index;
		
		recv(sockfd,u,sizeof u,0);//receiving uv^iw msg
		int x=0;
		cout<< "msg value "<<u;
		int a_count=0,b_count=0;
		for(int i=0;i<=strlen(u);i++)//calculating # of a's and b's
		 {
			if(u[i]=='a')
			 a_count++;
			if(u[i]=='b')
			 b_count++;
		 }
		for(int i=1;i<=index;i++)//recieving v index times
		{
			
			 recv(sockfd,v,sizeof v,0);
			 cout<<v;
			 x++;
		}	
		for(int j=1;j<=index;j++)//calculating # of a's and b's
		{
		for(int i=0;i<=strlen(v);i++)
		 {
			if(v[i]=='a')
			 a_count++;
			if(v[i]=='b')
			 b_count++;
		 }
		}
		recv(sockfd,w,sizeof w,0);//receveing w 
		cout<<w<<endl;
		for(int i=0;i<=strlen(w);i++)//calculating # of a's and b's
		 {
			if(w[i]=='a')
			 a_count++;
			if(w[i]=='b')
			 b_count++;
		 }
		cout<<"a_count "<<a_count<<"b_count "<<b_count<<endl;
		if(a_count!=b_count)//checking number of a's and b's are equal
		{
		  cout<<"count of a's and b's not equal"<<endl;
		
		cout<<"\nThis word sdoes not belong to the lang i am convinced L is non regular"<<endl;}
		cout<<"do u wish to continue"<<endl;//to continue
		cin>>value;
		int value1;
		value1=htonl(value);
		send(sockfd,&value1,sizeof value1,0);
	}
  }
  if(ch==0)//client2
  {
	if(r==1)
	{
		cout<<"client 2"<<endl;
		int n,n1;
		recv(sockfd,&n1,sizeof n1,0);//receiving n value 
		n=ntohl(n1);
		cout<<"value of n in client2"<<n<<endl;
		char str[100];//generating the string
		for(int i=0;i<=n-1;i++)
		 str[i]='a';
		for(int j=n;j<=2*n-1;j++)
	  	 str[j]='b';
		str[2*n]='\0';
		for(int i=0;i<=2*n-1;i++)
		 cout<<str[i]<<" ";
		for(int i=0;i<=2*n-1;i++)
		{
			send(sockfd,&str[i],sizeof str[i],0);//sending the string to cli1
		}
		
		cout<<"sent"<<endl;
		char u[100],v[100],w[100];
		recv(sockfd,u,sizeof u,0);//recving u value
		cout<<"value of u"<<u<<endl;

		recv(sockfd,v,sizeof v,0);//recving v value
		cout<<"value of v"<<v<<endl;

		recv(sockfd,w,sizeof w,0);//recving w value
		cout<<"value of w"<<w<<endl;
	
		int index,index1;//entering index values 
		cout<<"enter the value of i st uv^iw  doesnt belong to the lang"<<endl;
		cin>>index;
		index1=htonl(index);
		send(sockfd,&index1,sizeof index1,0);//sending this index to cli1
		
		cout<<"msg "<<endl;
		send(sockfd,u,sizeof u,0);//sending the msg of uv^iw
		cout<<u<<" ";
		for(int i=1;i<=index;i++)
		{
			
			 send(sockfd,v,sizeof v,0);
			cout<<v <<" ";
		}	
		send(sockfd,w,sizeof w,0);
		cout<<w<<endl;
		cout<<"do u wish to continue"<<endl;//to continue
		cin>>value;
		
	}	
  }
  if(r==2)//for a^p
  {
	if(ch==1)//cli1
	{
		int p,p1;
		cout<<"enter the value of p"<<endl;
		cin>>p;
		p1=htonl(p);
		send(sockfd,&p1,sizeof p1,0);//sending p value to cli2
		char str[100];
		cout<<"recvd msg "<<endl;
		for(int i=0;i<=p-1;i++){
		 recv(sockfd,&str[i],sizeof str[i],0);//receiving string generated from cli2
		 cout<<str[i]<<" ";}
		cout<<"\nsplitting into uvw "<<endl;
		char u[100],v[100],w[100];
		cin>>u;//decomposed u
		send(sockfd,u,sizeof u,0);//sending to cli2
		cin>>v;//decomposed v
		send(sockfd,v,sizeof v,0);//sending to cli2
		cin>>w;//decomposed w
		send(sockfd,w,sizeof w,0);//sending to cli2
		
		int index,index1;
		recv(sockfd,&index1,sizeof index1,0);//receiving i value 
		index=ntohl(index1);
		cout<<"index value is "<<index<<" ";
		
		recv(sockfd,u,sizeof u,0);//receiving uv^iw msg
		cout<< "msg value "<<u;
		for(int i=1;i<=index;i++)
		{
			
			 recv(sockfd,v,sizeof v,0);
			 cout<<v;
		}	
		recv(sockfd,w,sizeof w,0);
		cout<<w<<endl;
		int size=strlen(u)+index*strlen(v)+strlen(w);
		cout<<"\ngiven input:"<<size<<endl;
	int flag=0,result;
	for(int i=2;i<size;i++)//checking it is prime or not
	{
	 if(size%i==0)
	 {
		flag=1;
		break;
	 }
	}

	if(flag==0)
	{
	 result=1;
	 
	 cout<<"\nthe size is a prime number\n";
	}

	else if(flag==1)
	{
	 result=0;
	 
	 cout<<"\nthe size is not a prime number\n";
	  cout<<"\nThis word sdoes not belong to the lang i am convinced L is non regular"<<endl;
	}
	cout<<"\nIs prime completed\n";
		
		
		cout<<"do u wish to continue"<<endl;//to continue
		cin>>value;
		int value1;
		value1=htonl(value);
		send(sockfd,&value1,sizeof value1,0);
		
	}
	if(ch==0)//cli2
	{
		int p,p1;
		recv(sockfd,&p1,sizeof p1,0);//receiving p value
		p=ntohl(p1);
		char str[100];
		for(int i=0;i<=p-1;i++)//generating the string
		 str[i]='a';
		for(int i=0;i<=p-1;i++)
		{
			send(sockfd,&str[i],sizeof str[i],0);//sending the string to cli1
		}
		
		char u[100],v[100],w[100];
		cout<<"msg value "<<endl;
		recv(sockfd,u,sizeof u,0);//receiving u
		cout<<u<<" ";
		recv(sockfd,v,sizeof v,0);//receiving v
		cout<<v<<" ";
		recv(sockfd,w,sizeof w,0);//receiving w
		cout<<w<<" ";
		int index,index1;
		cout<<"enter the value of i st |uv^iw| is not prime"<<endl;
		cin>>index;
		index1=htonl(index);
		send(sockfd,&index1,sizeof index1,0);//sending i vlaue to cli1

		cout<<"msg "<<endl;
		send(sockfd,u,sizeof u,0);//sending uv^iw msg
		cout<<u;
		for(int i=1;i<=index;i++)
		{
			
			 send(sockfd,v,sizeof v,0);
			cout<<v <<" ";
		}	
		send(sockfd,w,sizeof w,0);
		cout<<w<<endl;
		cout<<"do u wish to continue"<<endl;//to continue
		cin>>value;
		
	}	
  }
  }while(value!=0);
  
}


int main(int argc,char* argv[])
{
  if(argc != 3)
    {
      cerr << "Usage syntax: ./client <ip_address> <port_no>" << endl;
     // return 0;
    }
	int port_no=atoi(argv[2]);
	
  client client1(port_no,argv[1]);
  client1.reg_lang();

  
   
  return 0;
}
