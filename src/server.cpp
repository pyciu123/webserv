/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpyciarz <jpyciarz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:56:03 by pmilek            #+#    #+#             */
/*   Updated: 2025/06/12 11:03:03 by jpyciarz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// constructors, destructors, =operator
Server::Server(const ServerConfig &cfg) : config(cfg) {
	port = config.port;
	server_fd = -1;
}

Server::~Server() {}


// private methods
int Server::acceptConnection()
{
    socklen_t addrlen = sizeof(address);
    int client_fd = accept(server_fd, (sockaddr*)&address, &addrlen);
    if (client_fd < 0)
    {
        std::cerr << "failed to setup client_id" << std::endl;
        return 1;
    }
    return client_fd;
}

std::string Server::generateResponse(const HttpRequestParse &req)
{
    HttpResponse res;
	std::string response;
    response = res.buildResponse(req);
    return response;
}

void Server::sendResponse(int client_id, const std::string &response)
{
    write(client_id, response.c_str(), response.length());
}

std::string Server::reciveRequest(int client_fd)
{
    char buffer[1024];
    std::string request;
    ssize_t bytesRead;
    while ((bytesRead = read(client_fd, buffer, sizeof(buffer))) > 0)
    {
        request.append(buffer, bytesRead);
        if (request.find("\r\n\r\n") != std::string::npos)
            break ;
    }
    return request;
}

// public methods
void Server::setupSocket()
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		std::cerr << "socket failed" << std::endl;
		return;
	}

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
		std::cerr << "bind failed" << std::endl;
		return;
	}
	if (listen(server_fd, 10) < 0) {
		std::cerr << "listen failed" << std::endl;
		return;
	}

	std::cout << "Server running at http://localhost:" << port << std::endl;
}

void Server::run()
{
    int client_id;
    std::string response;
    std::string clientRequest;
    while (true)
    {
        client_id = Server::acceptConnection();
        if (client_id < 0)
            continue;
        else
        {
            clientRequest = Server::reciveRequest(client_id);
            HttpRequestParse req(clientRequest);
            req.parseRequest();
            response = Server::generateResponse(req);
            Server::sendResponse(client_id, response);
            close(client_id);
        }
    }
}

// getters/setters
int Server::getPort()
{
    return port;
}