
#include <winsock2.h>
#include <windows.h>

#define MAX 8000
#define PORT 12345
#define SA struct sockaddr

int client_socket;
//*************************************************************************************************
//************************************* Make_Socket  Function *************************************
//*************************************************************************************************
int make_socket ()
{
    int client_socket;
	struct sockaddr_in servaddr, cli;

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
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
//		printf("Socket successfully created..\n");

	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// Connect the client socket to server socket
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
	else
//		printf("Successfully connected to the server..\n");

    return client_socket;
}
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
void set_color (int k)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
void reset_color()
{
    set_color(7);
}
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
int cprintf (int k, const char *format, ...)
{
    va_list arg;
    int done;

    set_color(k);
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    reset_color();

   return done;
}
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
void cscanf (int k, const char *format, ...)
{
    va_list arg;
    int done;

    set_color(k);
    va_start (arg, format);
    printf("               \r");
    done = vfscanf (stdin, format, arg);
    va_end (arg);
    reset_color();

   return done;
}
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
void gotoline(int n) {
  COORD coord;
  coord.X = 0;
  coord.Y = n;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
char make_menu (char* title, int n, ...) {
    cprintf(176, " %s \n\n", title);

    va_list arg;
    va_start (arg, n);

    char* opt[n];
    for (int i = 1; i <= n; i++) {
        opt[i] = va_arg(arg, int);
        cprintf(11, "%d: %s\n", i, opt[i]);
    }
    va_end (arg);

    char inp;
    int line = 1;

    while (true) {
        gotoline(line + 1);
        inp = getch();

        if (inp >= '1' && inp <= '0' + n)// 1, 2, ... , n
            return inp;

        if (inp == ' ' || inp == 13)    // space or enter
            return line + '0';

        if (inp == -32) {               // some arrow key
            inp = getch();

            if (inp == 72 && line > 1)  // up arrow
                line--;

            if (inp == 80 && line < n)  // down arrow
                line++;
        }
    }
}
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
void _send (const char *format, ...)
{
    char message[MAX];

    va_list arg;
    va_start (arg, format);
    vsnprintf(message, MAX, format, arg);

    client_socket = make_socket();
    send(client_socket, message, sizeof(message), 0);
}
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
char* _recv ()
{
    static char message[MAX];
    memset(message, 0, MAX);
    recv(client_socket, message, sizeof(message), 0);
    return message;
}
