#ifndef ARDUINO
#include "tests/RestTests.h"
#include <string>
#include <iostream>

// Main function - runs all test suites
int main(int argc, char* argv[]) {
    // Parse command line arguments for IP and port
    std::string ip = "localhost";
    std::string port = "8080";
    
    // Parse arguments: --ip <ip> --port <port> or -i <ip> -p <port>
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if ((arg == "--ip" || arg == "-i") && i + 1 < argc) {
            ip = argv[++i];
        } else if ((arg == "--port" || arg == "-p") && i + 1 < argc) {
            port = argv[++i];
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [--ip <ip>] [--port <port>]" << std::endl;
            std::cout << "  --ip, -i    Server IP address (default: localhost)" << std::endl;
            std::cout << "  --port, -p  Server port (default: 8080)" << std::endl;
            std::cout << "  --help, -h  Show this help message" << std::endl;
            return 0;
        }
    }
    
    // Run all test suites with IP and port
    RunAllRestTests(ip, port);

    return 0;
}

#endif // ARDUINO

