#include<bits/stdc++.h>
using namespace std;

bool loginuser(){};
bool createuser(){};
bool creategroup(){};
void list_groups(){};
bool joingroup(){};
void listgrouprequests(){};
void gruprequestfiles(){};
bool acceptgrouprequest(){};
bool upload_file(){};
bool stopgroupsharing(){};
bool leavegroup(){};
void showdownloads(){};
bool logout(){};

int main(int argc,char **argv)
{
    string command;
    //creating a thread for handling peer connections
    while(true)
    {
       //main thread handles commands entered by peer
       getline(cin,command);   
       stringstream user_command(command);
       if(command_split=="login"){};
       else if(command_split=="create_user"){};
       else if(command_split=="create_group"){};
       else if(command_split=="list_groups"){};
       else if(command_split=="list_requests"){};
       else if(command_split=="show_downloads"){};
       else if(command_split=="download_file"){};
       else if(command_split=="list_files"){};
       else if(command_split=="stop_share"){};
       else if(command_split=="upload_file"){};
       else if(command_split=="accept_request"){};
       else if(command_split=="leave_group"){};
       else if(command_split=="logout"){};     
    }
}