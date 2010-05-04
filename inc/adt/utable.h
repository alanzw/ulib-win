#ifndef U_TABLE_H
#define U_TABLE_H

#include <iostream>
#include <cstring>
#include <cstdlib>

namespace huys
{

namespace ADT
{

template <typename K>
struct hash
{
    static int hash_value(const K &key)
    {
        unsigned int hash_value = 0;
        /*
        Compute the hash value from the first four bytes of the object's
        resident memory.  Then get the hash index for the array by taking
        the modulo of the size of that array (<number_buckets>).
        */
        memcpy( &hash_value, &key, sizeof( unsigned int ) );
        return hash_value;
   }
};

template <typename K, typename D, typename H = hash<K> >
class UTable;

template <typename K, typename D, typename H>
std::ostream & operator<< ( std::ostream &output, const UTable<K,D,H> &table );

template <typename K, typename D, typename H>
class UTable
{
private:
    class Bucket
    {
    public:
        K key;
        D data;

        Bucket *next;

        Bucket( const K &key, const D &data, Bucket *next = NULL );

    } **buckets;

    int number_buckets;

    int number_entries;

    bool check( bool condition, const char *message ) const;

    Bucket *findBucket( const K &key ) const;

    void grow();

    int hash( const K &key ) const;


    const static int DEFAULT_BUCKETS;

    const static double THRESHOLD;

public:
    UTable<K,D,H> &operator=( const UTable<K,D,H> &table );

    UTable<K,D,H>( int starting_buckets = DEFAULT_BUCKETS );

    ~UTable<K,D,H>();

    bool inTable( const K &key ) const;

    UTable<K,D,H> & add( const K &key, const D &data = 0 );

    D &operator[]( const K &key );

