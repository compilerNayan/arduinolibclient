#ifndef ARDUINO
#include "tests/AllTests.h"
#include "IHttpRequestManager.h"

#include "ISpringBootCppApp.h"


/* @Autowired */
ISpringBootCppAppPtr springBootCppApp;

// Main function - runs the HTTP server loop
int main(int argc, char* argv[]) {

    RunAllTestSuites(argc, argv);

    springBootCppApp->StartApp();

    while(true) {
        springBootCppApp->ListenToRequest();
    }

    return 0;
}

#endif // ARDUINO

