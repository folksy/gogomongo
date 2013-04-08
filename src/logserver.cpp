#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>
#include "mongo/client/dbclient.h"
#include <getopt.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <cstdio>

namespace runner {
  using namespace std;

  void start( const string &configfname, const string &pidfname,
              const string &connection_string, const string &db, const string &input_collection )
  {
    cout << "Log server starting with:" << endl
         << "\tConfig filename:\t" << configfname << endl
         << "\tPID filename:\t" << pidfname << endl
         << "\tConnection string:\t" << connection_string << endl
         << "\tDatabase:\t\t" << db << endl
         << "\tInput collection:\t" << input_collection << endl;
    pid_t pid( getpid() );
    ofstream pidfile( pidfname.c_str() );
    try {
      pidfile << pid;
    } catch ( const ios_base::failure &e ) {
      pidfile.close();
      cerr << "Could not open pid file " << pidfname << " for writing." << endl;
      throw;
    }
    pidfile.close();
    throw runtime_error( "runner::start( ... ) not implemented." );
  }

  void stop( const string &pidfname )
  {
    pid_t pid;
    ifstream pidfile( pidfname.c_str() );
    try {
      pidfile >> pid;
    } catch ( const ios_base::failure &e ) {
      pidfile.close();
      cerr << "Could not open pid file " << pidfname << " for reading." << endl;
      throw;
    }
    pidfile.close();
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
      pidfname( "pids/logserver.pid" );
    static struct option long_options[] = {
      { "connection-string", required_argument, nullptr, 'c' },
      { "db",                required_argument, nullptr, 'd' },
      { "input-collection",  required_argument, nullptr, 'i' },
      { "pidfile",           required_argument, nullptr, 'p' }
    };
    int opt_index( 0 );
    int opt( getopt_long( argc, argv, "", long_options, &opt_index ) );
    while ( opt != -1 ) {
      switch ( opt ) {
        case 'c' : connection_string = optarg; break;
        case 'd' : db = optarg; break;
        case 'i' : input_collection = optarg; break;
        case 'p' : pidfname = optarg; break;
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
        runner::start( configfname, pidfname, connection_string, db, input_collection );
        break;
      case 'o' : // stop
        runner::stop( pidfname );
        break;
      case 's' : // restart
        runner::stop( pidfname );
        runner::start( configfname, connection_string, db, input_collection, pidfname );
        break;
    } // esac
    return 0;
  } catch ( const std::exception &e ) {
    std::cerr << "Failed with: " << e.what() << std::endl;
    return 1;
  }
}
