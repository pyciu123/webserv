/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpyciarz <jpyciarz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:55:33 by pmilek            #+#    #+#             */
/*   Updated: 2025/07/21 10:07:21 by jpyciarz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include "../includes/HttpRequestParse.hpp"
#include "../includes/Webserv.hpp"
#include "../includes/CgiHandler.hpp"

class HttpResponse
{
	private:
		ServerConfig _config;
		int _status;
		std::string _statusMessage;
		std::string _handleCgi(const HttpRequestParse &req);
		bool _exists(const std::string &path);
		std::string _readFile(const std::string &path);
	
	public:
		HttpResponse(const ServerConfig &cfg);
		HttpResponse(const HttpResponse &copy);
		HttpResponse &operator=(const HttpResponse &other);
		~HttpResponse();

		// methods
		std::string buildResponse(const HttpRequestParse &req);

};

#endif