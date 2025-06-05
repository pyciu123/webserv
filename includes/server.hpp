#ifndef SERVER_HPP
# define SERVER_HPP

#include "Webserv.hpp"
#include "HttpRequestParse.hpp"
#include "HttpResponse.hpp"

class Server {
	private:
		int server_fd;
		int port;
		sockaddr_in address;
		int acceptConnection();
		std::string generateResponse(const HttpRequestParse &req);
		void sendResponse(int client_id, const std::string &response);
		std::string reciveRequest(int client_fd);

	public:
		Server();
		Server(const Server &copy);
		Server &operator=(const Server &other);
		~Server();

		// methods
		void setupSocket();
		void run();
		
		// getters
		int getPort();
};

#endif