/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilek <pmilek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:59:11 by pmilek            #+#    #+#             */
/*   Updated: 2025/06/11 15:55:47 by pmilek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "webserv.hpp"
#include <vector>
#include <string>
#include <map>
#include <utility>

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

class ConfigParser {
	private:
		std::string _filepath;
		ServerConfig _serverConfig;

	public:
		ConfigParser(const std::string &path);
		void parse();
		ServerConfig getServerConfig() const;
};

#endif
