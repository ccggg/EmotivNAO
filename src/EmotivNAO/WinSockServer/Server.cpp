#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <Winsock2.h>
#include <string>
#include <iostream>

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
				std::cout << "Message from Client: ";
				std::cout << index;
				std::cout << " >> ";
				std::cout << buffer << std::endl;
				continue;
			}

			//Send message to other clients.
			send(Connections[i], buffer, sizeof(buffer), NULL);
		}
	}
	closesocket(Connections[index]);
}

void ServerMessageThread() {
	char Msg[256];
	std::string rawInput;
	while (true) {
		std::getline(std::cin, rawInput);

		int length = rawInput.length();

		//Convert string to char array
		sprintf(Msg, rawInput.c_str());

		for (int i = 0; i < ConnectionCounter; i++) {
			//if(i == 0)
				//std::cout << Msg << std::endl;

			send(Connections[i], Msg, length, NULL);
		}
	}
}

int main() {

	std::cout << "STARTING SERVER!" << std::endl;

	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	// If WSAStartup returns anything other than 0 then there is an error.
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
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

	std::cout << "WAIT FOR CONNECTION" << std::endl;

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
		if (newConnection == 0) {
			std::cout << "FAILED CONNECTION!" << std::endl;
		} else {
			std::cout << "SUCCESSFUL CONNECTION!" << std::endl;
			
			//This is Windows OS only, if running on another OS remove this line.
			system("cls");

			std::cout << "Type a message to send to clients!" << std::endl;
			//char Msg[256] = "Message from the server to the client.";
			//send(newConnection, Msg, sizeof(Msg), NULL);
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