#include  "../includes/Server.hpp"

// constructors, destructors, =operator
Server::Server() : server_fd(-1), port(8080) {}

Server::Server(const Server &copy)
{
    *this = copy;
}

Server &Server::operator=(const Server &other)
{
    this->server_fd = other.server_fd;
    return *this;
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

std::string Server::handleRequest()
{
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 14\r\n"
            "\r\n"
            "chuj ci w dupe";
    return response;
}

void Server::sendResponse(int client_id, const std::string &response)
{
    write(client_id, response.c_str(), response.length());
}


// public methods
void Server::setupSocket()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        std::cerr << "socked failed" << std::endl;
        return ;
    }
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0)
    {
        std::cerr << "bind failed" << std::endl;
        return ;
    }
    if (listen(server_fd, 10) < 0)
    {
        std::cerr << "listen" << std::endl;
        return ;
    }
    std::cout << "Server running at http://localhost:" << port << std::endl;
}

void Server::run()
{
    int client_id;
    std::string response;
    while (true)
    {
        client_id = Server::acceptConnection();
        if (client_id < 0)
            continue;
        else
        {
            response = Server::handleRequest();
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