#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>
#include <pthread.h>
#include <dirent.h>
#define MAX_SIZE 65535

using namespace std;
pthread_mutex_t file_lock;
pthread_mutex_t global_lock;

int my_sequence_no;
vector<pair<string,string>>tracker_info;
unordered_map<string,string>user_credentails;
set<string>online;
struct Message
{
    int new_cli;
	struct sockaddr_in client;
	string client_ip;
	string client_port;
};

bool userlogin(string user_name,string password,string port,string ip)
{
    bool flag;
    if(user_credentails.find(user_name)==user_credentails.end())
     return false;
    else
    {
        if(password==user_credentails[user_name])
        {
            if(online.find(user_name)==online.end())
            {
                online.insert(user_name);
              	//pthread_mutex_lock(&file_lock);
				ofstream outfile(".all_files/.online.txt",ios::out|ios::app);
				outfile<<user_name<<" ";
				outfile<<ip<<" ";
				outfile<<port;
				outfile<<endl;
				//pthread_mutex_unlock(&file_lock);   
                //do syncing 
              }
            flag=true;
        }
        else
        {
            flag=false;
        }
    }
    return flag;
}
bool createuser(string user_name,string password,string port,string ip)
{
    bool flag;
    if(user_credentails.find(user_name)==user_credentails.end())
    {
        cout<<"creating files"<<endl;
        //pthread_mutex_lock(&file_lock);
		online.insert(user_name);
		ofstream outfile("files/online.txt",ios::out|ios::app);
		outfile<<user_name<<" ";
		outfile<<ip<<" ";
		outfile<<port;
		outfile<<endl;
        outfile.close();
        //updating user details in user_details.txt file
		ofstream outfile2("files/user_details.txt",ios::out|ios::app);
		outfile2<<user_name<<" ";
		outfile2<<password;
		outfile2<<endl;
        outfile2.close();
        //creating files for the new users
		string name="files/files_"+user_name+".txt";
		ofstream outfile3(name,ios::out | ios::app);
		outfile3.close();
		string name1="files/files_"+user_name+"_downloading.txt";
		ofstream outfile4(name1,ios::out|ios ::app);
		outfile4.close();
		string name2="files/files_"+user_name+"_downloaded.txt";
		ofstream outfile5(name2,ios::out|ios ::app);
		outfile5.close();
        /*
		pthread_mutex_unlock(&file_lock);
		Sync(".all_files/.online.txt",mysequence_i,tracker_info);
		Sync(".all_files/.credential.txt",mysequence_i,tracker_info);
		Sync(name,mysequence_i,tracker_info);
		Sync(name1,mysequence_i,tracker_info);
		Sync(name2,mysequence_i,tracker_info);*/
		//getDetails();
        user_credentails[user_name]=password;
        cout<<user_credentails.size()<<endl;
		flag=true;
	}
    else
    {
        cout<<"Hello "<<endl;
        flag=false;//user already exists
    }
    return flag;
}
bool groupcreate()
{}
string grouplist()
{}
string grouprequestlist()
{}
string groupfilelist()
{}
bool groupjoin()
{}
bool groupacceptrequest()
{}
bool groupfileupload()
{}
bool groupstopshare()
{}
bool groupleave()
{}
string sendSHA()
{}
string sendseeder()
{}
string senddetails()
{}
string getfullpath()
{}
void updatedownload()
{}
string showdownloads()
{}
void completedownload()
{}
bool logout()
{}
void *handle_connections(void *pointer)
{
    Message *message=(Message *)pointer;
    vector<string>split_command(10);
    int data_len;
	char data[MAX_SIZE+1];
    cout<<"Handle connections "<<endl;
	memset(data,'\0', (MAX_SIZE+1)*sizeof(data[0]));
    data_len=recv(message->new_cli,data,MAX_SIZE,0);
	data[data_len]='\0';
    string command(data);
    stringstream command_object(command);
    command_object>>split_command[0];
    cout<<split_command[0]<<endl;
    if(split_command[0]=="login"){
        cout<<"login request"<<endl;
        command_object>>split_command[1];//usr_name
        command_object>>split_command[2];//password
        command_object>>split_command[3];//port
        command_object>>split_command[4];//ip
        if(userlogin(split_command[1],split_command[2],split_command[3],split_command[4]))
         send(message->new_cli,"1",1,0);
        else
         send(message->new_cli,"0",1,0);
    }
    else if(split_command[0]=="create_user"){
        cout<<"create_user_request "<<endl;
        command_object>>split_command[1];//usr_name
        command_object>>split_command[2];//password
        command_object>>split_command[3];//port
        command_object>>split_command[4];//ip
        if(createuser(split_command[1],split_command[2],split_command[3],split_command[4]))
         send(message->new_cli,"1",1,0);
        else
         send(message->new_cli,"0",1,0);
    }/*
    else if(split_command[0]=="create_group"){}
    else if(split_command[0]=="list_groups"){}
    else if(split_command[0]=="join_group"){}
    else if(split_command[0]=="list_requests"){}
    else if(split_command[0]=="show_downloads"){}
    else if(split_command[0]=="download_file"){}
    else if(split_command[0]=="stop_share"){}
    else if(split_command[0]=="upload_file"){}
    else if(split_command[0]=="accept_request"){}
    else if(split_command[0]=="leave_group"){}
    else if(split_command[0]=="logout"){}
    else if(split_command[0]==""){}//
    else if(split_command[0]==""){}//many other functionalities*/
    close(message->new_cli);
	return NULL;
}
void *TrackerServer(void *pointer)
{
    struct sockaddr_in client,server;
    int sock,new_cli,counter=0;
    pthread_t tid[1000];
    socklen_t len=sizeof(struct sockaddr_in);
    int sequence_no=*((int *)pointer);	
	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){//creating socket
		perror("server socket: ");
		exit(-1);
	}
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi((tracker_info[sequence_no].second).c_str()));
	server.sin_addr.s_addr=inet_addr((tracker_info[sequence_no].first).c_str());
	bzero(&server.sin_zero,8);
	if((bind(sock,(struct sockaddr*)&server,sizeof(struct sockaddr_in)))==-1){
		perror("Bind");
		exit(-1);
	}
	if(listen(sock,8)==-1){ //socket listenng for requests from peers
		perror("Listen");
		exit(-1);
	} 
    cout<<"waiting for accepted"<<endl;
	while(true){//accepting connections
		if((new_cli=accept(sock,(struct sockaddr*)&client,&len))==-1){
			perror("accept");
			exit(-1);
		}
        cout<<"connection accepted"<<endl;
        Message* message=new Message;
		message->new_cli=new_cli;
		message->client=client;
		message->client_port=to_string(ntohs(client.sin_port));
		string temp(inet_ntoa(client.sin_addr));
		message->client_ip=temp;
       //once connection is accepted from peer we create another thread to specifically handle that peer connection
        auto pid=pthread_create(&tid[counter],NULL,handle_connections,(void *)message);
        if(pid!=0){
            perror("thread failed");
            exit(-1);
        }
		++counter;
		counter=counter%1000;
    }
}
int main(int argc, char **argv)
{
  //comand format to start tracker ./tracker​ tracker_info.txt ​ tracker_no
  cout<<"Tracker started "<<endl;
  pthread_t tid;
  string tracker_ip,tracker_port;
  string filename=argv[1];
  string sequence=argv[2];
  my_sequence_no=atoi(sequence.c_str())-1;
  //cout<<my_sequence_no<<endl;
  //load details from tracker_file
  string line;
  ifstream infile;
  infile.open(filename,ios::in);
  while(getline(infile,line))
  {
       stringstream line_obj(line);
       line_obj>>tracker_ip;
       line_obj>>tracker_port;
       //cout<<tracker_ip<<" "<<tracker_port<<endl;
       tracker_info.push_back({tracker_ip,tracker_port});
  }
  //creating a thread to handle connections with peers
  auto pid=pthread_create(&tid,NULL,TrackerServer,&my_sequence_no);
  if(pid!=0){
		perror("Thread creation failed");
		exit(-1);
	}
  bool flag=true;
  string command;
  while(flag){
		cin>>command;
		if(command=="quit"){
			flag=false;
		}
		if(command=="print"){
			//print_all();
		}
	}
}