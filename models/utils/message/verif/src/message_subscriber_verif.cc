
#include <iostream>
#include <unistd.h>

#include "../include/message_subscriber_verif.hh"

void jeod::MessageFileVerif::update( unsigned int level __attribute__ ((unused)) , std::string header __attribute__ ((unused)) , std::string message ) {

    if ( enabled ) {
        size_t path_start, unique_end;                       
        unique_end = message.find("models/utils/message");
        if( unique_end != std::string::npos )                              
        {                                                                                      
            path_start = message.substr( 0, unique_end ).find_last_of(' ');            
            message = message.substr( 0, path_start+1 ) + message.substr( unique_end );
        }
        out_stream << message ;
        out_stream.flush() ;
    }

}
