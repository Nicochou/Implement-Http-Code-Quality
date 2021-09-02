#include "http.h"
#include "string.h"
#include <stdio.h>
#include <unistd.h>


void fill_request(char * request){
    char * saveptr1, *saveptr, *method ;
    strcpy(httprq.http_method,strtok_r(request, " ", &saveptr));    
    httprq.absolute_url = strtok_r(saveptr, " ", &saveptr1);
    
    printf("\n method :%s \n",httprq.http_method);
    printf("\n url : %s \n",httprq.absolute_url);
    // printf("buffer : \n%s\n", buffer);
    // printf("saved pointer : %s\n", saveptr);
    // msgsize = strlen(buffer);

}

void getFonction(int client_socket){
    char path[100] = "client/files/";
    char *file = strcat(path, httprq.absolute_url);
    //printf("%s is the file", file);
    size_t bytes_read;
    char buffer[4096];
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("ERROR(open): %s\n", httprq.absolute_url);
        close(client_socket);
        return;
    }

    //read file contents and send them to client
    while ((bytes_read = fread(buffer, 1, 4096, fp)) > 0)
    {
        printf("sending %zu bytes\n", bytes_read);
        write(client_socket, buffer, bytes_read);
    }
    fclose(fp);    
}

void PostFunction(int client_socket){

}

void HeadFunction(int client_socket){

}
void PutFunction(int client_socket){

}
void DeleteFunction(int client_socket){

}
void UnknownFunction();