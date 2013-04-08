#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <utility>
#include "mongo/client/dbclient.h"
#include <getopt.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <cstdio>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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

namespace runner {
  using namespace std;
  using boost::property_tree::ptree;

  void start( const string &configfname, const string &pidfname, const string &errlogfname,
              const string &connection_string, const string &db, const string &input_collection )
  {
    cout << "Log server starting with:" << endl
         << "\tConfig filename:\t" << configfname << endl
         << "\tPID filename:\t\t" << pidfname << endl
         << "\tError log filename:\t" << errlogfname << endl
         << "\tConnection string:\t" << connection_string << endl
         << "\tDatabase:\t\t" << db << endl
         << "\tInput collection:\t" << input_collection << endl;
    pid_t pid( getpid() );
    file::overwriter_t( pidfname ) << pid;
    ptree pt;
    read_json( configfname, pt );
    map<string, pair<string, vector<string>>> configs;
    for ( const auto &processor : pt.get_child( "processors" ) ) {
      string name( processor.second.get<string>( "name" ) );
      string ns( processor.second.get<string>( "ns" ) );
      vector<string> pdescs;
      for ( const auto &pdesc : processor.second.get_child( "processors" ) )
        pdescs.push_back( pdesc.second.data() );
      configs[name] = { ns, pdescs };
    }
    throw runtime_error( "runner::start( ... ) not implemented." );
  }

  void stop( const string &pidfname )
  {
    pid_t pid;
    file::reader_t( pidfname ) >> pid;
    kill( pid, SIGKILL );
    remove( pidfname.c_str() );
    throw runtime_error( "runner::stop( ... ) not implemented." );
  }
}

int main( int argc, char *argv[] )
{
  try {
    if ( argc < 2 )
      throw std::runtime_error( "No command given." );
    std::string command( argv[1] );
    if ( command.compare( "start" ) != 0 &&
         command.compare( "stop" ) != 0 &&
         command.compare( "restart" ) != 0 )
      throw std::runtime_error( "Unrecognised command." );
    if ( command.compare( "stop" ) != 0 && argc < 3 )
      throw std::runtime_error( "Any run command requires a config filename parameter." );
    std::string configfname( command.compare( "stop" ) == 0 ? "N/A" : argv[2] );
    std::string
      connection_string( "localhost" ),
      db( "logserver" ),
      input_collection( "_lsevents" ),
      pidfname( "pids/logserver.pid" ),
      errlogfname( "log/errors.log" );
    static struct option long_options[] = {
      { "connection-string", required_argument, nullptr, 'c' },
      { "db",                required_argument, nullptr, 'd' },
      { "input-collection",  required_argument, nullptr, 'i' },
      { "pidfile",           required_argument, nullptr, 'p' },
      { "errfile",           required_argument, nullptr, 'e' }
    };
    int opt_index( 0 );
    int opt( getopt_long( argc, argv, "", long_options, &opt_index ) );
    while ( opt != -1 ) {
      switch ( opt ) {
        case 'c' : connection_string = optarg; break;
        case 'd' : db = optarg; break;
        case 'i' : input_collection = optarg; break;
        case 'p' : pidfname = optarg; break;
        case 'e' : errlogfname = optarg; break;
        default :
          std::stringstream buf;
          buf << "Unhandled option '" << optarg << "'.";
          throw std::runtime_error( buf.str().c_str() );
          break;
      } // esac
      opt = getopt_long( argc, argv, "", long_options, &opt_index );
    } // wend
    switch ( command[2] ) {
      case 'a' : // start
        runner::start( configfname, pidfname, errlogfname,
                       connection_string, db, input_collection );
        break;
      case 'o' : // stop
        runner::stop( pidfname );
        break;
      case 's' : // restart
        runner::stop( pidfname );
        runner::start( configfname, pidfname, errlogfname,
                       connection_string, db, input_collection );
        break;
    } // esac
    return 0;
  } catch ( const std::exception &e ) {
    file::appender_t( "log/errors.log" ) << "Failed with: " << e.what() << "\n";
    std::cerr << "Failed with: " << e.what() << std::endl;
    return 1;
  }
}
