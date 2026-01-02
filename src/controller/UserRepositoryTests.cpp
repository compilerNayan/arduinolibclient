#ifndef ARDUINO
#include "UserRepositoryTests.h"
#include "IHttpRequestManager.h"

AUTOWIRED
IHttpRequestManagerPtr requestManager


// Main function - just calls RunAllTests
int main(int argc, char* argv[]) {
    RunAllTests(argc, argv);
    while(true) {
        requestManager->RetrieveRequest();
        requestManager->ProcessRequest();
    }
    return 0;
}

#endif // ARDUINO
