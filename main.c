

#define _GNU_SOURCE
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <linux/limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <memory.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include "utils/myqueue.h"
#include "string.h"
#include "utils/http.h"

#define SERVERPORT 12345
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define THREAD_POOL_SIZE 20
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

pthread_t thread_pool[THREAD_POOL_SIZE];

typedef struct sockaddr_in SA_IN;

void *handle_connection(void *p_client_socket);
int check(int exp, const char *msg);
void sendResponse(char * buffer, int client_socket,char * header,FILE *fp);
int parse(const char *line);
void *thread_function(void *arg);

// int GetFunction(int client_socket);
// void PostFunction(int client_socket);
// void PutFunction(int client_socket);
// void HeadFunction(int client_socket);
// void DeleteFunction(int client_socket);
// void UnknownFunction();

int main()
{
    int server_socket;
    int client_socket;
    int addr_size;
    SA_IN server_addr;
    SA_IN client_addr;

    //creating a bunch of thread to handle the future connections
    for (int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_create(&(thread_pool[i]), NULL, thread_function, NULL);
    }

    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
          "Failed to create socket");

    //initialize the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVERPORT);

    check(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)),
          "Bind failed");

    check(listen(server_socket, SOMAXCONN),
          "Listen Failed");

    while (true)
    {
        printf("waiting for connections...\n");
        // wait for and eventually accept an incoming connection

        addr_size = sizeof(SA_IN);
        check(client_socket =
                  accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size),
              "accept failed");

        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;

        pthread_mutex_lock(&mutex);
        enqueue(pclient);
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&mutex);
        /*int lp = pthread_create(&t, NULL, handle_connection, pclient);
        handle_connection(void *p_client_socket);*/
        printf("\n");
    }
    return 0;
}

int check(int exp, const char *msg)
{
    if (exp < 0)
        perror(msg);
    return exp;
}

void *thread_function(void *arg)
{
    while (true)
    {
        int *pclient;

        pthread_mutex_lock(&mutex);

        if ((pclient = dequeue()) == NULL)
        {
            pthread_cond_wait(&condition_var, &mutex);

            pclient = dequeue();
        }

        pthread_mutex_unlock(&mutex);

        if (pclient != NULL)
        {
            handle_connection(pclient);
        }
    }
}

void *handle_connection(void *p_client_socket)
{

    int client_socket = *((int *)p_client_socket);
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualPath[PATH_MAX + 1];
    char file_name[100];
    int a = 0;
    // read the client's message -- the path and name of the file to read
    while ((bytes_read = read(client_socket, buffer + msgsize, sizeof(buffer) - msgsize - 1)) > 0)
    {
        msgsize += bytes_read;
        //printf("msg size : %d \n", (int)bytes_read);
        if (buffer[msgsize - 1] == '\n' || msgsize > BUFSIZE - 1)
        {
            break;
        }
    }
    //http_request httprq;
    check(bytes_read, "recv error");
    buffer[msgsize - 1] = 0;

    fprintf(stdout, "%s\n", buffer);
    
    //extraction de method et nom de fichier
    fill_request(buffer);    

    if(strcmp(httprq.http_method,"GET")==0){
        char path[] = "client/files/";
        char *file = strcat(path, httprq.absolute_url);
        FILE *fp = fopen(file, "r");
        if (fp == NULL)
        {
            printf("ERROR(open): %s\n", file);
            fp = fopen("client/files/unknown.html", "r");
            //close(client_socket);
            
        }        
        
        memset(buffer,0,BUFSIZE);  
        char * header = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\n\n"; 
        sendResponse(buffer,client_socket,header,fp);
        // strcat(buffer,header );
        // char *endbuf = strchr(buffer,'\0');
        // //strcat(buffer,he)
        // //read file contents and send them to client
        // while ((bytes_read = fread(endbuf, 1, 4096-strlen(buffer), fp)) > 0)
        // {
        //     printf("sending %zu bytes :\n", bytes_read);
        //     //strcat(buffer,tmp);
        //     write(client_socket, buffer, bytes_read+strlen(header));
        // }
        // fclose(fp);  
        // close(client_socket); 
        // printf("closing connection\n");
    
    }
    else if(strcmp(httprq.http_method, "POST")==0){
        char path[] = "client/files/";
        char *file = strcat(path, "post.html");
        FILE *fp = fopen(file, "r");
        if (fp == NULL)
        {
            printf("ERROR(open): %s\n", file);
            fp = fopen("client/files/unknown.html", "r");
            //close(client_socket);            
        }        
        
        memset(buffer,0,BUFSIZE);  
        char * header = "HTTP/1.1 201 Created\nContent-Type: text/html; charset=UTF-8\n\n"; 
        sendResponse(buffer,client_socket,header,fp);

        //PostFunction(client_socket);
    }
    else if(strcmp(httprq.http_method, "HEAD")==0){
        //HeadFunction(client_socket);
        char path[] = "client/files/";
        char *file = strcat(path, "post.html");
        FILE *fp = fopen(file, "r");
        if (fp == NULL)
        {
            printf("ERROR(open): %s\n", file);
            fp = fopen("client/files/unknown.html", "r");
            //close(client_socket);            
        }        
        
        memset(buffer,0,BUFSIZE);  
        char * header = "HTTP/1.1 200 Created\nContent-Type: text/html; charset=UTF-8\n\n"; 
        sendResponse(buffer,client_socket,header,fp);
        
    }
    else if(strcmp(httprq.http_method, "PUT")==0){
        //PutFunction(client_socket);
        char path[] = "client/files/";
        char *file = strcat(path, "put.html");
        FILE *fp = fopen(file, "r");
        if (fp == NULL)
        {
            printf("ERROR(open): %s\n", file);
            fp = fopen("client/files/unknown.html", "r");
            //close(client_socket);            
        }        
        
        memset(buffer,0,BUFSIZE);  
        char * header = "HTTP/1.1 200 Created\nContent-Type: text/html; charset=UTF-8\n\n"; 
        sendResponse(buffer,client_socket,header,fp);
    }
    else if(strcmp(httprq.http_method, "DELETE")==0){
        DeleteFunction(client_socket);
    }else
    {
        char path[] = "client/files/";
        char *file = strcat(path, "unknown.html");
        FILE *fp = fopen(file, "r");
        if (fp == NULL)
        {
            printf("ERROR(open): %s\n", file);
            fp = fopen("client/files/unknown.html", "r");
            //close(client_socket);
            
        }        
        
        memset(buffer,0,BUFSIZE);  
        char * header = "HTTP/1.1 404 Not Found\nContent-Type: text/html; charset=UTF-8\n\n"; 
        sendResponse(buffer,client_socket,header,fp);
    }
    
    
            close(client_socket);
    //char *buf = buffer;
    // char * b = actualpath();
    // if( realpath(buffer, actualPath) == NULL){
    //     printf("Error(bad path): %s", buffer);
    //     close(client_socket);
    //     return NULL;
    // }
    
    
    //close(client_socket);
    
    //printf("sclosing connection\n");

    return 0;
}

void sendResponse(char * buffer, int client_socket,char * header,FILE *fp){
    strcat(buffer,header );
        char *endbuf = strchr(buffer,'\0');
        size_t bytes_read;
        //strcat(buffer,he)
        //read file contents and send them to client
        while ((bytes_read = fread(endbuf, 1, 4096-strlen(buffer), fp)) > 0)
        {
            printf("sending %zu bytes :\n", bytes_read);
            //strcat(buffer,tmp);
            write(client_socket, buffer, bytes_read+strlen(header));
        }
        fclose(fp);  
        close(client_socket); 
        printf("closing connection\n");
}