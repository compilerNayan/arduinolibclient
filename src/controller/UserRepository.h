#ifndef _USER_REPOSITORY_H_
#define _USER_REPOSITORY_H_

#include <StandardDefines.h>
#include "CpaRepository.h"
#include "User.h"

/// @Repository
DefineStandardPointers(UserRepository)
class UserRepository : public CpaRepository<User, int> {
    Public Virtual ~UserRepository() = default;
    
    // Additional custom repository methods can be added here
    // The base CRUD methods are inherited from CpaRepository
};

#endif // _USER_REPOSITORY_H_

