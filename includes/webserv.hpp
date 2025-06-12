/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpyciarz <jpyciarz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:55:41 by pmilek            #+#    #+#             */
/*   Updated: 2025/06/12 11:01:39 by jpyciarz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// c
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// c++
#include <iostream>
#include <cstring>
#include <map>
#include <sstream>
#include <fstream>
#include <utility>
#include <vector>

struct LocationConfig {
	std::string path;
	std::vector<std::string> methods;
	std::string upload_path;
	std::vector<std::string> index;
	bool autoindex;
	std::pair<int, std::string> redirection;
	std::string cgi_extension;
	std::string cgi_path;

	LocationConfig() : autoindex(false), redirection(std::make_pair(0, "")) {}
};

struct ServerConfig {
	int port;
	std::string root;
	size_t client_max_body_size;
	std::map<int, std::string> error_pages;
	std::vector<LocationConfig> locations;
	std::vector<std::string> server_names;
};

#endif