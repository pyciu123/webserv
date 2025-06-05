#include "../includes/HttpResponse.hpp"

// constuctors/destructors
HttpResponse::HttpResponse() : _status(200), _statusMessage("") {}

HttpResponse::HttpResponse(const HttpResponse &copy)
{
	*this = copy;
}

HttpResponse &HttpResponse::operator=(const HttpResponse &other)
{
	if (this != &other)
	{
		_status = other._status;
		_statusMessage = other._statusMessage;
	}
	return *this;
}

HttpResponse::~HttpResponse() {}


// private methods
bool HttpResponse::_exists(const std::string &path)
{
	if (FILE *file = fopen(path.c_str(), "r"))
		return fclose(file), true;
	else
		return false;
}

std::string HttpResponse::_readFile(const std::string &path)
{
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file:" + path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();
	return content;
}

std::string HttpResponse::_handleGet(const HttpRequestParse &req)
{
	std::string filePath = "www" + req.getPath();
	if (!_exists(filePath))
		return "HTTP/1.1 404 Not Found\r\n\r\n404 Not Found";

	std::string body = _readFile(filePath);

	std::ostringstream response;
	response << "HTTP/1.1 200 OK\r\n";
	response << "Content-Type: text/html\r\n";
	response << "Content-Length: " << body.size() << "\r\n";
	response << "Connection: close\r\n";
	response << "\r\n";
	response << body;

	return response.str();
}


//methods
std::string HttpResponse::buildResponse(const HttpRequestParse &req)
{
	std::string response;
	if (req.getMethod() == "GET")
		response = HttpResponse::_handleGet(req);
	else if (req.getMethod() == "POST")
		response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 4\r\n\r\nPOST";
	else if (req.getMethod() == "DELETE")
		response = response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 6\r\n\r\nDELETE";
	else
		response =  "HTTP/1.1 405 Method Not Allowed\r\n\r\nMethod No Allowed";
	return response;
}