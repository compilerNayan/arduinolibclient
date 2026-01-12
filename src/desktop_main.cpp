#ifndef ARDUINO
#include "tests/AllTests.h"
#include "IHttpRequestManager.h"
#include "controller/06-MyEntityRepository.h"
#include "http_client/ISpecialHttpClient.h"

/// @Autowired
ISpecialHttpClientPtr specialHttpClient;

/// @Autowired
IHttpRequestManagerPtr requestManager;

/// @Autowired
MyEntityRepositoryPtr myEntityRepository;

// Main function - runs all test suites
int main(int argc, char* argv[]) {
    // Run all test suites
    RunAllTestSuites(argc, argv);

    Var response = specialHttpClient->Get("https://api.example.com/data");
    std_println("response: ");
    std_println(response.c_str());

    Var myEntity = MyEntity();
    myEntity.id = 1;
    myEntity.username = "test";
    myEntity.password = "test";
    myEntityRepository->Save(myEntity);

    Var myEntity2 = myEntityRepository->FindById(1);
    std_println("myEntity2: ");
    std_println(myEntity2->Serialize().c_str());


    while(true) {
        requestManager->RetrieveRequest();
        requestManager->ProcessRequest();
        requestManager->ProcessResponse();
    }
    return 0;
}

#endif // ARDUINO

