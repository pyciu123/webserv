/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpyciarz <jpyciarz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:55:37 by pmilek            #+#    #+#             */
/*   Updated: 2025/06/12 11:05:36 by jpyciarz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../includes/Webserv.hpp"
#include "../includes/HttpRequestParse.hpp"
#include "../includes/HttpResponse.hpp"
#include "../includes/ConfigParser.hpp"

class Server {
	private:
		int server_fd;
		int port;
		sockaddr_in address;
		ServerConfig config;

		int acceptConnection();
		std::string generateResponse(const HttpRequestParse &req);
		void sendResponse(int client_id, const std::string &response);
		std::string reciveRequest(int client_fd);

	public:
		Server(const ServerConfig &cfg);
		~Server();

		// methods
		void setupSocket();
		void run();

		// getters
		int getPort();
};

#endif
