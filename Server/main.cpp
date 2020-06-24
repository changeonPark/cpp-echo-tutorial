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

int main() {
  WSADATA wsaData; //Windows Socket API
  SOCKET serverSocket, clientSocket;
  SOCKADDR_IN serverAddress, clientAddress;

  int serverPort = 9876; //사용할 포트 선언
  char received[256]; // 주고 받을 문자 크기

  /*Winsock 초기화*/
  //WSA 2.2Ver(MAKEWORD). Startup된 결과를 wsaData에 넣음
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    showErrorMessage("WSAStartup()");
  }

  /*Server Socket 생성*/
  //PF_INET = Protocol Family
  serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  //소켓 예외 처리
  if (serverSocket == INVALID_SOCKET) {
    showErrorMessage("Socket()");
  }

  /*serverAddress를 0으로 초기화*/
  memset(&serverAddress, 0, sizeof(serverAddress));
  //htonl = 4byte 정수를 network byte 형식으로 (network long)
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  //AF_INET = Address Family(IPv4)
  serverAddress.sin_family = AF_INET;
  //htons = 2byte 정수를 network byte 형식으로 (network short)
  serverAddress.sin_port = htons(serverPort);

  /*binding*/
  if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
    showErrorMessage("bind()");
  }
  cout << "[Bind Complete]\n";

  /*listening*/
  //Client를 5개까지 수용하도록 설정 
  if (listen(serverSocket, 5) == SOCKET_ERROR) {
    showErrorMessage("listen()");
  }
  cout << "[Listen Complete]\n";

  /*accept*/
  int sizeClientAddress = sizeof(clientAddress);
  clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
  cout << "[Accept Complete]\n";

  /*Echo Message*/
  while (1) {
    //recv() = socket으로부터 데이터 수신하는 수, 반환값 = -1 이외: 실제 수신한 바이트 수, -1: fail
    int length = recv(clientSocket, received, sizeof(received), 0);
    //예상치 못한 문자열 잘라내기 위함
    received[length] = NULL;
    cout << "[Client Message]: " << received << endl;
    cout << "[Send Message]: " << received << endl;

    /*Client 측에서 종료할 경우*/
    if (strcmp(received, "[exit]") == 0) {
      //client가 enter를 통해 전송하므로 마지막 공백 문자 제거를 위해 sizeof - 1 해줌
      send(clientSocket, received, sizeof(received) - 1, 0);
      cout << "Shut Down\n";
      break;
    }

    /*메시지를 입력하였을 경우*/
    send(clientSocket, received, sizeof(received) - 1, 0);
  }

  closesocket(clientSocket);
  closesocket(serverSocket);
  WSACleanup();
  return 0;
}