    UTable<K,D,H> & clear();

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
    friend std::ostream & operator<< ( std::ostream &output, const UTable<K,D,H> &table );	
#else
    friend std::ostream & operator<< <>( std::ostream &output, const UTable<K,D,H> &table );
#endif // (_MSC_VER <= 1200)

};

/*
  ISO C++ (98) standard requires that static double constants be
  defined outside of the main class declaration.  There is a bit of an
  art to determining the best THRESHOLD value, however, many Internet
  resources have suggested values in the range 0.7 to 0.8.  Here, we
  cut the range in the middle.  See the THRESHOLD declaration in class
  UTable for more information.
*/

template <typename K, typename D, typename H>
const int UTable<K,D,H>::DEFAULT_BUCKETS = 89;

template <typename K, typename D, typename H>
const double UTable<K,D,H>::THRESHOLD = 0.75;

template <typename K, typename D, typename H>
UTable<K,D,H> &UTable<K,D,H>::operator=( const UTable<K,D,H> &table )
{
    Bucket *current_bucket = NULL;

    this->clear();

    /*
        Iterate over all the buckets in <table>, copying over each entry.
    */
    for( int bucket_index = 0; bucket_index < table.number_buckets; bucket_index++ )
    {
        current_bucket = table.buckets[bucket_index];
        while( current_bucket != NULL )
        {
            add( current_bucket->key, current_bucket->data );
            current_bucket = current_bucket->next;
        }
    }

    return *this;
}

template <typename K, typename D, typename H>
int UTable<K,D,H>::hash( const K &key ) const
{
    return (int)(H::hash_value(key) % number_buckets);
}

template <typename K, typename D, typename H>
void UTable<K,D,H>::grow()
{
    /*
        grow() sizing algorithm has been modeled after that used in the
        built in Java hash table.  C++ style memory management is
        insufficient, here.
    */
    number_buckets = 2*number_buckets + 1;
    buckets = (Bucket **)realloc( buckets, (sizeof( Bucket *) * number_buckets) );
}

template <typename K, typename D, typename H>
UTable<K,D,H>::~UTable()
{
    int bucket_index = 0;
    Bucket *current_bucket = NULL;
    Bucket *next_bucket = NULL;

    for ( bucket_index = 0; bucket_index < number_buckets; bucket_index++ )
    {
        current_bucket = buckets[bucket_index];
        while ( current_bucket != NULL )
        {
            next_bucket = current_bucket->next;
            free( current_bucket );
            current_bucket = next_bucket;
        }
    }

    free( buckets );
}

template <typename K, typename D, typename H>
UTable<K,D,H>::UTable( int starting_buckets )
: number_buckets( starting_buckets ), number_entries( 0 )
{
    /*
        Set up the list of Bucket pointers.  The use of calloc ensures
        that each Bucket pointer will be initially set to NULL.
    */
    buckets = (Bucket **)calloc( sizeof( Bucket * ), number_buckets );
}

template <typename K, typename D, typename H>
UTable<K,D,H> &UTable<K,D,H>::add( const K &key, const D &data )
{
    check( !inTable( key ), "Key already in Table." );

    if( ((double)number_entries / number_buckets) >= THRESHOLD )
    {
        grow();
    }
    int hash_value = hash( key );

    number_entries++;

    Bucket *new_bucket = (Bucket *)malloc(sizeof(Bucket) );
    ::ZeroMemory(new_bucket, sizeof(Bucket));
    new_bucket->key = key;
    new_bucket->data = data;

    /*
        If there are already buckets in the bucket list for the given hash
        value, prepend the new one to the existing list.  Regardless,
        update the Bucket pointer in the hash array.
    */
    if( buckets[hash_value] != NULL )
    {
         new_bucket->next = buckets[hash_value];
    }
    else
    {
        new_bucket->next = NULL;
    }

    buckets[hash_value] = new_bucket;

    return *this;
}

template <typename K, typename D, typename H>
bool UTable<K,D,H>::check( bool condition, const char *message ) const
{
    if ( !condition )
    {
        std::cerr << "ERROR: " << message << std::endl;
        //exit( EXIT_FAILURE );
        return false;
    }
    return true;
}

template <typename K, typename D, typename H>
bool UTable<K,D,H>::inTable( const K &key ) const
{
    return findBucket( key ) != NULL;
}

template <typename K, typename D, typename H>
typename UTable<K,D,H>::Bucket *UTable<K,D,H>::findBucket( const K &key ) const
{
    int hash_value = hash( key );
    Bucket *current_bucket = buckets[hash_value];

    while( current_bucket != NULL )
    {
        if( current_bucket->key == key )
            return current_bucket;
        current_bucket = current_bucket->next;
    }

    return NULL;
}

template <typename K, typename D, typename H>
UTable<K,D,H> &UTable<K,D,H>::clear()
{
    int bucket_index;

    for ( bucket_index = 0; bucket_index < number_buckets; bucket_index++ )
    {
        Bucket *current_bucket = buckets[bucket_index];
        while ( current_bucket != NULL )
        {
            Bucket *next_bucket = current_bucket->next;
            free(current_bucket);
            current_bucket = next_bucket;
        }
        buckets[bucket_index] = NULL;
    }

    /*
      Reset the table to the default size.  This is not necessary, but
      is courteous from a memory management standpoint.
    */
    number_entries = 0;
    number_buckets = DEFAULT_BUCKETS;
    buckets = (Bucket **)realloc( buckets, (sizeof( Bucket *) * number_buckets) );

    return *this;
}

template <typename K, typename D, typename H>
std::ostream &operator<<( std::ostream &output, const UTable<K,D,H> &table )
{
    int bucket_index;
    typename UTable<K,D>::Bucket *current_bucket = NULL;

    for ( bucket_index = 0; bucket_index < table.number_buckets; bucket_index++ )
    {
        current_bucket = table.buckets[bucket_index];
        while ( current_bucket != NULL )
        {
            output << "["  << current_bucket->key
                << ", " << current_bucket->data << "]\n";
            current_bucket = current_bucket->next;
        }
    }

    return output;
}

template <typename K, typename D, typename H>
D & UTable<K,D,H>::operator[]( const K &key )
{
    Bucket *found_bucket = findBucket( key );
    check( found_bucket != NULL, "Key not in Table.");

    if (NULL == found_bucket)
    {
        add(key);
        return findBucket(key)->data;
    }

    return found_bucket->data;
}

template <typename K, typename D, typename H>
UTable<K,D,H>::Bucket::Bucket( const K &key, const D &data, typename UTable<K,D,H>::Bucket *next )
: key( key ), data( data ), next( next )
{}

}; // namespace ADT

}; // namespace huys

#endif // U_TABLE_H

