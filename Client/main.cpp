#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

//에러 핸들링 함수
void showErrorMessage(string message) {
  cout << "[Error!]: " << message << endl;
  system("pause");
  exit(1);
}

int main(void) {
  WSADATA wsaData; //Windows Socket API
  SOCKET clientSocket;
  SOCKADDR_IN serverAddress;

  int serverPort = 9876; // 포트 선언
  char received[256]; //주고 받을 문자 크기
  string sent;//서버로 보낼 message를 담을 string 변수

  /*Winsock 초기화*/
  //WSA 2.2Ver(MAKEWORD). Startup된 결과를 wsaData에 넣음
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    showErrorMessage("WSAStartup()");
  }

  /*Client Socket 생성*/
  //PF_INET = Protocol Family(IPv4)
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  //소켓 예외 처리
  if (clientSocket == INVALID_SOCKET) {
    showErrorMessage("Socket()");
  }

  /*serverAddress를 0으로 초기화*/
  memset(&serverAddress, 0, sizeof(serverAddress));
  //inet_addr = 내부적으로 매개변수를 문자열을 받는다 -- 127.0.0.1 = 본인 컴퓨터의 IP Address를 나타냄
  serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
  //AF_INET = Address Family(IPv4)
  serverAddress.sin_family = AF_INET;
  //htons = 2byte 정수를 network byte 형식으로 (network short)
  serverAddress.sin_port = htons(serverPort);

  /*connecting*/
  if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
    showErrorMessage("Connect()");
  }
  cout << "[Connect Complete]\n";

  /*Echo Message*/
  while (1) {
    cout << "[Send Message]: ";
    getline(cin, sent);
    //아무 문자를 입력하지 않았을 시
    if (sent == "") continue;

    send(clientSocket, sent.c_str(), sent.length(), 0);

    //recv() = socket으로부터 데이터 수신하는 수, 반환값 = -1 이외: 실제 수신한 바이트 수, -1: fail
    int length = recv(clientSocket, received, sizeof(received), 0);
    received[length] = '\0';

    /*종료 할 경우*/
    if (strcmp(received, "[exit]") == 0) {
      cout << "Shut Down\n";
      break;
    }

    /*메시지를 입력하였을 경우*/
    cout << "Server Message: " << received << endl;
  }

  closesocket(clientSocket);
  WSACleanup();
  return 0;
}