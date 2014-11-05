#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>
#include "Entry.h"
#include "Position.h"

namespace BrooklynPoly {
    class Directory {
    public:
        Directory()

        Directory(const Directory& dir)

        ~Directory()

        Directory& operator=(const Directory& dir)

        friend ostream& operator<< (ostream& os, const Directory& dir)

        void add(const std::string& name, unsigned room, unsigned ph, Position& pos)

        unsigned operator[] (const string& name)

    private:	
        Entry** entries;
        size_t size;
        size_t capacity;
    };
}
#endif
