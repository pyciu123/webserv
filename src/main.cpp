#include "../includes/server.hpp"
#include "../includes/webserv.hpp"

int main()
{
	Server server;
	server.setupSocket();
	server.run();
}