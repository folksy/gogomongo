#pragma once

#include <memory>
#include "mongo/client/dbclient.h"

namespace fhq {
  using namespace std;
  using namespace mongo;

  class processor_c
  {
  public:
    virtual ~processor_c() {}
    virtual void operator() ( BSONObj &obj ) = 0;
  };

  template <typename T>
    class processor_w : public processor_c
    {
    public:
      processor_w( const T &t ) : __t( t ) {}
      processor_w( const processor_w &o ) = default;
      processor_w( processor_w &&o ) = default;

      virtual void operator() ( BSONObj &obj ) { __t.operator() ( obj ); }
      
    private:
      T __t;
    };

  class processor_t
  {
  public:
    template <typename T>
      processor_t( const T &p ) : __p( make_shared<T>( p ) ) {}
    processor_t( const processor_t &o ) = default;
    processor_t( processor_t &&o ) = default;

    VIRTUAL void operator() ( BSONObj &obj ) { __p->operator() ( obj ); }
    
  private:
    shared_ptr<processor_c> __p;
  };
}
