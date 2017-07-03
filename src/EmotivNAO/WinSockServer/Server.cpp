#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <Winsock2.h>
#include <iostream>

// Only 1 connection is required as there is only 1 robot
SOCKET Connections[5];
int ConnectionCounter = 0;

int ClientHandlerThread(int index) {
	char buffer[256];
	while (true) {
		recv(Connections[index], buffer, sizeof(buffer), NULL);
		for (int i = 0; i < ConnectionCounter; i++) {
			//Don't need to send the message back to the same client.
			if (i == index)
				continue;

			//Send message to other clients.
			send(Connections[i], buffer, sizeof(buffer), NULL);
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
	for (int i = 0; i < 5; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
		if (newConnection == 0) {
			std::cout << "FAILED CONNECTION!" << std::endl;
		}
		else {
			std::cout << "SUCCESSFUL CONNECTION!" << std::endl;
			char Msg[256] = "This is a message from the server to the Client.";
			send(newConnection, Msg, sizeof(Msg), NULL);
			Connections[i] = newConnection;
			ConnectionCounter++;
		}
	}
	system("pause");
	return 0;
}