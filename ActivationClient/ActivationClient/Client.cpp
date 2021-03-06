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
bool		activation;			// variable to control activation loop

// Function to close the specified socket and perform DLL cleanup (WSACleanup)
void cleanup(SOCKET socket);

int provideInfo(string computerName) {


	// Loop to communicate with server until either side types "end"

	done = false;

	do
	{
		// SEND and then RECV

		// Always make sure there's a '\0' at the end of the buffer

		buffer[BUFFERSIZE - 1] = '\0';

		// Get a message from the user and attempt to send it to the server

		cout << "Send serial number and computerName(serialNumber computerName)> ";

		string input3;
		cin >> input3;


		iResult = send(mySocket, input3.c_str(), (int)input3.size() + 1, 0);

		if (iResult == SOCKET_ERROR)
		{
			cerr << "Send failed with error: " << WSAGetLastError() << endl;

			cleanup(mySocket);

			return 1;
		}

		// Communication ends if client sent an "end" message

		if (input == "end")

			done = true;

		if (!done)
		{
			// Wait to receive a reply message back from the server

			cout << "\n\t--WAIT--\n\n";

			// String to hold received message

			input = "";

			// Receive until server stops sending data

			moreData = false;

			do
			{
				iResult = recv(mySocket, buffer, BUFFERSIZE - 1, 0);
				cout << buffer << "2\n";
				if (iResult > 0)
				{
					// Received data; need to determine if there's more coming                                                        

					if (buffer[iResult - 1] != '\0')
						moreData = true;
					else
						moreData = false;

					// Concatenate received data onto end of string we're building

					input = input + (string)buffer;
					if (input == GOODMSG) {
						return 2;
					}

					else {
						return -1;
					}

				}

				else if (iResult == 0)
				{
					cout << "Connection closed\n";

					cleanup(mySocket);

					return 0;
				}

				else
				{
					cerr << "Recv failed with error: " << WSAGetLastError() << endl;

					cleanup(mySocket);

					return 1;
				}

			} while (moreData);

			cout << "Recv > " << input << endl;

			if (input == GOODMSG) {
				return 2;
			}

			else {
				return -1;
			}

			done = true;
		}



	} while (!done);
}

inline bool isFile() {
	ifstream f(ACTIVATIONFILENAME);
	return f.good();
}
void printInfile(string CNum) {
	ofstream inFile;
	inFile.open(ACTIVATIONFILENAME, ios::out);
	inFile << CNum;
	inFile.close();

}
void checkFile(string comName) {
	if (isFile() == true) {
		ifstream inFile;
		inFile.open(ACTIVATIONFILENAME);
		string line;
		if (inFile.is_open())
		{
			while (getline(inFile, line))
			{
				cout << line << '\n';
				if (line != comName) {
					if (provideInfo(comName) == 2) {
						cout << "ava";
						printInfile(comName);
					}
					else {
						exit(0);
					}
				}
				else {
					exit(0);
				}
			}
			inFile.close();
		}

	}
	else {
		if (provideInfo(comName) == 2) {
			printInfile(comName);
			cout << "ava";
		}
		else {
			exit(0);
			cout << "noe";

		}
	}

}


int main(int argc, char* argv[])
{



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

	cout << "Enter MachineID (digits only)";
	string input2;
	cin >> input2;


	checkFile(input2);
	cout << "Enter MachineID (digits only)";
	
	cin >> input2;
		return 0;
}




void cleanup(SOCKET socket)
{
	if (socket != INVALID_SOCKET)
		closesocket(socket);

	WSACleanup();
}