#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

//���� �ڵ鸵 �Լ�
void showErrorMessage(string message) {
  cout << "[Error!]: " << message << endl;
  system("pause");
  exit(1);
}

int main() {
  WSADATA wsaData; //Windows Socket API
  SOCKET serverSocket, clientSocket;
  SOCKADDR_IN serverAddress, clientAddress;

  int serverPort = 9876; //����� ��Ʈ ����
  char received[256]; // �ְ� ���� ���� ũ��

  /*Winsock �ʱ�ȭ*/
  //WSA 2.2Ver(MAKEWORD). Startup�� ����� wsaData�� ����
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    showErrorMessage("WSAStartup()");
  }

  /*Server Socket ����*/
  //PF_INET = Protocol Family
  serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  //���� ���� ó��
  if (serverSocket == INVALID_SOCKET) {
    showErrorMessage("Socket()");
  }

  /*serverAddress�� 0���� �ʱ�ȭ*/
  memset(&serverAddress, 0, sizeof(serverAddress));
  //htonl = 4byte ������ network byte �������� (network long)
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  //AF_INET = Address Family(IPv4)
  serverAddress.sin_family = AF_INET;
  //htons = 2byte ������ network byte �������� (network short)
  serverAddress.sin_port = htons(serverPort);

  /*binding*/
  if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
    showErrorMessage("bind()");
  }
  cout << "[Bind Complete]\n";

  /*listening*/
  //Client�� 5������ �����ϵ��� ���� 
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
    //recv() = socket���κ��� ������ �����ϴ� ��, ��ȯ�� = -1 �̿�: ���� ������ ����Ʈ ��, -1: fail
    int length = recv(clientSocket, received, sizeof(received), 0);
    //����ġ ���� ���ڿ� �߶󳻱� ����
    received[length] = NULL;
    cout << "[Client Message]: " << received << endl;
    cout << "[Send Message]: " << received << endl;

    /*Client ������ ������ ���*/
    if (strcmp(received, "[exit]") == 0) {
      //client�� enter�� ���� �����ϹǷ� ������ ���� ���� ���Ÿ� ���� sizeof - 1 ����
      send(clientSocket, received, sizeof(received) - 1, 0);
      cout << "Shut Down\n";
      break;
    }

    /*�޽����� �Է��Ͽ��� ���*/
    send(clientSocket, received, sizeof(received) - 1, 0);
  }

  closesocket(clientSocket);
  closesocket(serverSocket);
  WSACleanup();
  return 0;
}