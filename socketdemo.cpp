// socketdemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <WinSock2.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA wsaData;
	int port = 5099;

	char buf[] = "Server: hello, I am a server.....";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock");
		return 0;
	}

	//创建用于监听的套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port); //1024以上的端口号
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
	if (retVal == SOCKET_ERROR) {
		printf("Failed bind:%d\n", WSAGetLastError());
		return 0;
	}

	if (listen(sockSrv, 10) == SOCKET_ERROR) {
		printf("Listen failed:%d", WSAGetLastError());
		return 0;
	}

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		//等待客户请求到来	
		SOCKET sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);
		if (sockConn == SOCKET_ERROR) {
			printf("Accept failed:%d", WSAGetLastError());
			break;
		}

		printf("Accept client IP:[%s]\n", inet_ntoa(addrClient.sin_addr));

		//发送数据
		int iSend = send(sockConn, buf, sizeof(buf), 0);
		if (iSend == SOCKET_ERROR) {
			printf("send failed");
			break;
		}

		char recvBuf[100];
		memset(recvBuf, 0, sizeof(recvBuf));
		// 		//接收数据
		recv(sockConn, recvBuf, sizeof(recvBuf), 0);
		printf("%s\n", recvBuf);

		closesocket(sockConn);
	}

	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
