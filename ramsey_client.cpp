/*Friend/Enemy game:Client 1 is generating graph and sending it to client2 via server & client 2 tests whether there exists atleast three mutual friends or three mutual enemies in the graph*/

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
#include <math.h>

using namespace std;

class client
  {
      public:

	client(int argc,char *argv[]);
	void enthusiast();
	void ramsey();
	void powerset(int [],int );
	void testing();

	int sockfd;
  	struct sockaddr_in my_addr;
	int d[100],c[100],graph[10][10];
  };

client::client(int argc,char *argv[]) //connecting to server
  {
	if(argc != 3)
    	  {
      	    cerr << "Usage syntax: ./client <ip_address> <port_no>" << endl;
      	    return;
    	  }

  	sockfd=socket(PF_INET,SOCK_STREAM,0);

  	my_addr.sin_family=AF_INET;
  	my_addr.sin_port=htons(atoi(argv[2]));
  	my_addr.sin_addr.s_addr=inet_addr(argv[1]);
  	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  	int temp=connect(sockfd,(struct sockaddr*)&my_addr, sizeof my_addr);

  	if(temp==0)
    	   cout << "Connection established with server..." << endl;
  	else
    	 {
     	    cout << "Could not establish connection with server" << endl;
      	    return;
    	 }

	int choice;
	cout << "Do u want to claim(1/0):";
	cin >> choice;

	if(choice==1)
	  {
	      ramsey();
	  }
	else
	  {
	       enthusiast();
	  }
  }

void client::ramsey() //Game starts...
  {
	string st;
	int ch=1,choice,o,result=0;
	char buf[1024];

  	st="In any group of 6 people, there exist 3 mutual enemies or 3 mutual friends.This claim is true";
	strcpy(buf,st.c_str());
	int temp=send(sockfd,buf,strlen(buf)+1,0);

	if(temp==-1)
	  {
		cout << "message not sent...error occurred" << endl;
	  }

	temp=recv(sockfd,buf,sizeof buf,0);
	cout << "Client: " << buf << endl;
	if(temp==-1)
	  {
		cout << "message not received...error occurred" << endl;
	  }
	
	int i,j,k,b[100],count=0,n;
     
	n=6;
     	for(i=1;i<=6;i++)
	{
	   b[i]=i;
	}
     	k=0;
   
        for(i=1;i<=n;i++)
	{
	  for(j=1;j<=n;j++)
	    {
		if(b[i]<b[j])
		 {
		   count++;
	 	   d[k]=b[i];
		   c[k]=b[j];
		   k++;
		 }
	    }
	}
   
       powerset(d,count);
	
  }

void client::powerset(int d[100],int set_size) //Generating graphs and sending to client2 via server
  {
	int r=0,temp,j;
	unsigned int pow_set_size = pow(2, set_size);
	int counter,is_completed=0;
	char buf[1024];
	string st;
	int no_of_graphs=0;
     

        for(counter = 1; counter < pow_set_size; counter++)
        {
		r++;
		  
		for(int index=1;index<=6;index++)
		    {
		      for(int index1=1;index1<=6;index1++)
		       {
			graph[index][index1]=0;
		       }
		    }
	       
		for(j = 0; j < set_size; j++)  //block generating graphs
		    {
		      if(counter & (1<<j))
			{
			   graph[d[j]][c[j]]=1;
			   graph[c[j]][d[j]]=1;
			   cout << "(" << d[j] << "," << c[j] << ")" << "(" << c[j] << "," << d[j] << ")";
			}
		    }
	
		no_of_graphs++;
		cout << "No: of graphs:" << no_of_graphs << endl;
		
                st="Yes for sure.This is another example";
		strcpy(buf,st.c_str());
		sleep(1);
		temp=send(sockfd,buf,strlen(buf)+1,0);
		cout << "Sent yes" << endl;
	
		if(temp==-1)
		   cout << "Message not sent" << endl;
	
	        cout << "\nSending graph.." << endl;
		 for(int index=1;index<=6;index++)       //sending each graph to client2 via server
		    {
		      for(int index1=1;index1<=6;index1++)
		       {
			int o=htonl(graph[index][index1]);
			temp=send(sockfd,&o,sizeof o,0);
			if(temp==-1)
			  cout << "not sent" << endl;
		       }
		    }
		cout << "graph sent" << endl;
	
		
		temp=recv(sockfd,buf,sizeof buf,0);
		cout << "Client: " << buf << endl;
		if(temp==-1)
		  cout << "Message not received" << endl;
	
		if(strcmp(buf,"I am convinced")==0)
		 {
		    is_completed=1;
		    close(sockfd);
		    break;
		 }
		else
		    continue;
	
	}
		if(is_completed!=1)   //Sent all possible graphs on 6 vertices and the proof completed
		 {
		    st="Thats all";
		    strcpy(buf,st.c_str());
		    temp=send(sockfd,buf,strlen(buf)+1,0);
		    
                    if(temp==-1)
		        cout << "Message not sent" << endl;
		    close(sockfd);
		 }
  }

