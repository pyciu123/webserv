#include "../includes/HttpRequestParse.hpp"

// constructors/destructor
HttpRequestParse::HttpRequestParse(const std::string &rawRequest) : _rawRequest(rawRequest), _path(""), _httpVersion(""), _body(""), _contentLength(-1) {}

HttpRequestParse::HttpRequestParse(const HttpRequestParse &copy)
{
	*this = copy;
}

HttpRequestParse &HttpRequestParse::operator=(const HttpRequestParse &other)
{
	if (this != &other)
	{
		_rawRequest = other._rawRequest;
		_path = other._path;
		_httpVersion = other._httpVersion;
		_headers = other._headers;
		_body = other._body;
		_method = other._method;
		_contentLength = other._contentLength;
	}
	return *this;
}

HttpRequestParse::~HttpRequestParse() {}


void HttpRequestParse::printAll() const
{
    std::cout << "Method: " << _method << std::endl;
    std::cout << "Path: " << _path << std::endl;
    std::cout << "HTTP Version: " << _httpVersion << std::endl;

    std::cout << "Headers:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
    {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }

    std::cout << "Body:\n" << _body << std::endl;
}

// methods
void HttpRequestParse::parseRequest()
{
	std::istringstream stream(_rawRequest);
	std::string line;
	bool isFirstLine = true;
	bool isHeaders = false;
	bool isBody = false;
	while (std::getline(stream, line))
	{
		line.erase(line.find_last_not_of("\r") + 1);

		if (isFirstLine) {
			std::istringstream iss(line);
			iss >> _method >> _path >> _httpVersion;
			if (_method.empty() || _path.empty() || _httpVersion.empty())
				throw std::runtime_error("400 Bad Request: invalid request line");

			isFirstLine = false;
			isHeaders   = true;
			continue;                // <-- KLUCZOWA LINIA
		}

		if (isHeaders) {
			if (line.empty()) {
				isHeaders = false;
				isBody    = true;
				continue;
			}

			size_t pos = line.find(':');
			if (pos == std::string::npos)
				throw std::runtime_error("400 Bad Request: invalid header format");

			std::string key   = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			value.erase(0, value.find_first_not_of(' '));
			_headers[key] = value;
			continue;
		}

		if (isBody)
			_body += line + '\n';
	}
}


// getters
std::string HttpRequestParse::getMethod() const
{
	return _method;
}

std::string HttpRequestParse::getPath() const
{
	return _path;
}

std::string HttpRequestParse::getHttpVersion() const
{
	return _httpVersion;
}

std::map<std::string, std::string> HttpRequestParse::getHeaders() const
{
	return _headers;
}

std::string HttpRequestParse::getBody() const
{
	return _body;
}

size_t HttpRequestParse::getContentLength() const
{
	return _contentLength;
}