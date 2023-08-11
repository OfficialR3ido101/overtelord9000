#ifndef IRC_CLIENT_H
#define IRC_CLIENT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class IRCClient {
public:
  IRCClient(string server, int port, string nickname);
  void send(string message);
  void receive();

private:
  string server;
  int port;
  string nickname;
  auto socket;
};

#endif
