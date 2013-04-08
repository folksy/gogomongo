#pragma once

#include <fstream>
#include <string>

namespace b23 {
  namespace file {
    using namespace std;

    template <ios_base::openmode M>
      class Writer
      {
      public:
        Writer( const string &fname ) : __file( fname, M ) {}
        ~Writer() { if ( __file.is_open() ) __file.close(); }
        template <typename T>
          ostream & operator << ( const T &obj )
          {
            return __file << obj;
          }
      private:
        ofstream __file;
      };

    class Reader
    {
    public:
      Reader( const string &fname ) : __file( fname, ios_base::in ) {}
      ~Reader() { if ( __file.is_open() ) __file.close(); }
      template <typename T>
        istream & operator >> ( T &obj )
        {
          return __file >> obj;
        }
    private:
      ifstream __file;
    };

    typedef Writer<ios_base::out | ios_base::app> appender_t;
    typedef Writer<ios_base::out | ios_base::trunc> overwriter_t;
    typedef Reader reader_t;
  }
}
