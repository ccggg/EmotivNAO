#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <Winsock2.h>
#include <string>
#include <iostream>
#include "GyroData.h"
#include "FacialExpression.h"

extern int startGyro();
extern int startExpression();

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

void ServerMessageThread() {
	char Msg[256];
	string rawInput;
	while (true) {
		//string currentFaceCommand = faceCommand;
		cout << faceCommand << endl;

		int length = string(("a," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
		sprintf(Msg, string("a," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());

		if (faceCommand == "n") {
			int length = std::string(("n," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, std::string("n," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}
		else if (faceCommand == "y") {
			int length = std::string(("y," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, std::string("y," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}
		else if (faceCommand == "t") {
			int length = std::string(("t," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, std::string("t," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}
		else if (faceCommand == "f") {
			int length = std::string(("f," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, std::string("f," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}
		else if (faceCommand == "s") {
			int length = std::string(("s," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, std::string("s," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
		}
		else {
			int length = std::string(("a," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str()).length();
			sprintf(Msg, std::string("a," + to_string(xCoord) + "," + to_string(yCoord) + "|").c_str());
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

void GyroCoordinate() {
	while (true) {
		xCoord = xmax / 96;
		yCoord = ymax / 96;

		//cout << xCoord << endl;
		//cout << yCoord << endl;
	}
}

int main(int argc, char** argv) {
	/* Create thread for Gyro */
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startGyro, NULL, NULL, NULL);

	/* Create thread for getting Gyro coordinates */
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GyroCoordinate, NULL, NULL, NULL);

	/* Create thread for facial expression */
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startExpression, NULL, NULL, NULL);

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

	system("pause");
	return 0;
}