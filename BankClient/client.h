#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <Winsock2.h>
#include <ws2tcpip.h>

#include<iostream>
#define MAX 1024
using namespace std;
#ifndef BANKCLIENT_CLIENT_H
#define BANKCLIENT_CLIENT_H


class client {
public:
    client();
    void clientUI();
private:
    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[MAX];
    int n;
    void login();
    void menu();
    void withdraw();
    void deposit();
    void transfer();
    void transactions();
    void admin();
    void showAll();
    void reg();
    string email;
    string password;
    string amount;
    string convertToString(char* buffer);
    int mysend(SOCKET s,const char* buf);
    int myrecv(SOCKET s,string *buf);
    int count=0;
};
#endif //BANKCLIENT_CLIENT_H
