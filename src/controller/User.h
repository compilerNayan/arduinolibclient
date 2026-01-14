#ifndef USER_H
#define USER_H

#include <StandardDefines.h>

/* @Entity */
class User {

    /* @Id */
    /* @NotNull */
    Public optional<int> id;

    Public optional<StdString> name;
};

#endif

