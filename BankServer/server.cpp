#include "server.h"

using namespace std;

server::server()
{
    cout<<"This is server";
    WSAData wsaData;
    char *ip="127.0.0.1";
    int port=5566;
    WSAStartup(MAKEWORD(2,2),&wsaData);
    server_sock=socket(AF_INET,SOCK_STREAM,0);
    if(server_sock<0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");
    memset(&server_addr,'\0',sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=port;
    server_addr.sin_addr.s_addr=inet_addr(ip);
    n=bind(server_sock,(struct sockaddr*)&server_addr, sizeof(server_addr));
    if(n<0)
    {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bing to the por nubmer : %d\n",port);
    listen(server_sock,5);
    printf("Listening...\n");
//    while(1)
//    {
        addr_size=sizeof(client_addr);
        client_sock= accept(server_sock,(struct sockaddr*)&client_addr,&addr_size);
        printf("[+]Client connected.\n");
        main();

//        printf("Client : %s\n",buffer);
//        memset(buffer,'\0',MAX);
//        strcpy(buffer,"Hi , This is server.");
//        send(client_sock,buffer,sizeof(buffer),0);
//        close(client_sock);
//        printf("[+]Client disconnected.\n\n");
//    }
}
void server::main()
{
    root=binaryTree.reload();
    string temp;
    myrecv(client_sock,&temp);
    if(temp=="Login")
    {
        login();
    }
    else if(temp=="Register")
    {
        reg();
    }
    else if(temp=="Exit")
    {
        close(client_sock);
        printf("[+]Client disconnected.\n\n");
        exit(0);
    }
}
void server::login()
{
    myrecv(client_sock,&email);
    myrecv(client_sock,&password);
    cunode=checkcredentials(email, password);
    if(cunode!=NULL)
    {
        mysend(client_sock,"1");
        menu();
    }
    else
    {
        mysend(client_sock,"0");
        if(checkAdmin(email,password))
        {
            mysend(client_sock,"1");
            admin();
            exit(0);
        }
        else
        {
            mysend(client_sock,"0");
        }
        count++;
        if(count>3)
        {
            main();
            exit(0);
        }
        login();
    }
}
int server::checkAdmin(string email,string password)
{
    if(email=="admin@gmail.com" && password=="admin")
    {
        return 1;
    }
    return 0;
}
void server::menu()
{
    string temp;
    mysend(client_sock, to_string(cunode->amount).c_str());
    myrecv(client_sock,&temp);
    if(temp=="Return")
    {
        main();
        exit(0);
    }
    else if(temp=="Transfer")
    {
        myrecv(client_sock,&email);
        if(email==root->email)
        {
            recnode=root;
            mysend(client_sock,"1");
            goto Here;
        }
        queue<Node*> q;
        q.push(root);
        while(!q.empty())
        {
            Node* temp=q.front();
            q.pop();
            if(temp->left!=NULL)
            {
                if(email==temp->left->email)
                {
                    recnode=temp->left;
                    mysend(client_sock,"1");
                    break;
                }
                q.push(temp->left);
            }
            else
            {
                mysend(client_sock,"0");
                menu();
                return;
            }
            if (temp->right!=NULL)
            {

                if(email==temp->right->email)
                {

                    mysend(client_sock,"1");
                    recnode=temp->right;
                    break;
                }
                q.push(temp->right);
            }
            else
            {
                mysend(client_sock,"0");
                menu();
                return;
            }
        }
    }
    else if(temp=="Transactions")
    {
        transactions();
        menu();
        exit(0);
    }
    Here:
    string strforamount;
    myrecv(client_sock,&strforamount);
    amount=stoi(strforamount);
    if(temp=="Withdraw")
    {
        withdraw();
    }
    else if(temp=="Deposit")
    {
        deposit();
    }
    else if(temp=="Transfer")
    {
        transfer();
    }
    else if(temp=="Return")
    {
        main();
        exit(0);
    }
}
void server::withdraw()
{
    cunode->amount=cunode->amount-amount;
    auto now=chrono::system_clock::now();
    time_t now_time=chrono::system_clock::to_time_t(now);
    string time_str=ctime(&now_time);
    cunode->transactions.push_back(cunode->email+" have withdrew "+to_string(amount)+"$ from their account at "+time_str.erase(time_str.size() - 1)+".");
    binaryTree.write(root);
    binaryTree.twrite(root);
    menu();
}
void server::deposit()
{
    cunode->amount=cunode->amount+amount;
    auto now=chrono::system_clock::now();
    time_t now_time=chrono::system_clock::to_time_t(now);
    string time_str=ctime(&now_time);
    cunode->transactions.push_back(cunode->email+" have deposited "+to_string(amount)+"$ to their account at "+time_str.erase(time_str.size() - 1)+".");
    binaryTree.write(root);
    binaryTree.twrite(root);
    menu();
}
void server::transfer()
{
    cunode->amount=cunode->amount-amount;
    recnode->amount=recnode->amount+amount;
    auto now=chrono::system_clock::now();
    time_t now_time=chrono::system_clock::to_time_t(now);
    string time_str=ctime(&now_time);
    cunode->transactions.push_back(cunode->email+" have transferred "+to_string(amount)+"$ to the recipient's account "+recnode->email+" at "+time_str.erase(time_str.size() - 1)+".");
    recnode->transactions.push_back(recnode->email+" have received "+to_string(amount)+"$ from "+cunode->email+" at "+time_str.erase(time_str.size() - 1)+".");
    binaryTree.write(root);
    binaryTree.twrite(root);
    menu();
}
void server::transactions()
{
    mysend(client_sock,to_string(cunode->transactions.size()).c_str());
    for (int i = 0; i < cunode->transactions.size(); ++i)
    {
        mysend(client_sock,cunode->transactions[i].c_str());
    }
}
void server::admin()
{
    string temp;
    myrecv(client_sock,&temp);
    if(temp=="Show All")
    {
        showAll();
    }
    else if(temp=="Show Specific")
    {

    }
    else if(temp=="Manage")
    {

    }
    else if(temp=="Return")
    {
        main();
        exit(0);
    }
}
void server::showAll()
{
//    int count=binaryTree.sizeofbinaryTree(root);
//    mysend(client_sock,to_string(count).c_str());
    if(root->email.empty())
        mysend(client_sock,"Stop");
    queue<Node*> q;
    q.push(root);
    mysend(client_sock,root->email.c_str());
    mysend(client_sock,to_string(root->transactions.size()).c_str());
    for (int i = 0; i < root->transactions.size(); ++i)
    {
        mysend(client_sock,root->transactions[i].c_str());
    }
    while(!q.empty())
    {
        Node* temp=q.front();
        q.pop();
        if(temp->left!=NULL)
        {
            mysend(client_sock,temp->left->email.c_str());
            mysend(client_sock,to_string(temp->left->transactions.size()).c_str());
            for (int i = 0; i < temp->left->transactions.size(); ++i)
            {
                mysend(client_sock,temp->left->transactions[i].c_str());
            }
            q.push(temp->left);
        }
        else
        {
            mysend(client_sock,"Stop");
            break;
        }
        if (temp->right!=NULL)
        {
            mysend(client_sock,temp->right->email.c_str());
            mysend(client_sock,to_string(temp->right->transactions.size()).c_str());
            for (int i = 0; i < temp->right->transactions.size(); ++i)
            {
                mysend(client_sock,temp->right->transactions[i].c_str());
            }
            q.push(temp->right);
        }
        else
        {
            mysend(client_sock,"Stop");
            break;
        }
    }
}
void server::reg()
{
    myrecv(client_sock,&email);
    myrecv(client_sock,&password);
    string temp;
    myrecv(client_sock,&temp);
    int amount= stoi(temp);
    Node* node=binaryTree.newNode(root);
    node->email=email;
    node->password=password;
    node->amount=amount;
    binaryTree.write(root);
}
Node* server::checkcredentials(string email, string password)
{
//    if(email=="HNM"&&  password=="123")
//    {
//        return 1;
//    }
    if(email==root->email && password==root->password)
    {
//        cunode=root;
        return root;
    }
    queue<Node*> q;
    q.push(root);
    while(!q.empty())
    {
        Node* temp=q.front();
        q.pop();
        if(temp->left!=NULL)
        {
            if(email==temp->left->email && password==temp->left->password)
            {
//                cunode=temp->left;
                return temp->left;
//                return 1;
            }
            q.push(temp->left);
        }
        else
        {
            break;
        }
        if (temp->right!=NULL)
        {

            if(email==temp->right->email && password==temp->right->password)
            {
//                cunode=temp->left;
                return temp->right;
            }
            q.push(temp->right);
        }
        else
        {
            break;
        }
    }
    return NULL;
}
string server::convertToString(char* buffer)
{
    int i;
    string s="";
    for (int i = 0; i < strlen(buffer); ++i)
    {
        s=s+buffer[i];
    }
    return s;
}

int server::mysend(SOCKET s,const char *buf)
{
    char buffer[MAX];
    memset(buffer,'\0',MAX);
    strcpy(buffer,buf);
    printf("Server : %s\n",buffer);
    return send(s,buffer,strlen(buffer),0);
}

int server::myrecv(SOCKET s,string *buf)
{
    char buffer[MAX];
    memset(buffer,'\0',MAX);
    int temp=recv(s,buffer,sizeof(buffer),0);
    printf("Client : %s\n",buffer);
    *buf=convertToString(buffer);
    return temp;
}

