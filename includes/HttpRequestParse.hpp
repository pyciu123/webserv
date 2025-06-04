#ifndef HTTPREQUESTPARSE
# define HTTPREQUESTPARSE

#include "Webserv.hpp"

class HttpRequestParse {
	private:
		std::string _rawRequest;
		std::string	_method;
		std::string _path;
		std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		std::string _body;
	
	public:
		HttpRequestParse(const std::string &rawRequest);
		HttpRequestParse(const HttpRequestParse &copy);
		HttpRequestParse &operator=(const HttpRequestParse &other);
		~HttpRequestParse();

		// methods
		void parseRequest();
};

#endif