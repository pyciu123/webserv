#include "../includes/Server.hpp"
#include "../includes/Webserv.hpp"
#include "../includes/HttpRequestParse.hpp"
#include "../includes/ConfigParser.hpp"

int main() {
    try {
        // Parsowanie pliku konfiguracyjnego
        ConfigParser parser("www/config.conf");
        parser.parse();
        ServerConfig config = parser.getServerConfig();

        // Wypisywanie sparsowanych danych (debug)
        std::cout << "Parsed Config:\n";
        std::cout << "Port: " << config.port << "\n";
        std::cout << "Root: " << config.root << "\n";
        std::cout << "Max body size: " << config.client_max_body_size << "\n";
        std::cout << "Server names:\n";
        for (size_t i = 0; i < config.server_names.size(); ++i)
            std::cout << "  " << config.server_names[i] << "\n";

        std::cout << "Error pages:\n";
        for (std::map<int, std::string>::iterator it = config.error_pages.begin(); it != config.error_pages.end(); ++it)
            std::cout << "  " << it->first << " => " << it->second << "\n";

        std::cout << "Locations:\n";
        for (size_t i = 0; i < config.locations.size(); ++i) {
            std::cout << "  Path: " << config.locations[i].path << "\n";
            std::cout << "    Methods: ";
            for (size_t j = 0; j < config.locations[i].methods.size(); ++j)
                std::cout << config.locations[i].methods[j] << " ";
            std::cout << "\n";
            std::cout << "    Upload path: " << config.locations[i].upload_path << "\n";
            std::cout << "    Index: ";
            for (size_t j = 0; j < config.locations[i].index.size(); ++j)
                std::cout << config.locations[i].index[j] << " ";
            std::cout << "\n";
            std::cout << "    Autoindex: " << (config.locations[i].autoindex ? "on" : "off") << "\n";
            std::cout << "    CGI extension: " << config.locations[i].cgi_extension << "\n";
            std::cout << "    CGI path: " << config.locations[i].cgi_path << "\n";
        }

        // Uruchomienie serwera z wczytaną konfiguracją
        Server server(config);
        server.setupSocket();
        server.run();

    } catch (const std::exception &e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
    }

    return 0;
}