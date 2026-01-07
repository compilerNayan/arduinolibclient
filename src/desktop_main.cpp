#ifndef ARDUINO
#include "controller/UserRepositoryTests.h"
#include "IHttpRequestManager.h"
#include "controller/06-MyEntityRepository.h"

/// @Autowired
IHttpRequestManagerPtr requestManager;

/// @Autowired
MyEntityRepositoryPtr myEntityRepository;

// Main function - just calls RunAllTests
int main(int argc, char* argv[]) {
    RunAllTests(argc, argv);

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