void client::enthusiast()  //client 2: Not believing claim that has given by client1
  {
	char buf[1024];
	int temp;
	temp=recv(sockfd,buf,sizeof buf,0);
	cout << "Client: " << buf << endl;
	if(temp==-1)
	  {
		cout << "message not received...error occurred" << endl;
	  }
	

        string st;
	st="I won't believe that the claim is true. Can u prove it by example?";
	strcpy(buf,st.c_str());
	temp=send(sockfd,buf,strlen(buf)+1,0);
	if(temp==-1)
	  {
		cout << "message not sent...error occurred" << endl;
	  }
	

        testing();
  }

void client::testing() //Receiving graphs and testing it
  {
	int temp,j,o;
	char buf[1024];
	string st;
	

        while(1)
	{
		cout << "\nReceiving yes" << endl;
		//sleep(1);
		temp=recv(sockfd,buf,sizeof buf,0);
		cout << "Client: " << buf << endl;
		if(strcmp(buf,"Thats all")==0)
		{
		  close(sockfd);
		  break;
		}
		

                cout << "receiving graph" << endl;
		for(int index=1;index<=6;index++)
		{
		    for(int index1=1;index1<=6;index1++)
		   {
			temp=recv(sockfd,&o,sizeof o,0);
			graph[index][index1]=ntohl(o);
			
                        if(temp!=-1);
			  //cout << "Received " << graph[index][index1] << endl;
			else
			  cout << "Not received" << endl;
			
                        if(graph[index][index1]==1)
			  cout << "(" << index << "," << index1 << ")";
		   }
		}
		cout << "completed receiving graph" << endl;
		

                int is_perfect=0;
		for(int index=1;index<=6;index++)
		{
		  for(int index1=index+1;index1<=6;index1++)
		   {
			if(graph[index][index1]==1)        //checks for 3 mutual friends
		 		{
				    int sum=0;
				    for(j=1;j<=6;j++)
				      {
					 sum=sum+(graph[index][j] & graph[index1][j]);
				      }
				    if(sum>=1)
				      {	
					 is_perfect=1;
					 break;
				      }
		 		}
			else
		                {
				    for(j=1;j<=6;j++)            //checks for 3 mutual enemies
				      {
					if(graph[index][j]==0 && graph[index1][j]==0)
					  {
					    is_perfect=1;
					    
					    break;
					  }
				      }  
		 		}

		   }
		}
		
                cout << "\nThere exists atleast 3 mutual friends or 3 mutual enemies" << endl;
		st=="Perfect...But i am not convinced send the next graph";
		strcpy(buf,st.c_str());
		temp=send(sockfd,buf,strlen(buf)+1,0);
		if(temp==-1)
		{
		cout << "Message not sent...error occurred" << endl;
		}


		}
  }

int main(int argc,char *argv[])
  {
	client obj(argc,argv);
	return 0;
  }



