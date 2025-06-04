#include "../includes/HttpRequestParse.hpp"

// constructors/destructor
HttpRequestParse::HttpRequestParse(const std::string &rawRequest) : _rawRequest(rawRequest), _path(""), _httpVersion(""), _body("") {}

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
	}
	return *this;
}

HttpRequestParse::~HttpRequestParse() {}


// methods
void HttpRequestParse::parseRequest()
{
	std::istringstream stream(_rawRequest);
	std::string line;
	int i = 0;
	while (std::getline(stream, line))
	{
		std::cout << "line " << i << ": " << line << std::endl;
		i++;
	}
}