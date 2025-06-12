#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include "../includes/HttpRequestParse.hpp"
#include "../includes/Webserv.hpp"

class HttpResponse
{
	private:
		int _status;
		std::string _statusMessage;
		std::string _handleGet(const HttpRequestParse &req);
		bool _exists(const std::string &path);
		std::string _readFile(const std::string &path);
	
	public:
		HttpResponse();
		HttpResponse(const HttpResponse &copy);
		HttpResponse &operator=(const HttpResponse &other);
		~HttpResponse();

		// methods
		std::string buildResponse(const HttpRequestParse &req);

};

#endif