#include "IRCClient.h"

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

IRCClient::IRCClient(string server, int port, string nickname) {
  this->server = server;
  this->port = port;
  this->nickname = nickname;
  this->socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->socket == -1) {
    cerr << "Failed to create socket" << endl;
    exit(1);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(server.c_str());

  if (connect(this->socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    cerr << "Failed to connect to server" << endl;
    exit(1);
  }

  this->send("NICK " + nickname);
  this->send("USER " + nickname + " 0 * :My IRC Client");

  cout << "Was able to connect to server at" << server.c_str() << endl;
}

void IRCClient::send(string message) {
  write(this->socket, message.c_str(), message.length());
}

void IRCClient::receive() {
  char buffer[1024];
  int bytes_received = read(this->socket, buffer, sizeof(buffer));
  if (bytes_received == -1) {
    cerr << "Failed to receive message" << endl;
    exit(1);
  }

  string message(buffer, bytes_received);

  if(message.starts_with("/")) {
    cout << "This is a command" << endl;
  }
}
