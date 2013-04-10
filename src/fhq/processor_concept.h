#pragma once

#include <memory>
#include "mongo/client/dbclient.h"

namespace fhq {
  using namespace std;
  using namespace mongo;
  using std::shared_ptr;

  class processor_c
  {
  public:
    virtual ~processor_c() {}
    virtual void operator() ( BSONObj &obj ) = 0;
    
    TEST_ONLY( public: virtual const string desc() const = 0; )
  };

  template <typename T>
    class processor_w : public processor_c
    {
    public:
      processor_w( const T &t ) : __t( t ) {}
      processor_w( T &&t ) : __t( std::move( t ) ) {}
      processor_w( const processor_w &o ) = default;
      processor_w( processor_w &&o ) = default;

      virtual void operator() ( BSONObj &obj ) { __t( obj ); }
      
    private:
      T __t;

      TEST_ONLY( public: virtual const string desc() const { return __t.desc(); } )
    };

  class processor_t
  {
  public:
    processor_t() = default;
    template <typename T>
      processor_t( const T &p ) : __p( make_shared<processor_w<T>>( p ) ) {}
    template <typename T>
      processor_t( T &&p ) : __p( make_shared<processor_w<T>>( std::move( p ) ) ) {}
    processor_t( const processor_t & ) = default;
    processor_t( processor_t && ) = default;

    processor_t & operator= ( const processor_t & ) = default;
    processor_t & operator= ( processor_t && ) = default;

    VIRTUAL void operator() ( BSONObj &obj ) { ( *__p )( obj ); }
    
  private:
    shared_ptr<processor_c> __p;

    TEST_ONLY( public: const string desc() const { return __p->desc(); } )
  };
}
