#include "myqueue.h"
#include <stdlib.h>

node_t * head = NULL;
node_t * tail = NULL;

void enqueue(int *client__socket){
    node_t *newnode = malloc(sizeof(node_t));
    newnode->client_socket = client__socket;
    if(tail == NULL)
        head = newnode;
    else
    {
        tail->next = newnode;
    }
    tail = newnode;
    
}

int * dequeue(){
    if(tail == NULL)
        return NULL;
    else
    {
        int* result = head->client_socket;
        node_t *temp = head;        
        head = head->next;
        if(head == NULL) tail = NULL;
        free(temp);
        return result;
        
    }
    
}