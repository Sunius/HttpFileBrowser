#include "PrecompiledHeader.h"
#include "AssetDatabase.h"
#include "ClientServerConnection.h"
#include "FileBrowserResponseHandler.h"
#include "HttpServer.h"
#include "TcpListener.h"
#include "TcpClient.h"
#include "WSAInitializer.h"

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WSAInitializer winSockContext;

	AssetDatabase::Initialize();

	TcpClient::Connect(L"Google.com", &ClientServerConnection::Create);
	/*
	TcpListener::Run(1337, 0, [](SOCKET incomingSocket, sockaddr_in clientAddress)
	{
		HttpServer::StartServiceClient(incomingSocket, clientAddress, &FileBrowserResponseHandler::ExecuteRequest);
	});
	*/
	return 0;
}