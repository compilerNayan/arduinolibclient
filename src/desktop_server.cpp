#ifndef ARDUINO
#include "tests/AllTests.h"
//#include "IHttpRequestManager.h"

#include "ISpringBootApp.h"


/* @Autowired */
ISpringBootAppPtr springBootApp;

// Main function - runs the HTTP server loop
int main(int argc, char* argv[]) {

   // RunAllTestSuites(argc, argv);

    springBootApp->StartApp();
/*    while(true) {
        requestManager->RetrieveRequest();
        requestManager->ProcessRequest();
        requestManager->ProcessResponse();
    }*/

    while(true) {
        springBootApp->ListenToRequest();
    }

    return 0;
}

#endif // ARDUINO

