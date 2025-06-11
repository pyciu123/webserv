/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilek <pmilek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:59:21 by pmilek            #+#    #+#             */
/*   Updated: 2025/06/11 15:30:11 by pmilek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigParser.hpp"
#include <fstream>
#include <sstream>

ConfigParser::ConfigParser(const std::string &path) : _filepath(path) {}

void ConfigParser::parse() {
	std::ifstream file(_filepath.c_str());
	if (!file.is_open())
		throw std::runtime_error("Could not open config file");

	std::string line;
	LocationConfig currentLocation;
	bool inLocation = false;

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string key;
		iss >> key;

		if (key == "listen") {
			iss >> _serverConfig.port;
		} else if (key == "root") {
			iss >> _serverConfig.root;
		} else if (key == "client_max_body_size") {
			iss >> _serverConfig.client_max_body_size;
		} else if (key == "server_name") {
			std::string name;
		while (iss >> name)
			_serverConfig.server_names.push_back(name);
		} else if (key == "error_page") {

			int code;
			std::string path;
			iss >> code >> path;
			_serverConfig.error_pages[code] = path;
		} else if (key == "location") {
			inLocation = true;
			iss >> currentLocation.path;
		} else if (key == "methods" && inLocation) {
			std::string method;
			while (iss >> method)
				currentLocation.methods.push_back(method);
		} else if (key == "upload_path" && inLocation) {
			iss >> currentLocation.upload_path;
		} else if (key == "}") {
			if (inLocation) {
				_serverConfig.locations.push_back(currentLocation);
				currentLocation = LocationConfig();
				inLocation = false;
			}
		}
	}
}

ServerConfig ConfigParser::getServerConfig() const {
	return _serverConfig;
}
