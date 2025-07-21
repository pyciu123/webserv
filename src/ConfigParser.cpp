#include "../includes/ConfigParser.hpp"
#include "../includes/Webserv.hpp"

ConfigParser::ConfigParser(const std::string &path) : _filepath(path) {}

void ConfigParser::parse() {
    std::ifstream file(_filepath.c_str());
    if (!file.is_open())
        throw std::runtime_error("Could not open config file");

    std::string line;
    LocationConfig currentLocation;
    bool inLocation = false;

    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t\r") + 1);

        if (line.empty() || line[0] == '#')
            continue;

        if (!line.empty() && line[line.size() - 1] == ';')
            line.erase(line.size() - 1);

        std::istringstream iss(line);
        std::string key;
        iss >> key;

        std::cerr << "Parsing line: " << line << ", key: " << key << std::endl;

        if (key == "listen") {
            iss >> _serverConfig.port;
            std::cerr << "Parsed listen: " << _serverConfig.port << std::endl;
        }
        else if (key == "root") {
            iss >> _serverConfig.root;
            if (!_serverConfig.root.empty() && _serverConfig.root[_serverConfig.root.size() - 1] == ';')
                _serverConfig.root.erase(_serverConfig.root.size() - 1);
            std::cerr << "Parsed root: " << _serverConfig.root << std::endl;
        }
        else if (key == "client_max_body_size") {
            iss >> _serverConfig.client_max_body_size;
            std::cerr << "Parsed client_max_body_size: " << _serverConfig.client_max_body_size << std::endl;
        }
        else if (key == "server_name") {
            std::string name;
            while (iss >> name) {

                if (!name.empty() && name[name.size() - 1] == ';')
                    name.erase(name.size() - 1);
                _serverConfig.server_names.push_back(name);
            }
            std::cerr << "Parsed server_names: ";
            for (size_t i = 0; i < _serverConfig.server_names.size(); ++i)
                std::cerr << _serverConfig.server_names[i] << " ";
            std::cerr << std::endl;
        }
        else if (key == "error_page") {
            int code;
            std::string path;
            iss >> code >> path;
            if (!path.empty() && path[path.size() - 1] == ';')
                path.erase(path.size() - 1);
            _serverConfig.error_pages[code] = path;
            std::cerr << "Parsed error_page: " << code << " => " << path << std::endl;
        }
        else if (key == "location") {
            inLocation = true;
            iss >> currentLocation.path;
            std::cerr << "Parsed location path: " << currentLocation.path << std::endl;
        }
        else if (key == "methods" && inLocation) {
            std::string method;
            while (iss >> method) {

                if (!method.empty() && method[method.size() - 1] == ';')
                    method.erase(method.size() - 1);
                currentLocation.methods.push_back(method);
            }
            std::cerr << "Parsed methods: ";
            for (size_t j = 0; j < currentLocation.methods.size(); ++j)
                std::cerr << currentLocation.methods[j] << " ";
            std::cerr << std::endl;
        }
        else if (key == "upload_path" && inLocation) {
            iss >> currentLocation.upload_path;
            if (!currentLocation.upload_path.empty() && currentLocation.upload_path[currentLocation.upload_path.size() - 1] == ';')
                currentLocation.upload_path.erase(currentLocation.upload_path.size() - 1);
            std::cerr << "Parsed upload_path: " << currentLocation.upload_path << std::endl;
        }
        else if (key == "index" && inLocation) {
            std::string file;
            while (iss >> file) {

                if (!file.empty() && file[file.size() - 1] == ';')
                    file.erase(file.size() - 1);
                currentLocation.index.push_back(file);
            }
            std::cerr << "Parsed index: ";
            for (size_t j = 0; j < currentLocation.index.size(); ++j)
                std::cerr << currentLocation.index[j] << " ";
            std::cerr << std::endl;
        }
        else if (key == "autoindex" && inLocation) {
            std::string val;
            iss >> val;
            if (!val.empty() && val[val.size() - 1] == ';')
                val.erase(val.size() - 1);
            currentLocation.autoindex = (val == "on");
            std::cerr << "Parsed autoindex: " << (currentLocation.autoindex ? "on" : "off") << std::endl;
        }
        else if (key == "return" && inLocation) {
            int code;
            std::string target;
            iss >> code >> target;
            if (!target.empty() && target[target.size() - 1] == ';')
                target.erase(target.size() - 1);
            currentLocation.redirection = std::make_pair(code, target);
            std::cerr << "Parsed redirection: " << code << " => " << target << std::endl;
        }
        else if (key == "cgi_extension" && inLocation) {
            iss >> currentLocation.cgi_extension;
            if (!currentLocation.cgi_extension.empty() && currentLocation.cgi_extension[currentLocation.cgi_extension.size() - 1] == ';')
                currentLocation.cgi_extension.erase(currentLocation.cgi_extension.size() - 1);
            std::cerr << "Parsed cgi_extension: " << currentLocation.cgi_extension << std::endl;
        }
        else if (key == "cgi_path" && inLocation) {
            iss >> currentLocation.cgi_path;
            if (!currentLocation.cgi_path.empty() && currentLocation.cgi_path[currentLocation.cgi_path.size() - 1] == ';')
                currentLocation.cgi_path.erase(currentLocation.cgi_path.size() - 1);
            std::cerr << "Parsed cgi_path: " << currentLocation.cgi_path << std::endl;
        }
        else if (key == "}") {
            if (inLocation) {
                _serverConfig.locations.push_back(currentLocation);
                std::cerr << "Added location: " << currentLocation.path << std::endl;
                currentLocation = LocationConfig();
                inLocation = false;
            }
        }
        else {
            std::cerr << "Unknown directive: " << key << std::endl;
        }
    }
    file.close();
}

ServerConfig ConfigParser::getServerConfig() const {
    return _serverConfig;
}