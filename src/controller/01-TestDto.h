#ifndef TESTDTO_H
#define TESTDTO_H

#include <StandardDefines.h>

_Entity
class TestDto {

    _Id
    int xx;

    Public optional<int> a;

    optional<int> b;

    optional<StdString> c;
};

#endif