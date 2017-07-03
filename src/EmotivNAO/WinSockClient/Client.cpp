#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

SOCKET Connection;

void ClientThread() {
	char buffer[256];
	while (true) {
		recv(Connection, buffer, sizeof(buffer), NULL);
		std::cout << buffer << std::endl;
	}
}

int main() {
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	// If WSAStartup returns anything other than 0 then there is an error.
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		MessageBoxA(NULL, "CONNECTION FAILED", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	std::cout << "CONNECTED!" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);

	char buffer[256];
	while (true) {
		std::cin.getline(buffer, sizeof(buffer));
		send(Connection, buffer, sizeof(buffer), NULL);
	}

	system("pause");

	return 0;
}