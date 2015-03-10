template <class Object>
class Vector
{
public:
    explicit Vector( int initSize = 0 )
    : theSize( initSize ), theCapacity( initSize + SPARE_CAPACITY )
    { objects = new Object[ theCapacity ]; }

    ~Vector( )
    { delete [ ] objects; }

    bool empty( ) const
    { return size( ) == 0; }

    int size( ) const
    { return theSize; }

    int capacity( ) const
    { return theCapacity; }

    Object & operator[]( int index )
    { return objects[ index ]; }

    // Iterator stuff: not bounds checked
    typedef Object * iterator;
    typedef const Object * const_iterator;

    iterator begin( )
    { return &objects[ 0 ]; }
    const_iterator begin( ) const
    { return &objects[ 0 ]; }

    iterator end( )
    { return &objects[ size( ) ]; }
    const_iterator end( ) const
    { return &objects[ size( ) ]; }


    const Object & operator[]( int index ) const
    { return objects[ index ]; }

    void reserve( int newCapacity );

    void push_back( const Object & x );

    iterator erase( iterator vItr) {
        if (!theSize) return;

        delete vItr;
        iterator endItr = end();
        iterator startItr = vItr;
        while (startItr != endItr) {
            *startItr = *(startItr + 1)
            startItr++;
        }
        return vItr;
    }

    Vector & operator=(const Vector & rhs );

    static const int SPARE_CAPACITY = 16;

private:
    int theSize;
    int theCapacity;
    Object * objects;
};

template <class Object>
void Vector<Object>::push_back( const Object & x )
{
    if( theSize == theCapacity )
        reserve( 2 * theCapacity + 1 );
    objects[ theSize++ ] = x;
}


template <class Object>
void Vector<Object>::reserve( int newCapacity )
{

    if ( newCapacity <= theCapacity ) return;
    // never decrease the capacity

    Object* p = new Object[ newCapacity];
    for( int k = 0; k < theSize; k++ )
        p[ k ] = objects[ k ];

    delete [ ] objects;
    objects = p;
    theCapacity = newCapacity;
}

template <class Object>
Vector<Object> & Vector<Object>::operator=( const Vector<Object> & rhs )
{
    if( this != &rhs )
    {
        delete [ ] objects;
        theSize = rhs.theSize;
        theCapacity = rhs.theCapacity;

        objects = new Object[ theCapacity( ) ];
        for( int k = 0; k < size( ); k++ )
            objects[ k ] = rhs.objects[ k ];
    }
    return *this;
}
