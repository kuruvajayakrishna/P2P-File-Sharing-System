#include<bits/stdc++.h>
#include<sys/socket.h>
#include<error.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
//#include "client_server.cpp"
using namespace std;
#define MAX_SIZE 65535
#define CHUNK_SIZE 524288
string username;
string myip;
string tracker_ip="";
string tracker_port="";
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
    auto pid=pthread_create(&tid,NULL,ClientServer,&myport_i);
    if(pid!=0)
    {
        perror("thread creation failed");
		exit(-1);
    }
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
           if(sock=socket(AF_INET,SOCK_STREAM,0)==-1)
            {
                perror("socket");
                exit(-1);
            }
           tracker.sin_family=AF_INET;
           tracker.sin_port=htons(tracker_port);
           tracker.sin_addr.s_addr=inet_addr(tracker_ip.c_str());
           bzero(&tracker.sin_zero,8);
           if((connect(sock,(struct sockaddr*)&tracker,sizeof(struct sockaddr_in)))==-1)
            {
                perror("connect");
                exit(-1);
            }
           //command format login username password
           user_command>>split_command[1];
           user_command>>split_command[2];
           string data="login "+split_command[1]+" "+split_command[2]+" "+to_string(myport_i)+" "+myip;
           //sending data to tracker online
	       send(sock,data.c_str(),data.size(),0);
	       int len=recv(sock,output,MAX_SIZE,0);
           close(sock);
	       output[len]='\0';
           if(output[0]=='0')
                cout<<"login Failed"<<endl;
           else
                cout<<"successufully"<<endl;
       }
       /*
       else if(command_split=="create_user"){
            string data="create_user "+username+" "+password+" "+to_string(myport_i)+" "+myip;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Invalid create user request "<<endl;
            else
                cout<<"successfully created_user"<<endl;
       }
       else if(command_split=="create_group"){
            string data="create_group "+groupname+" "+username;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Invalid create group request "<<endl;
            else
                cout<<"successfully created group"<<endl;
       }
       else if(command_split=="list_groups"){
           string data="list_groups";
           send(sock,data.c_str(),data.size(),0);
           int len=recv(sock,output,MAX_SIZE,0);
           output[len]='\0';
           cout<<"##### List of Groups #####"<<endl<<endl;
           cout<<output<<endl;
           cout<<"#####  #####"<<endl;
       }
       else if(command_split=="join_group"){
           	string data="join_group "+groupname+" "+username;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Failure in joining of group "<<endl;
            else
               cout<<"successfully joined group"<<endl;
       }
       else if(command_split=="list_requests"){
            string data="list_requests "+groupname+" "+username;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            cout<<"##### List of Requests #####"<<endl<<endl;
            cout<<output<<endl;
            cout<<"#####  #####"<<endl;
       }
       else if(command_split=="show_downloads"){
            string data="show_downloads "+username;
        	send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            cout<<"##### List of Downloads #####"<<endl<<endl;
            cout<<output<<endl;
            cout<<"#####  #####"<<endl;
       }
       else if(command_split=="download_file"){}/////Download file pending
       else if(command_split=="list_files"){
            string data="list_files "+groupname;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            cout<<"##### List of Files #####"<<endl<<endl;
            cout<<output<<endl;
            cout<<"#####  #####"<<endl;
       }
       else if(command_split=="stop_share"){
            string data="stop_share "+groupname+" "+filename+" "+username;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Invalid stop sharing request"<<endl;
            else
                cout<<"successfully stopped sharing"<<endl;
       }
       else if(command_split=="upload_file"){}///upload file pending
       else if(command_split=="accept_request"){
            string data="accept_request "+groupname+" "+username1+" "+username;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Please try later"<<endl;
            else
                cout<<"successfully accepted"<<endl;
       }
       else if(command_split=="leave_group"){
            string data="leave_group "+groupname+" "+username;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Invalid leave_group request"<<endl;
            else
                cout<<"succussfully left_out of group"<<endl;
       }
       else if(command_split=="logout"){
           	string data="logout "+username;
            send(sock,data.c_str(),data.size(),0);
            int len=recv(sock,output,MAX_SIZE,0);
            output[len]='\0';
            if(output[0]=='0')
                cout<<"Invalid Logout request"<<endl;
            else
                cout<<"you logged_out"<<endl;
       }  */  
    }
}