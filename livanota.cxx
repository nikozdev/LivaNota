/* configs */

/* headers */

#include "ctime"
#include "cstdio"
#include "cstring"
#include "cstdlib"

#include "algorithm"
#include "string"
#include "string_view"
#include "vector"
#include "fstream"
#include "sstream"
#include "iostream"
#include "iomanip"
#include "filesystem"

#include "unistd.h"

/* defines */

#ifndef _NAME
#    define _NAME       livanota
#    define _NAME_STR   "livanota"
#endif /* _NAME */
#ifndef _VERS
#    define _VERS       0x0x0
#    define _VERS_STR   "0x0x0"
#endif /* _VERS */

#define _TO_STR( text ) ( #text )

#define _NAMESPACE_START namespace _NAME {
#define _NAMESPACE_CLOSE }

_NAMESPACE_START

/* typedefs */

#define _ENUM_DEF( name, _FOR ) \
    \
enum name##_enum: int \
{ \
    _FOR( _ACT_ENUM ) \
    name##_last \
}; \
const char* name##_text[] = \
{ \
    _FOR( _ACT_TEXT ) \
    [ name##_last ] = "" \
}; \
/* ENUM_DEF */

#define _FOR_ERROR( _ACT ) \
    \
_ACT( none ) \
_ACT( argc ) \
_ACT( argv ) \
_ACT( comd ) \
_ACT( opts ) \
_ACT( conf ) \
_ACT( evar ) \
/* _FOR_ERROR */
#define _ACT_ENUM( name ) error_##name,
#define _ACT_TEXT( name ) [ error_##name ] = _TO_STR( error_##name ),
_ENUM_DEF( error, _FOR_ERROR )
#undef  _ACT_ENUM
#undef  _ACT_TEXT

#define _FOR_TIMEZ( _ACT ) \
        \
    _ACT( loc ) \
    _ACT( gmt ) \
    /* _FOR_TIMEZ */

#define _ACT_ENUM( name ) timez_##name,
#define _ACT_TEXT( name ) [ timez_##name ] = _TO_STR( timez_##name ),
_ENUM_DEF( timez, _FOR_TIMEZ )
#undef  _ACT_ENUM
#undef  _ACT_TEXT

    /* constants */

    constexpr auto FALSE = false;
    constexpr auto TRUTH = true;

    constexpr auto ZERO = 0;
    constexpr auto UNIT = 1;

    std::string CONFIG_PREFIX = _NAME_STR;
    std::string CONFIG_SUFFIX = "lua";
    std::string CONFIG_PATH = _NAME_STR ".lua";

    std::string TARGET_PREFIX = _NAME_STR;
    std::string TARGET_SUFFIX = "txt";
    std::string TARGET_PATH = _NAME_STR ".txt";

    std::string TIME_FORMAT = "y%ym%md%d-h%Hm%Ms%S";

    /* variables */

    struct
{
    std::filesystem::path path;
    struct {
        std::filesystem::path path;
    } target;
    struct {
        std::filesystem::path path;
    } editor;
    struct {
        std::string format;
        timez_enum  zone;
    } time;
} config;

struct
{
    std::time_t unix;
    std::tm     info;
} time;

struct
{
    std::vector< std::string_view > args;
} cli;

/* functions */

inline error_enum help( error_enum error )
{
    if ( error == error_none ) { return error; }

    std::cerr << "[" _NAME_STR "]" << "[help]" << std::endl;
    {
        std::cerr << "- usage" << std::endl;
        {
            std::cerr << "> " << _NAME_STR << " <action>" << " [-o|--option]..." << std::endl;
        }
        std::cerr << "- actions" << std::endl;
        {
#           if FALSE
            std::cerr << "> make [tag]... - write new nota in a text editor" << std::endl;
            std::cerr << "> edit [tag]... - open tagged notas in a text editor" << std::endl;
            std::cerr << "> view [tag]... - open notas in text user interface" << std::endl;
            std::cerr << "> read [tag]... - output notas into stdout" << std::endl;
#           endif
        }
        std::cerr << "- options" << std::endl;
        {
#           if FALSE
#           ifdef _USE_CONFIG
            std::cerr << "> [-c|--config=<config-file-path>]="
                << "${LIVANOTA_CONFIG_PATH}" << "|" 
                << "${XDG_CONFIG_HOME}/" << CONFIG_PATH << "|"
                << "${HOME}/" << CONFIG_PATH << "|"
                << CONFIG_PATH << "|"
                << std::endl;
#           endif
            std::cerr << "> [-f|--file=<target-file-path>]="
                << "config.target.path" << "|" 
                << "${LIVANOTA_TARGET_PATH}" << "|" 
                << "${XDG_DATA_HOME}/" << TARGET_PATH << "|"
                << "${HOME}/" << TARGET_PATH << "|"
                << TARGET_PATH << std::endl;
            std::cerr << "> [-e|--editor=<editor-file-path>]="
                << "config.editor.path" << "|"
                << "${LIVANOTA_EDITOR_PATH}" << "|"
                << "${EDITOR}" << "|"
                << "nvim" << "|"
                << "vim" << "|"
                << "vi" << std::endl;
#           endif
        }
        std::cerr << "- error" << std::endl;
        {
            std::cerr << "> code=" << static_cast< int >( error ) << std::endl;
            std::cerr << "> text=" << error_text[ error ] << std::endl;
        }
    }

    return error;
}

inline std::string get_env( const std::string_view& name )
{
    const char* nstr = &name[0];
    auto estr =  std::getenv( nstr );

    return estr == NULL ? std::string() : std::string( estr );
}

    template< typename type_t >
inline const type_t& get_nodefault( const type_t& def, const type_t& arg )
{
    return arg;
}
    template< typename type_t, typename ... args_t >
inline const type_t& get_nodefault( const type_t& def, const type_t& arg, args_t&& ... args )
{
    return arg == def ? get_nodefault( def, std::forward< args_t >( args )... ) : arg;
}
    template< typename ... args_t >
inline const std::string& get_nodefault_string( args_t&& ... args )
{
    std::string def;
    return get_nodefault( def, std::forward< args_t >( args )... );
}

inline error_enum update_time()
{
    time.unix = std::time( nullptr );

    switch ( config.time.zone )
    {
        case timez_loc:
            time.info = *std::localtime( &time.unix );
            break;
        case timez_gmt:
            time.info = *std::gmtime( &time.unix );
            break;
        default:
            return help( error_conf );
    }

    return error_none;
}

inline std::ostream& append_header( std::ostream& stream, bool top )
{
    return stream
        << ( top ? "([" : "[" )
        << time.unix << "="
        << std::put_time( &time.info, &config.time.format[0] )
        << ( top ? "]" : "])" )
        << std::endl << std::endl;
}

error_enum make()
{
    char source_name[ PATH_MAX ];
    std::snprintf( source_name, PATH_MAX, "%s%s%s", _NAME_STR, "-", "XXXX" "XXXX" );
    auto tempfd = ::mkstemp( source_name );
    ( void )tempfd;

    std::filesystem::path source_path = std::filesystem::current_path() / source_name;

    std::string command = config.editor.path.string() + " " + source_path.string();
    std::system( &command[0] );

    update_time();

    std::fstream target_stream;
    std::fstream source_stream;

    target_stream.open( config.target.path, std::ios::app );
    source_stream.open( source_path, std::ios::in );

    append_header( target_stream, TRUTH );
    target_stream << source_stream.rdbuf() << std::endl;
    append_header( target_stream, FALSE );

    source_stream.close();
    target_stream.close();

    std::filesystem::remove( source_path );

    return help( error_none );
}
error_enum edit()
{
    return help( error_none );
}
error_enum view()
{
    return help( error_none );
}
error_enum read()
{
    return help( error_none );
}

error_enum init()
{
    // config.path = get_nodefault_string( get_env( "LIVANOTA_CONFIG_PATH" ), std::format( "{}/{}", get_env( "XDG_CONFIG_HOME" ), CONFIG_PATH ), get_env( "HOME" ), CONFIG_PATH );
    config.path = get_nodefault_string( get_env( "LIVANOTA_CONFIG_PATH" ), get_env( "XDG_CONFIG_HOME" ) + "/" + CONFIG_PATH, get_env( "HOME" ), CONFIG_PATH );

    config.target.path = get_nodefault_string( get_env( "LIVANOTA_TARGET_PATH" ), get_env( "XDG_DATA_HOME" ) + "/" + TARGET_PATH, get_env( "HOME" ) + TARGET_PATH, TARGET_PATH );

    config.editor.path = get_nodefault_string( get_env( "LIVANOTA_EDITOR_PATH" ), get_env( "EDITOR" ), std::string( "nvim" ) );

    config.time.format = get_nodefault_string( get_env( "LIVANOTA_TIME_FORMAT" ), TIME_FORMAT );

    return error_none;
}
error_enum work()
{
    const std::string_view action = cli.args[ 1 ];

    if ( action[ 0 ] == '-' )
    {
        return help( error_argv );
    }
    else if ( action == "make" )
    {
        make();
    }
    else if ( action == "edit" )
    {
        edit();
    }
    else if ( action == "view" )
    {
        view();
    }
    else if ( action == "read" )
    {
        read();
    }
    else
    {
        return help( error_argv );
    }

    return help( error_none );
}

int main( int argc, const char* argv[] )
{
    cli.args = std::vector< std::string_view >( argv, argv + argc );
    if ( cli.args.size() > 1 )
    {
        std::clog << std::endl;
        std::clog << "(" << "[" << _NAME_STR << "]" << "[args]" << std::endl;

        std::copy( argv, argv + argc, std::ostream_iterator< const char* >( std::clog, "\n" ) );

        std::clog << "[" << _NAME_STR << "]" << "[args]" << ")" << std::endl;
        std::clog << std::endl;
    }
    else
    {
        return help( error_argc );
    }

    init();
    work();

    return help( error_none );
}

_NAMESPACE_CLOSE

#ifdef _TYPE_RUN
int main( int argc, const char** argv )
{
    return _NAME::main( argc, argv );
}
#endif
