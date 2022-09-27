#pragma once

/* configs */

#ifndef _NAME_
#    define _NAME_ livanota
#endif /* _NAME_ */
#ifndef _VERS_
#    define _VERS_ 0.0.0
#endif /* _VERS_ */

/* headers */

#   include "cstdio"
#   include "cstdlib"
#   include "cstring"

#   include "string"
#   include "iostream"

/* defines */

#   define NS_OPEN namespace _NAME_ {
#   define NS_SHUT }

NS_OPEN

/* typedefs */

using v1b_t = bool;
using v1c_t = char;
using v1s_t = int;
using v1u_t = unsigned int;
using v1f_t = float;

using count_t   = v1u_t;
using index_t   = v1u_t;

using scstr_t   = const char*;

using mbyte_t   = char;
using mdata_t   = mbyte_t*;

/* arguments
 * command line arguments array
 */
class args_t
{
    public:

        args_t( ): count( ), array( ) {}
        args_t( count_t count, scstr_t* array ):
            count( count ), array( array ) {}

    public:

        std::ostream& oput( std::ostream& stream ) const
        {
            stream << "[count]=(" << this->count << ")";
            stream << "[array]=(";
            for ( index_t iter = 0; iter < this->count; iter++ )
            {
                stream << "[" << iter << "]" << "(" << this->array[ iter ] << ")";
            }
            stream << ")";
            return stream;
        }
        std::ostream& olog() const
        {
            return this->oput( std::cout );
        }

    public:

        count_t count;
        scstr_t*array;
};

enum class error_e: int
{
    none = 0,
};

/* constants */

constexpr bool FALSE = false;
constexpr bool TRUTH = true;

constexpr bool ZERO = 0;
constexpr bool UNIT = 1;

args_t ARGS;

/* variables */

/* functions */

bool init( )
{
    return TRUTH;
}
bool work( )
{
    ARGS.olog();

    return FALSE;
}
bool quit( )
{
    return TRUTH;
}

int main( int argc, const char** argv )
{
    ARGS = args_t( argc, argv );

    init();
    while ( work() );
    quit();

    return static_cast< int >( error_e::none );
}

NS_SHUT
