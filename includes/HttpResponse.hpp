/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilek <pmilek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:55:33 by pmilek            #+#    #+#             */
/*   Updated: 2025/06/11 14:55:34 by pmilek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include "../includes/HttpRequestParse.hpp"
#include "../includes/webserv.hpp"

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