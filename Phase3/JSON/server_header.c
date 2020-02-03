
#include <winsock2.h>
#include <stdio.h>

#define MAX 8000
#define PORT 12345
#define SA struct sockaddr

int server_socket, client_socket;
char __buffer[MAX];

//*************************************************************************************************
//************************************* Make_Socket  Function *************************************
//*************************************************************************************************
int make_socket() {
    struct sockaddr_in server, client;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else {
//        printf("Socket successfully created..\n");
    }

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else {
//        printf("Socket successfully bound..\n");
    }

    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else {
        printf("Server listening..\n");
    }

    // Accept the data packet from client and verify
    int len = sizeof(client);
    client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else {
//        printf("Server acccepted the client..\n");
    }

    closesocket(server_socket);

    return client_socket;
 }
//*************************************************************************************************
//*************************************** Receive  Function ***************************************
//*************************************************************************************************
char* _recv() {

    make_socket();

    memset(__buffer, 0, sizeof(__buffer));

    recv(client_socket, __buffer, sizeof(__buffer), 0);
    return __buffer;
}
//*************************************************************************************************
//***************************************** Send Function *****************************************
//*************************************************************************************************
void _send (const char *format, ...)
{
    char message[MAX];
    memset(message, 0, MAX);


    va_list arg;
    va_start (arg, format);
    vsnprintf(message, MAX, format, arg);

    send(client_socket, message, sizeof(message), 0);
}
