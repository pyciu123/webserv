#ifndef HTTPREQUESTPARSE
# define HTTPREQUESTPARSE

#include "../includes/webserv.hpp"

class HttpRequestParse {
	private:
		std::string _rawRequest;
		std::string	_method;
		std::string _path;
		std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		std::string _body;
		size_t _contentLength;
	
	public:
		HttpRequestParse(const std::string &rawRequest);
		HttpRequestParse(const HttpRequestParse &copy);
		HttpRequestParse &operator=(const HttpRequestParse &other);
		~HttpRequestParse();

		// methods
		void parseRequest();
		void printAll() const; // only for testin

		//getters
		std::string getMethod() const;
		std::string getPath() const;
		std::string getHttpVersion() const;
		std::map<std::string, std::string> getHeaders() const;
		std::string getBody() const;
		size_t getContentLength() const;
};

#endif