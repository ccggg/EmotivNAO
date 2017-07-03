#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <Winsock2.h>
#include <iostream>

// Only 1 connection is required as there is only 1 robot
SOCKET Connections[100];
int ConnectionCounter = 0;

void ClientHandlerThread(int index) {
	int bufferLength;
	while (true) {
		recv(Connections[index], (char*)&bufferLength, sizeof(int), NULL);
		char * buffer = new char[bufferLength+1];
		recv(Connections[index], buffer, bufferLength, NULL);
		for (int i = 0; i < ConnectionCounter; i++) {
			//Don't need to send the message back to the same client.
			if (i == index)
				continue;

			//Send message to other clients.
			send(Connections[i], (char*)&bufferLength, sizeof(int), NULL);
			send(Connections[i], buffer, bufferLength, NULL);
		}
		delete[] buffer; //Clear allocated memory to avoid memory leak.
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
			std::string Msg = "This is a message from the server to the Client.";
			int MsgLength = Msg.size();
			send(newConnection, (char*)&MsgLength, sizeof(int), NULL);
			send(newConnection, Msg.c_str(), MsgLength, NULL);
			Connections[i] = newConnection;
			ConnectionCounter++;

			//Create new thread to handle new client, each client has its own thread on the server.
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL);
		}
	}

	system("pause");
	return 0;
}