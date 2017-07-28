#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <Winsock2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "GyroData.h"
#include "FacialExpression.h"
#include "MentalCommands.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

extern int startGyro();
extern int startExpression();
extern void startMental(int selectedOption);

using namespace std;

// Only 1 connection is required as there is only 1 robot
SOCKET Connections[100];
int ConnectionCounter = 0;

void ClientHandlerThread(int index) {
	char buffer[256];
	while (true) {
		int num = recv(Connections[index], buffer, sizeof(buffer), NULL);
		for (int i = 0; i < ConnectionCounter; i++) {
			
			buffer[num] = '\0';
			if (buffer[num - 1] == '\n')
				buffer[num - 1] = '\0';

			//Don't need to send the message back to the same client.
			if (i == index) {
				cout << "Message from Client: ";
				cout << index;
				cout << " >> ";
				cout << buffer << endl;
				continue;
			}

			//Send message to other clients.
			send(Connections[i], buffer, sizeof(buffer), NULL);
		}
	}
	closesocket(Connections[index]);
}

int xCoord = 0;
int yCoord = 0;

char currAction = 'n';
int currActionPower = 0;

void ServerMessageThread() {
	char Msg[256];
	string rawInput;
	while (true) {
		//string currentFaceCommand = faceCommand;
		cout << faceCommand << endl;

		int length = string((string(1, 'a') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
		sprintf(Msg, string(string(1, 'a') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());

		if (faceCommand == "n") {
			int length = string((string(1, 'n') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, string(string(1, 'n') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}
		else if (faceCommand == "y") {
			int length = string((string(1, 'y') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, string(string(1, 'y') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}
		else if (faceCommand == "t") {
			int length = string((string(1, 't') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, string(string(1, 't') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}
		else if (faceCommand == "f") {
			int length = string((string(1, 'f') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, string(string(1, 'f') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}
		else if (faceCommand == "s") {
			int length = string((string(1, 's') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, string(string(1, 's') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}
		else {
			int length = string((string(1, 'a') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, string(string(1, 'a') + "," + string(1, currAction) + "," + to_string(currActionPower) + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}

		//getline(cin, rawInput);
		Sleep(25);
		//int length = string((faceCommand + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();

		//Convert string to char array
		//sprintf(Msg, rawInput.c_str());
		//sprintf(Msg, string(faceCommand + "," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());

		for (int i = 0; i < ConnectionCounter; i++) {
			//if(i == 0)
				//cout << Msg << endl;

			send(Connections[i], Msg, length, NULL);
		}
	}
}

void UpdateValues() {
	while (true) {
		xCoord = xmax / 96;
		yCoord = ymax / 96;

		currAction = action;
		currActionPower = actionPower;

		//cout << xCoord << endl;
		//cout << yCoord << endl;
	}
}

void initServer(int option) {
	/* Create thread for Gyro */
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startGyro, NULL, NULL, NULL);

	/* Create thread for getting Gyro coordinates */
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpdateValues, NULL, NULL, NULL);

	/* Create thread for facial expression */
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startExpression, NULL, NULL, NULL);

	if (mentalCommandsOn) {
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startMental, (LPVOID)option, NULL, NULL);
	}

	cout << "STARTING SERVER!" << endl;

	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	// If WSAStartup returns anything other than 0 then there is an error.
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		cout << "STARTUP FAILED!" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	cout << "WAIT FOR CONNECTION" << endl;

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
		if (newConnection == 0) {
			cout << "FAILED CONNECTION!" << endl;
		} else {
			cout << "SUCCESSFUL CONNECTION!" << endl;

			cout << "Type a message to send to clients!" << endl;
			char Msg[256] = "head,0.0,0.0";
			send(newConnection, Msg, sizeof(Msg), NULL);
			Connections[i] = newConnection;
			ConnectionCounter++;

			//Create new thread to handle new client, each client has its own thread on the server.
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ServerMessageThread, (LPVOID)(i), NULL, NULL);
		}
	}
}

int main(int argc, char** argv) {
	int option = 0;
	string input;

	if (!mentalCommandsOn) {
		cout << "WARNING: Mental Commands are currently disabled.\nTo enable them set mentalCommandsOn to true in MentalCommands.cpp" << endl;
	}

	cout << "1. Run Server" << endl;
	cout << "2. Train Headset Mental Commands" << endl;
	cout << ">> ";

	getline(cin, input, '\n');
	option = atoi(input.c_str());

	if (option == 1) {
		initServer(option);
	} else if(option == 2) {
		if (mentalCommandsOn) {
			startMental(option);
		} else {
			cout << "Mental Commands are currently disabled" << endl;
		}
	}
}