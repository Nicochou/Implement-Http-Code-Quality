#ifndef HTTP_H
#define	HTTP_H

// typedef enum   {
//     HTTP_GET,
//     HTTP_HEAD,
//     HTTP_PUT,
//     HTTP_POST,
//     HTTP_DELETE,
//     HTTP_GET,
//     HTTP_UNSUPPORTED
// } http_method;



typedef struct  {
    char http_method[6];
    int bad_request;
    int major_version;
    int minor_version;
    char * content_type;
    char *url;
    char *absolute_url;
    char filepath[100];
    char *extension;
    char *status;
    char *content_length;
} http_request;


http_request httprq;

void fill_request(char * request);
void getFunction(int client_socket);
void PostFunction(int client_socket);
void PutFunction(int client_socket);
void HeadFunction(int client_socket);
void DeleteFunction(int client_socket);
void UnknownFunction();

#endif	/* HTTP_H */

