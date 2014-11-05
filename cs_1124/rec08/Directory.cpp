#include <string>
#include <iostream>

#include "Directory.h"
#include "Entry.h"
#include "Position.h"

using namespace std;

namespace BrooklynPoly {
    Directory::Directory()
	: capacity(2), size(0), entries(new Entry*[2] )
    {
        // Should we do this?  What do you think?
        for (size_t i = 0; i < capacity; i++) {
            entries[i] = nullptr;
        } // for
    } // Directory()

    Directory::Directory(const Directory& dir) {
        cout << "Copy constructor for directory is called" << endl;
        size = dir.size;
        capacity = dir.capacity;
        entries = new Entry*[ capacity ];
        for (size_t i = 0; i < dir.capacity; i++) {
            entries[i] = dir.entries[i];
        }
    }

    Directory::~Directory() {
        cout << "Destructor for directory is called" << endl;
        delete [] entries;
    }

    Directory& operator=(const Directory& dir) {
        cout << "Assignment operator for directory is called" << endl;
        if (this != &dir) {
            if (size != dir.size) {
                delete [] entries;
                size = dir.size;
                entries = new Entry*[size];
            }

            for (size_t i = 0; i < size; i++) {
                entries[i] = dir.entries[i];
            }
        }

        return *this;
    }

    ostream& operator<< (ostream& os, const Directory& dir) {
        os << "Directory" << endl;
        for (size_t i = 0; i < dir.size; i++) {
            os << *(dir.entries[i]) << endl;
        }
        return os;
    }

    void add(const string& name, unsigned room, unsigned ph, Position& pos) {
        if( size == capacity )	{
            Entry** tmp = new Entry*[ capacity *= 2 ];

            for (size_t i = 0; i < capacity; i++) {
                tmp[i] = entries[i];
            }

            delete entries;
            entries = tmp;
        } // if
        entries[size] = new Entry(name, room, ph, pos);
        ++size;
    } // add

    unsigned operator[] (const string& name) {
        for (size_t i = 0; i < size; i++) {
            if (entries[i]->getName() == name) {
                return entries[i]->getNumber();
            }
        }

        return 0;
    }
}

