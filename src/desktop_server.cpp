#ifndef ARDUINO
#include "tests/AllTests.h"
#include "IHttpRequestManager.h"

/// @Autowired
IHttpRequestManagerPtr requestManager;

// Main function - runs the HTTP server loop
int main(int argc, char* argv[]) {

    RunAllTestSuites(argc, argv);


    while(true) {
        requestManager->RetrieveRequest();
        requestManager->ProcessRequest();
        requestManager->ProcessResponse();
    }
    return 0;
}

#endif // ARDUINO

