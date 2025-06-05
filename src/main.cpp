#include "../includes/Server.hpp"
#include "../includes/Webserv.hpp"
#include "../includes/HttpRequestParse.hpp"

int main()
{
	Server server;
	server.setupSocket();
	server.run();

	// std::string raw =
	// 	"POST /upload HTTP/1.1\r\n"
	// 	"Host: localhost:8080\r\n"
	// 	"Content-Type: application/json\r\n"
	// 	"Custom-Header: test-value\r\n"
	// 	"\r\n"
	// 	"{\n"
	// 	"  \"username\": \"jakub\",\n"
	// 	"  \"action\": \"upload\",\n"
	// 	"  \"file\": \"data.txt\"\n"
	// 	"}";

	// HttpRequestParse request(raw);
	// request.parseRequest();
	return 0;
}