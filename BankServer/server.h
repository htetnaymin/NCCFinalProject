#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include <sstream>
#include<unistd.h>
#include <Winsock2.h>
#include <ws2tcpip.h>
#include<unistd.h>
#include<iostream>
#include<vector>
#include<fstream>
#include<queue>
#include<chrono>
#ifndef BANKSERVER_SERVER_H
#define BANKSERVER_SERVER_H
#define MAX 1024
using namespace std;
class Node
{
public:
    string email;
    string password;
    int amount;
    vector<string> transactions;
    Node* left;
    Node* right;
};
class binaryTree{
public:
    struct Node* reload()
    {
        fstream t;
        string temp;
        Node* root=createNode();
        Node* node=root;
        f.open("database.txt",ios::in);
        t.open("transactions.txt",ios::in);
        if (!f)
        {
            f.close();
            ofstream database("database.txt");
            database.close();

            t.close();
            ofstream transactions("transactions.txt");
            transactions.close();
            return root;
        }
        getline(f,temp,'|');
        node->email=temp;
        getline(f,temp,'|');
        node->password=temp;
        getline(f,temp,'\n');
        node->amount=stoi(temp);
        while(t.peek()!='\n'&&t.peek()!=EOF)
        {
            getline(t,temp,'|');
            node->transactions.push_back(temp);
        }
        if(t.peek()=='\n')
            getline(t,temp,'\n');
        while(f.peek() != EOF)
        {
            node=newNode(root);
            getline(f,temp,'|');
            node->email=temp;
            getline(f,temp,'|');
            node->password=temp;
            getline(f,temp,'\n');
            node->amount=stoi(temp);

            while(t.peek()!='\n'&&t.peek()!=EOF)
            {
                getline(t,temp,'|');
                node->transactions.push_back(temp);
            }
            if(t.peek()=='\n')
                getline(t,temp,'\n');
        }
        f.close();
        t.close();
//        treload(root);
        return root;
    }
    struct Node* newNode(Node* root)
    {
        if(root==NULL)
        {
            root=createNode();
        }
        queue<Node*> q;
        q.push(root);
        while(!q.empty())
        {
            Node* temp=q.front();
            q.pop();
            if(temp->left!=NULL)
            {
                q.push(temp->left);
            }
            else
            {
                temp->left= createNode();
                return temp->left;
            }
            if (temp->right!=NULL)
            {
                q.push(temp->right);
            }
            else
            {
                temp->right= createNode();
                return temp->right;
            }
        }
    }
    int sizeofbinaryTree(Node* root)
    {
        int count=0;
        if(root==NULL)
            return  count;
        count++;
        queue<Node*> q;
        q.push(root);
        while(!q.empty())
        {
            Node* temp=q.front();
            q.pop();
            if(temp->left!=NULL)
            {
                count++;
                q.push(temp->left);
            }
            else
            {
                return count;
            }
            if (temp->right!=NULL)
            {
                count++;
                q.push(temp->right);
            }
            else
            {
                return count;
            }
        }
        return count;
    }
    void write(Node* root)
    {
        f.open("database.txt",ios::out);
        if (!f)
        {
            f.close();
            ofstream database("database.txt");
            database.close();
            f.open("database.txt",ios::out);
        }
        queue<Node*> q;
        q.push(root);
        f<<root->email<<"|"<<root->password<<"|"<<root->amount<<endl;
        while(!q.empty())
        {
            Node* temp=q.front();
            q.pop();
            if(temp->left!=NULL)
            {
                f<<temp->left->email<<"|"<<temp->left->password<<"|"<<temp->left->amount<<endl;
                q.push(temp->left);
            }
            else
            {
                break;
            }
            if (temp->right!=NULL)
            {
                f<<temp->right->email<<"|"<<temp->right->password<<"|"<<temp->right->amount<<endl;
                q.push(temp->right);
            }
            else
            {
                break;
            }
        }
        f.close();
    }
    void twrite(Node* root)
    {
        f.open("transactions.txt",ios::out);
        if (!f)
        {
            f.close();
            ofstream database("transactions.txt");
            database.close();
            f.open("transactions.txt",ios::out);
        }
        queue<Node*> q;
        q.push(root);
//        f<<root->email<<"|"<<root->password<<"|"<<root->amount<<endl;
        for (int i = 0; i < root->transactions.size(); ++i)
        {
            f<<root->transactions[i]<<"|";
        }
        f<<endl;
        while(!q.empty())
        {
            Node* temp=q.front();
            q.pop();
            if(temp->left!=NULL)
            {
//                f<<temp->left->email<<"|"<<temp->left->password<<"|"<<temp->left->amount<<endl;
                for (int i = 0; i < temp->left->transactions.size(); ++i)
                {
                    f<<temp->left->transactions[i]<<"|";
                }
                f<<endl;
                q.push(temp->left);
            }
            else
            {
                break;
            }
            if (temp->right!=NULL)
            {
//                f<<temp->right->email<<"|"<<temp->right->password<<"|"<<temp->right->amount<<endl;
                for (int i = 0; i < temp->right->transactions.size(); ++i)
                {
                    f<<temp->right->transactions[i]<<"|";
                }
                f<<endl;
                q.push(temp->right);
            }
            else
            {
                break;
            }
        }
        f.close();
    }
private:
    fstream f;

    struct Node* createNode()
    {
//    struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));

        Node* newNode=new Node;
        newNode->left=NULL;
        newNode->right=NULL;
        return newNode;
    }
};

class server{
public:
    binaryTree binaryTree;
    Node* root;
    server();
    Node* checkcredentials(string email,string password);
private:
    Node* cunode;
    Node* recnode;
    string email;
    string password;
    int amount;
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[MAX];
    int n;
    string convertToString(char* buffer);
    void main();
    void login();
    int checkAdmin(string email,string password);
    void menu();
    void withdraw();
    void deposit();
    void transfer();
    void transactions();
    void admin();
    void showAll();
    void reg();
    int mysend(SOCKET s,const char* buf);
    int myrecv(SOCKET s,string *buf);
    int count=0;
};
#endif //BANKSERVER_SERVER_H
