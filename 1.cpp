// Copyright 2025 Miroslav

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>

#define BUFFER_SIZE 512

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: ./client <server_ip> <port>\n";
    return 1;
  }

  const char* server_ip = argv[1];
  int port = std::stoi(argv[2]);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    return 1;
  }

  sockaddr_in server_addr {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
    std::cerr << "Invalid address/ Address not supported\n";
    return 1;
  }

  if (connect(sock, reinterpret_cast<sockaddr*>(&server_addr),
              sizeof(server_addr)) < 0) {
    perror("connect");
    return 1;
  }

  std::string login, password;
  std::cout << "Login: ";
  std::getline(std::cin, login);
  std::cout << "Password: ";
  std::getline(std::cin, password);

  std::string message = login + " " + password;

  send(sock, message.c_str(), message.length(), 0);

  char buffer[BUFFER_SIZE] = {0};
  int valread = recv(sock, buffer, BUFFER_SIZE - 1, 0);
  if (valread > 0) {
    buffer[valread] = '\0';
    std::cout << "Server response: " << buffer;
  } else {
    std::cout << "No response from server.\n";
  }

  close(sock);
  return 0;
}
