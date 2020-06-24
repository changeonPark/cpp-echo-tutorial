#define _WINSOCK_DEPRECATED_NO_WARNINGS
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

int main(void) {
  WSADATA wsaData; //Windows Socket API
  SOCKET clientSocket;
  SOCKADDR_IN serverAddress;

  int serverPort = 9876; // ��Ʈ ����
  char received[256]; //�ְ� ���� ���� ũ��
  string sent;//������ ���� message�� ���� string ����

  /*Winsock �ʱ�ȭ*/
  //WSA 2.2Ver(MAKEWORD). Startup�� ����� wsaData�� ����
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    showErrorMessage("WSAStartup()");
  }

  /*Client Socket ����*/
  //PF_INET = Protocol Family(IPv4)
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  //���� ���� ó��
  if (clientSocket == INVALID_SOCKET) {
    showErrorMessage("Socket()");
  }

  /*serverAddress�� 0���� �ʱ�ȭ*/
  memset(&serverAddress, 0, sizeof(serverAddress));
  //inet_addr = ���������� �Ű������� ���ڿ��� �޴´� -- 127.0.0.1 = ���� ��ǻ���� IP Address�� ��Ÿ��
  serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
  //AF_INET = Address Family(IPv4)
  serverAddress.sin_family = AF_INET;
  //htons = 2byte ������ network byte �������� (network short)
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
    //�ƹ� ���ڸ� �Է����� �ʾ��� ��
    if (sent == "") continue;

    send(clientSocket, sent.c_str(), sent.length(), 0);

    //recv() = socket���κ��� ������ �����ϴ� ��, ��ȯ�� = -1 �̿�: ���� ������ ����Ʈ ��, -1: fail
    int length = recv(clientSocket, received, sizeof(received), 0);
    received[length] = '\0';

    /*���� �� ���*/
    if (strcmp(received, "[exit]") == 0) {
      cout << "Shut Down\n";
      break;
    }

    /*�޽����� �Է��Ͽ��� ���*/
    cout << "Server Message: " << received << endl;
  }

  closesocket(clientSocket);
  WSACleanup();
  return 0;
}