/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpyciarz <jpyciarz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:59:11 by pmilek            #+#    #+#             */
/*   Updated: 2025/06/12 11:01:37 by jpyciarz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Webserv.hpp"

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
