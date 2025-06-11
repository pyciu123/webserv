/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilek <pmilek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:55:37 by pmilek            #+#    #+#             */
/*   Updated: 2025/06/11 15:42:37 by pmilek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../includes/webserv.hpp"
#include "../includes/HttpRequestParse.hpp"
#include "../includes/HttpResponse.hpp"
#include "../includes/ConfigParser.hpp"  // ⬅️ dodaj to

class Server {
	private:
		int server_fd;
		int port;
		sockaddr_in address;
		ServerConfig config; // ⬅️ przechowujemy dane z parsera

		int acceptConnection();
		std::string generateResponse(const HttpRequestParse &req);
		void sendResponse(int client_id, const std::string &response);
		std::string reciveRequest(int client_fd);

	public:
		Server(const ServerConfig &cfg);  // ⬅️ nowy konstruktor
		~Server();

		// methods
		void setupSocket();
		void run();

		// getters
		int getPort();
};

#endif
