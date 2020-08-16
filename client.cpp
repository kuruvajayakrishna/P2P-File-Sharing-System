#include<iostream>
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
#include <stdio.h>
#include <stdlib.h>
#include <utility>
using namespace std;
#define MAX_SIZE 65535
#define CHUNK_SIZE 524288
string user_name;
string myip;
string tracker_ip="127.0.0.1";
int tracker_port=1047;
bool check_tracker_online(string filepath)
{
return true;
}
int main(int argc,char **argv)
{
    bool login_flag=false;
    string command;
    vector<string>split_command(10);
    pthread_t tid;//creating a thread id for handling peer connections
    //input command given by peer ./client <ip> <port> <file_name_containing info of all trackers>
    myip=argv[1];
	string myport=argv[2];
	int myport_i=stoi(myport);
	string filepath=argv[3];
    /*auto pid=pthread_create(&tid,NULL,ClientServer,&myport_i);
    if(pid!=0)
    {
        perror("thread creation failed");
		exit(-1);
    }*/
    //main thread handles commands entered by peer
    while(true)
    {
       getline(cin,command);   
       stringstream user_command(command);
       user_command>>split_command[0];
       if(split_command[0]=="quit")
         break;
       if(!check_tracker_online(filepath))
       {
           cout<<"No tracker found online"<<endl;
           continue;
       }
       if(split_command[0]=="login"){
           if(login_flag)
           {
               cout<<"Already_logged_in"<<endl;
               continue;
           }
           //Making connection with available tracker online
           struct sockaddr_in tracker;
           int sock;
           if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(-1);
            }
           char output[MAX_SIZE];
           tracker.sin_family=AF_INET;
           tracker.sin_port=htons(tracker_port);
           tracker.sin_addr.s_addr=inet_addr(tracker_ip.c_str());
           bzero(&tracker.sin_zero,8);
           if((connect(sock,(struct sockaddr*)&tracker,sizeof(struct sockaddr_in)))==-1)
            {
                perror("connect");
                exit(-1);
            }
           //command format login user_name password
           user_command>>split_command[1];//usr_name
           user_command>>split_command[2];//password
           string data="login "+split_command[1]+" "+split_command[2]+" "+to_string(myport_i)+" "+myip;
           //sending data to tracker online
	       send(sock,data.c_str(),data.size(),0);
	       int len=recv(sock,output,MAX_SIZE,0);
           close(sock);
	       output[len]='\0';
           if(output[0]=='0')
                cout<<"login Failed"<<endl;
           else
           {
                cout<<"successfully"<<endl;
                user_name=split_command[1];
           }
       }
       else if(split_command[0]=="create_user"){
           //Making connection with available tracker online
           struct sockaddr_in tracker;
           int sock;
           if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(-1);
            }
           char output[MAX_SIZE];
           tracker.sin_family=AF_INET;
           tracker.sin_port=htons(tracker_port);
           tracker.sin_addr.s_addr=inet_addr(tracker_ip.c_str());
           bzero(&tracker.sin_zero,8);
           if((connect(sock,(struct sockaddr*)&tracker,sizeof(struct sockaddr_in)))==-1)
            {
                perror("connect");
                exit(-1);
            }
           //command format login user_name password
           user_command>>split_command[1];//usr_name
           user_command>>split_command[2];//password
           string data="create_user "+split_command[1]+" "+split_command[2]+" "+to_string(myport_i)+" "+myip;
           //sending data to tracker online
	       send(sock,data.c_str(),data.size(),0);
	       int len=recv(sock,output,MAX_SIZE,0);
           close(sock);
	       output[len]='\0';
            if(output[0]=='0')
                cout<<"Invalid create user request "<<endl;
            else
                cout<<"successfully created_user"<<endl;
       }
       else if(split_command[0]=="create_group"){
           struct sockaddr_in tracker;
           int sock;
           if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(-1);
            }
           char output[MAX_SIZE];
           tracker.sin_family=AF_INET;
           tracker.sin_port=htons(tracker_port);
           tracker.sin_addr.s_addr=inet_addr(tracker_ip.c_str());
           bzero(&tracker.sin_zero,8);
           if((connect(sock,(struct sockaddr*)&tracker,sizeof(struct sockaddr_in)))==-1)
            {
                perror("connect");
                exit(-1);
            }
            //command format create_group group_id
            user_command>>split_command[1];//group_id
            string data="create_user "+split_command[1]+" "+user_name;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Invalid create group request "<<endl;
            else
                cout<<"successfully created group"<<endl;
       }
       else if(split_command[0]=="list_groups"){
           struct sockaddr_in tracker;
           int sock;
           if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(-1);
            }
           char output[MAX_SIZE];
           tracker.sin_family=AF_INET;
           tracker.sin_port=htons(tracker_port);
           tracker.sin_addr.s_addr=inet_addr(tracker_ip.c_str());
           bzero(&tracker.sin_zero,8);
           if((connect(sock,(struct sockaddr*)&tracker,sizeof(struct sockaddr_in)))==-1)
            {
                perror("connect");
                exit(-1);
            }
           //command format list_groups
           string data="list_groups";
           send(sock,data.c_str(),data.size(),0);
           int len=recv(sock,output,MAX_SIZE,0);
           output[len]='\0';
           cout<<"##### List of Groups #####"<<endl<<endl;
           cout<<output<<endl;
           cout<<"#####  #####"<<endl;
       }
       else if(split_command[0]=="join_group"){
           struct sockaddr_in tracker;
           int sock;
           if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(-1);
            }
           char output[MAX_SIZE];
           tracker.sin_family=AF_INET;
           tracker.sin_port=htons(tracker_port);
           tracker.sin_addr.s_addr=inet_addr(tracker_ip.c_str());
           bzero(&tracker.sin_zero,8);
           if((connect(sock,(struct sockaddr*)&tracker,sizeof(struct sockaddr_in)))==-1)
            {
                perror("connect");
                exit(-1);
            }
            //command format join_group group_id
            user_command>>split_command[1];
           	string data="join_group "+split_command[1]+" "+user_name;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Group doen't exists "<<endl;
            else
               cout<<"successfully joined group"<<endl;
       }
       else if(split_command[0]=="list_requests"){
           struct sockaddr_in tracker;
           int sock;
           if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(-1);
            }
           char output[MAX_SIZE];
           tracker.sin_family=AF_INET;
           tracker.sin_port=htons(tracker_port);
           tracker.sin_addr.s_addr=inet_addr(tracker_ip.c_str());
           bzero(&tracker.sin_zero,8);
           if((connect(sock,(struct sockaddr*)&tracker,sizeof(struct sockaddr_in)))==-1)
            {
                perror("connect");
                exit(-1);
            }
            //command format for list_request group_id
            user_command>>split_command[1];
           	string data="join_group "+split_command[1]+" "+user_name;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            cout<<"##### List of Requests #####"<<endl<<endl;
            cout<<output<<endl;
            cout<<"#####  #####"<<endl;
       }/*
       else if(split_command[0]=="show_downloads"){
            string data="show_downloads "+user_name;
        	send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            cout<<"##### List of Downloads #####"<<endl<<endl;
            cout<<output<<endl;
            cout<<"#####  #####"<<endl;
       }
       else if(split_command[0]=="download_file"){}/////Download file pending
       else if(split_command[0]=="list_files"){
            string data="list_files "+groupname;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            cout<<"##### List of Files #####"<<endl<<endl;
            cout<<output<<endl;
            cout<<"#####  #####"<<endl;
       }
       else if(split_command[0]=="stop_share"){
            string data="stop_share "+groupname+" "+filename+" "+user_name;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Invalid stop sharing request"<<endl;
            else
                cout<<"successfully stopped sharing"<<endl;
       }
       else if(split_command[0]=="upload_file"){}///upload file pending*/
       else if(split_command[0]=="accept_request"){
           struct sockaddr_in tracker;
           int sock;
           if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(-1);
            }
           char output[MAX_SIZE];
           tracker.sin_family=AF_INET;
           tracker.sin_port=htons(tracker_port);
           tracker.sin_addr.s_addr=inet_addr(tracker_ip.c_str());
           bzero(&tracker.sin_zero,8);
           if((connect(sock,(struct sockaddr*)&tracker,sizeof(struct sockaddr_in)))==-1)
            {
                perror("connect");
                exit(-1);
            }
            //command format accept_request group_id user_name
            user_command>>split_command[1];
            user_command>>split_command[2];
            string data="accept_request "+split_command[1]+" "+split_command[2]+" "+user_name;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Please try later"<<endl;
            else
                cout<<"successfully accepted"<<endl;
       }
       else if(split_command[0]=="leave_group"){
           struct sockaddr_in tracker;
           int sock;
           if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(-1);
            }
           char output[MAX_SIZE];
           tracker.sin_family=AF_INET;
           tracker.sin_port=htons(tracker_port);
           tracker.sin_addr.s_addr=inet_addr(tracker_ip.c_str());
           bzero(&tracker.sin_zero,8);
           if((connect(sock,(struct sockaddr*)&tracker,sizeof(struct sockaddr_in)))==-1)
            {
                perror("connect");
                exit(-1);
            }
            //command format leave_group group_id
            user_command>>split_command[1];
           	string data="leave_group "+split_command[1]+" "+user_name;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Invalid leave_group request"<<endl;
            else
                cout<<"succussfully left_out of group"<<endl;
       }
       else if(split_command[0]=="logout"){
           struct sockaddr_in tracker;
           int sock;
           if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
            {
                perror("socket");
                exit(-1);
            }
           char output[MAX_SIZE];
           tracker.sin_family=AF_INET;
           tracker.sin_port=htons(tracker_port);
           tracker.sin_addr.s_addr=inet_addr(tracker_ip.c_str());
           bzero(&tracker.sin_zero,8);
           if((connect(sock,(struct sockaddr*)&tracker,sizeof(struct sockaddr_in)))==-1)
            {
                perror("connect");
                exit(-1);
            }
           	string data="logout "+user_name;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Invalid Logout request"<<endl;
            else
                cout<<"you logged_out"<<endl;
       }   
    }
}