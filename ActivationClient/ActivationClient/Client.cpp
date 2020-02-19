// Author:
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
#define ACTIVATIONFILENAME "actFile.txt"
#define IPADDRESS "127.0.0.1"
#define GOODMSG "good"
#define BADMSG "invalid"
#define DEFAULTPORT 6000


// Function to close the specified socket and perform DLL cleanup (WSACleanup)
void cleanup(SOCKET socket);


int main(int argc, char* argv[])
{
	WSADATA		wsaData;			// structure to hold info about Windows sockets implementation
	SOCKET		mySocket;			// socket for communication with the server
	SOCKADDR_IN	serverAddr;			// structure to hold server address information
	string		ipAddress;			// server's IP address
	string		input;				// generic input from user
	char		buffer[BUFFERSIZE];	// buffer to hold message received from server
	int			port;				// server's port number
	int			iResult;			// resulting code from socket functions
	bool		done;				// variable to control communication loop
	bool		moreData;			// variable to control receive data loop


	cout << "\n*** CHAT CLIENT ***\n\n";

	// Get server connection information from user
	cout << "Enter server IP address (press enter for localhost): ";
	getline(cin, ipAddress);
	if (ipAddress == "")
		ipAddress = "127.0.0.1";

	cout << endl;
	cout << "Enter server port number is " << DEFAULTPORT << endl;
	cout << "Press enter to accept port, or enter new port: ";
	getline(cin, input);
	if (input == "")
		port = DEFAULTPORT;
	else
		port = stoi(input);


	// WSAStartup loads WS2_32.dll (Winsock version 2.2) used in network programming
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		cerr << "WSAStartup failed with error: " << iResult << endl;
		return 1;
	}


	// Create a new socket for communication with the server
	mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mySocket == INVALID_SOCKET)
	{
		cerr << "Socket failed with error: " << WSAGetLastError() << endl;
		cleanup(mySocket);
		return 1;
	}

	cout << "\nAttempting to connect...\n";


	// Setup a SOCKADDR_IN structure which will be used to hold address
	// and port information for the server. Notice that the port must be converted
	// from host byte order to network byte order.
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);


	// Try to connect to server
	iResult = connect(mySocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (iResult == SOCKET_ERROR)
	{
		cerr << "Connect failed with error: " << WSAGetLastError() << endl;
		cleanup(mySocket);
		return 1;
	}

	cout << "Connected...\n\n";
	int port;

	// If user types in a port number on the command line use it,
	// otherwise, use the default port number
	if (argc > 1)
		port = atoi(argv[1]);
	else
		port = DEFAULTPORT; 
	
	

	return 0;
}


void cleanup(SOCKET socket)
{
	if (socket != INVALID_SOCKET)
		closesocket(socket);

	WSACleanup();
}