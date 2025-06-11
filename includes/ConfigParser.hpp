/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilek <pmilek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:59:11 by pmilek            #+#    #+#             */
/*   Updated: 2025/06/11 15:33:10 by pmilek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "webserv.hpp"
#include <vector>
#include <string>
#include <map>

struct LocationConfig {
	std::string path;
	std::vector<std::string> methods;
	std::string upload_path;
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
