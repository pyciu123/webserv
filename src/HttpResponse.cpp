/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   HttpResponse.cpp								   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: jpyciarz <jpyciarz@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/06/11 14:55:52 by pmilek			#+#	#+#			 */
/*   Updated: 2025/07/21 10:11:21 by jpyciarz		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../includes/HttpResponse.hpp"

// constuctors/destructors
HttpResponse::HttpResponse(const ServerConfig &cfg) : _config(cfg), _status(200), _statusMessage("") {}

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
	std::ifstream file(path.c_str());
	if (!file.is_open())
		throw std::runtime_error("Failed to open file:" + path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();
	return content;
}

std::string HttpResponse::_handleCgi(const HttpRequestParse &req) {
    std::string path = req.getPath();
    std::string method = req.getMethod();

    std::cerr << "Checking CGI for path: " << path << ", method: " << method << std::endl;

    for (size_t i = 0; i < _config.locations.size(); ++i) {
        const LocationConfig &loc = _config.locations[i];

        std::cerr << "Location: " << loc.path << ", CGI extension: " << loc.cgi_extension << std::endl;

        bool methodAllowed = loc.methods.empty();
        for (size_t j = 0; j < loc.methods.size(); ++j) {
            if (loc.methods[j] == method) {
                methodAllowed = true;
                break;
            }
        }
        if (!methodAllowed) {
            std::cerr << "Method " << method << " not allowed for location " << loc.path << std::endl;
            continue;
        }

        if (path.find(loc.path) == 0 &&
            path.size() >= loc.cgi_extension.size() &&
            path.compare(path.size() - loc.cgi_extension.size(), loc.cgi_extension.size(), loc.cgi_extension) == 0) {
            std::map<std::string, std::string> env;
            env["REQUEST_METHOD"] = method;
            env["CONTENT_LENGTH"] = std::to_string(req.getContentLength());
            if (req.getHeaders().count("Content-Type"))
                env["CONTENT_TYPE"] = req.getHeaders().at("Content-Type");
            else
                env["CONTENT_TYPE"] = "text/plain";
            env["SCRIPT_NAME"] = path;
            env["QUERY_STRING"] = "";

            std::string interpreter = loc.cgi_path;
            std::string scriptPath = _config.root + path;
            std::string body = req.getBody();

            std::cerr << "Executing CGI: " << scriptPath << " with interpreter: " << interpreter << std::endl;
            std::cerr << "Body: " << body << std::endl;

            if (!_exists(scriptPath)) {
                std::cerr << "Script does not exist: " << scriptPath << std::endl;
                return "";
            }

            CgiHandler cgi(scriptPath, env, body, interpreter);
            std::string result = cgi.execute();

            std::cerr << "CGI result: " << result << std::endl;

            std::ostringstream response;
            response << "HTTP/1.1 200 OK\r\n";
            response << "Content-Type: text/plain\r\n"; // Zmiana na text/plain, aby pasowało do api.py
            response << "Content-Length: " << result.size() << "\r\n";
            response << "Connection: close\r\n\r\n";
            response << result;
            return response.str();
        }
    }
    std::cerr << "No matching CGI location found for path: " << path << std::endl;
    return "";
}

// Zmodyfikowana metoda buildResponse
std::string HttpResponse::buildResponse(const HttpRequestParse &req) {
	std::string response;

	// Obsługa CGI dla GET i POST
	if (req.getMethod() == "GET" || req.getMethod() == "POST") {
		response = _handleCgi(req);
		if (!response.empty()) {
			return response;
		}
	}

	// Standardowa obsługa GET (jeśli nie CGI)
	if (req.getMethod() == "GET") {
		std::string filePath = _config.root + req.getPath();
		if (_exists(filePath)) {
			std::string content = _readFile(filePath);
			std::ostringstream oss;
			oss << "HTTP/1.1 200 OK\r\n";
			oss << "Content-Type: text/html\r\n";
			oss << "Content-Length: " << content.size() << "\r\n";
			oss << "Connection: close\r\n\r\n";
			oss << content;
			return oss.str();
		} else {
			std::string msg = "404 Not Found";
			std::ostringstream oss;
			oss << "HTTP/1.1 404 Not Found\r\n";
			oss << "Content-Type: text/plain\r\n";
			oss << "Content-Length: " << msg.size() << "\r\n";
			oss << "Connection: close\r\n\r\n";
			oss << msg;
			return oss.str();
		}
	}
	// Obsługa POST (jeśli nie CGI)
	else if (req.getMethod() == "POST") {
		std::string msg = "POST request processed";
		std::ostringstream oss;
		oss << "HTTP/1.1 200 OK\r\n";
		oss << "Content-Type: text/plain\r\n";
		oss << "Content-Length: " << msg.size() << "\r\n";
		oss << "Connection: close\r\n\r\n";
		oss << msg;
		return oss.str();
	}
	// Obsługa DELETE
	else if (req.getMethod() == "DELETE") {
		std::string msg = "DELETE request processed";
		std::ostringstream oss;
		oss << "HTTP/1.1 200 OK\r\n";
		oss << "Content-Type: text/plain\r\n";
		oss << "Content-Length: " << msg.size() << "\r\n";
		oss << "Connection: close\r\n\r\n";
		oss << msg;
		return oss.str();
	}
	// Domyślna odpowiedź dla nieobsługiwanych metod
	else {
		std::string msg = "Method Not Allowed";
		std::ostringstream oss;
		oss << "HTTP/1.1 405 Method Not Allowed\r\n";
		oss << "Content-Type: text/plain\r\n";
		oss << "Content-Length: " << msg.size() << "\r\n";
		oss << "Connection: close\r\n\r\n";
		oss << msg;
		return oss.str();
	}
}