#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// Make sure build environment links to Winsock library file
#pragma comment(lib, "Ws2_32.lib")


// Define default global constants
#define BUFFERSIZE 256
#define DATAFILENAME "dataFile.txt"
#define IPADDRESS "127.0.0.1"
#define GOODMSG "good"
#define BADMSG "invalid"
#define DEFAULTPORT 6000


// Function to close the specified socket and perform DLL cleanup (WSACleanup)
void cleanup(SOCKET socket);

void chatLog ()
{
    ofstream log;
    log.open(DATAFILENAME);
}


int main(int argc, char* argv[])
{
    WSADATA		wsaData;			// structure to hold info about Windows
                                    // sockets implementation
    SOCKET		listenSocket;		// socket for listening for incoming
                                    // connections
    SOCKET		clientSocket;		// socket for communication with the client
    SOCKADDR_IN	serverAddr;			// structure to hold server address
                                    // information
    string		ipAddress;			// server's IP address
    string		input;				// generic input from user
    char		buffer[BUFFERSIZE];	// buffer to hold message received from
                                    // server
    int			port;				// server's port number
    int			iResult;			// resulting code from socket functions
    bool		done;				// variable to control communication loop
    bool		moreData;			// variable to control receive data loop

    cout << "\n*** CHAT SERVER ***\n" << endl;

    //Get server address information from user
    cout << "Enter IP address (press enter for localhost): "
    getline(cin, ipAddress);
    if (ipAddress == "")
        ipAddress = "127.0.0.1"

    cout << endl;
	// If user types in a port number on the command line use it, otherwise, use
	// the default port number
    cout << "Enter port number (press enter for default port): "
    getline(cin, input)
	if (port == "")
		port = DEFAULTPORT;
	else
	    port = stoi(input);


	

	return 0;
}


void cleanup(SOCKET socket)
{
	if (socket != INVALID_SOCKET)
		closesocket(socket);

	WSACleanup();
}


