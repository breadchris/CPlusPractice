template<class Object>
class LList;

template<class Object>
class LListItr;

template<class Object>
class LListNode
{

    LListNode(const Object & element = Object(), LListNode * n = nullptr ):element( element ),next( n ){}

    
    Object element;
    LListNode * next;
    
    friend class LList<Object>;
    friend class LListItr<Object>;

};

template<class Object>
class LListItr
{
public:
    LListItr():current(nullptr){}
    
    LListItr  & operator++();
    LListItr  operator++(int);
    
    bool operator==(const LListItr & rhs);
    bool operator!=(const LListItr & rhs);
    
    Object & operator*();

private:
    LListItr ( LListNode<Object> * p ):current( p ){}

    LListNode<Object> * current;
    
    friend class LList<Object>;
};

template<class Object>
class LList
{
public:
    typedef LListItr<Object> iterator; // I added this typedef
    
    LList ( ):header( new LListNode<Object>() ){ }
    
// Methods discussed in the class - you may remove these as they are duplicated below
    LListItr<Object> zeroth()
    { return LListItr<Object>( header ); }
    LListItr<Object> first() 
    {return LListItr<Object>(header->next);}

// Using the STL method names for the methods above
    LListItr<Object> before_begin()
    { return LListItr<Object>( header ); }
    LListItr<Object> begin()
    { return LListItr<Object>( header->next ); }
    LListItr<Object> end()
    { return LListItr<Object>( LListItr<Object>( nullptr ) ); }
    
    void push_front( const Object & x );
    
    bool empty() { return header->next == nullptr; }
    
    int size();
    
    void pop_front();

    template<class Predicate>// This is how you create a template method within a class
    void remove_if( Predicate pred );

    LListItr<Object> erase_after( LListItr<Object> itr );
 

    
private:
    LListNode<Object> * header;
};

template<class Object> template< class Predicate> /* The signature for when you define the template method outside the class*/
void LList<Object>::remove_if( Predicate pred)
{


}
