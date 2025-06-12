#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include "webserv.hpp"

class CgiHandler
{
	private:
		std::string _scriptPath;
		std::map<std::string, std::string> _env;
		std::string _body;
	
	public:
    	CgiHandler(const std::string &path, const std::map<std::string, std::string> &env, const std::string &body);
		CgiHandler(const CgiHandler &copy);
		CgiHandler &operator=(const CgiHandler &other);
		~CgiHandler();
		std::string execute();
};

#endif