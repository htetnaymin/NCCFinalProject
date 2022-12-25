#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <Winsock2.h>
#include <ws2tcpip.h>

#include<iostream>
#include<limits>
using namespace std;
#include "client.h"
client::client()
{
    WSADATA wsaData;
    char *ip = "127.0.0.1";
    int port = 5566;
    WSAStartup(MAKEWORD(2,2),&wsaData);
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");
    memset(&addr,'\0',sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=port;
    addr.sin_addr.s_addr=inet_addr(ip);

    if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))<0)
    {
        printf("[-]Can't connect the server \n");
        exit(1);
    };
    printf("Connected to the server.\n");
    clientUI();
//    memset(buffer,'\0',MAX);
//    strcpy(buffer,"Hello, This is client.");
//    send(sock,buffer,strlen(buffer),0);
//
//    memset(buffer,'\0',MAX);
//    recv(sock,buffer,sizeof(buffer),0);
//    printf("Server : %s\n",buffer);
//
//    close(sock);
//    printf("Disconnected from server\n");
}

void client::clientUI()
{

    memset(buffer,'\0',MAX);
    int condition=0;
    cout<<"Welcome to the bank."<<endl;
    cout<<"1.Login\n2.Register\n0.Exit";
    cin>>condition;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (condition)
    {
        case 1:
            mysend(sock,"Login");
            login();
            break;
        case 2:
            mysend(sock,"Register");
            reg();
            break;
        case 0:
            close(sock);
            printf("Disconnected from server\n");
            mysend(sock,"Exit");
            exit(0);
            break;
        default:
            cout<<"Invalid Option!"<<endl;
            clientUI();
    }
}
void client::login()
{
    cout<<"This is Login."<<endl;
    cout<<"Enter email : ";
    getline(cin,email);
    mysend(sock, email.c_str());
    cout<<"Enter Password : ";
    getline(cin,password);
    mysend(sock,password.c_str());
    string temp;
    myrecv(sock,&temp);
    if(temp=="0")
    {
        myrecv(sock,&temp);
        if(stoi(temp))
        {
            admin();
            exit(0);
        }
        count++;
        if(count>3)
        {
            clientUI();
            exit(0);
        }
        login();
        exit(0);
    }
    menu();
}
void client::menu()
{
    int condition;

    myrecv(sock,&amount);
    cout<<"Your Current Amount is : "<<amount<<endl;
    cout<<"1.Withdraw Money\n2.Deposit Money\n3.Transfer Money\n4.Transactions History\n0.Return";
    cin>>condition;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (condition)
    {
        case 1:
            mysend(sock,"Withdraw");
            withdraw();
//            login();
            break;
        case 2:
            mysend(sock,"Deposit");
            deposit();
            break;
        case 3:
            mysend(sock,"Transfer");
            transfer();
            break;
        case 4:
            mysend(sock,"Transactions");
            transactions();
            menu();
            break;
        case 0:
            mysend(sock,"Return");
            clientUI();
            exit(0);
            break;
        default:
            cout<<"Invalid Option!"<<endl;
            clientUI();
    }
}
void client::transactions()
{
    cout<<"***Transactions History***"<<endl;
    string temp;
    myrecv(sock,&temp);
    int count=stoi(temp);
    for (int i = 0; i < count; ++i)
    {
        myrecv(sock,&temp);
        cout<<temp<<endl;
    }
}
void client::withdraw()
{
    cout<<"Enter Amount to withdraw : ";
    getline(cin,amount);
    mysend(sock,amount.c_str());
    cout<<"Success!"<<endl;
    menu();
}
void client::deposit()
{
    cout<<"Enter Amount to deposit : ";
    getline(cin,amount);
    mysend(sock,amount.c_str());
    cout<<"Success!"<<endl;
    menu();
}
void client::transfer()
{
    string recipentemail;
    cout<<"Enter the recipient's account email : ";
    getline(cin,recipentemail);
    mysend(sock,recipentemail.c_str());
    string temp;
    myrecv(sock,&temp);
    if(temp=="0")
    {
        cout<<"Sorry, there was an error processing your request. \nPlease check the recipient email and try again."<<endl;
        menu();
    }
    else
    {
        cout<<"Enter Amount to transfer : ";
        getline(cin,amount);
        mysend(sock,amount.c_str());
        cout<<"You have transferred "<<amount<<"$ to the recipient's account "<<recipentemail<<"."<<endl;
        menu();
    }
}
void client::admin()
{
    int condition=0;
    cout<<"1.Show All Info\n2.Show Info Specific\n3.Manage Users\n4.Transactions History\n0.Return";
    cin>>condition;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (condition)
    {
        case 1:
            mysend(sock,"Show All");
            showAll();
            break;
        case 2:
            mysend(sock,"Show Specific");
            deposit();
            break;
        case 3:
            mysend(sock,"Manage");
            transfer();
            break;
        case 4:
            mysend(sock,"Transactions");
            transactions();
            menu();
            break;
        case 0:
            mysend(sock,"Return");
            clientUI();
            exit(0);
            break;
        default:
            cout<<"Invalid Option!"<<endl;
            clientUI();
    }
}
void client::showAll()
{
    string temp;
    myrecv(sock,&temp);
    while(temp!="Stop")
    {
        cout<<temp<<endl;
        myrecv(sock,&temp);
        int l=stoi(temp);
        for (int k = 0; k < l; ++k)
        {
            myrecv(sock,&temp);
            cout<<temp<<endl;
        }
        myrecv(sock,&temp);
        cout<<endl;
    }
//    myrecv(sock,&temp);
//    const int j=stoi(temp);
//    for (int i = 0; i < j; ++i)
//    {
//        myrecv(sock,&temp);
//        cout<<temp;
//        myrecv(sock,&temp);
//        int l=stoi(temp);
//        for (int k = 0; k < l; ++k)
//        {
//            myrecv(sock,&temp);
//            cout<<temp<<endl;
//        }
//    }
}
void client::reg()
{
    cout<<"Enter email : ";
    getline(cin,email);
    mysend(sock, email.c_str());
//    memset(buffer,'\0',MAX);
//    strcpy(buffer,username.c_str());
//    send(sock,buffer,strlen(buffer),0);
//    memset(buffer,'\0',MAX);
    cout<<"Enter Password : ";
    getline(cin,password);
    mysend(sock,password.c_str());
    cout<<"Enter Amount : ";
    getline(cin,amount);
    mysend(sock,amount.c_str());

}
string client::convertToString(char* buffer)
{
    int i;
    string s="";
    for (int i = 0; i < strlen(buffer); ++i)
    {
        s=s+buffer[i];
    }
    return s;
}
int client::mysend(SOCKET s, const char *buf)
{
    char buffer[MAX];
    memset(buffer,'\0',MAX);
    strcpy(buffer,buf);
    printf("Client : %s\n",buffer);
    return send(s,buffer,strlen(buffer),0);
}

int client::myrecv(SOCKET s,string *buf)
{
    char buffer[MAX];
    memset(buffer,'\0',MAX);
    int temp=recv(s,buffer,sizeof(buffer),0);
    printf("Server : %s\n",buffer);
    *buf=convertToString(buffer);
    return temp;
}

