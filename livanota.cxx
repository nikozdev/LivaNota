/* configs */

#ifndef _NAME
#    define _NAME livanota
#endif /* _NAME */
#ifndef _VERS
#    define _VERS 0.0.0
#endif /* _VERS */

#define _TO_STR( text ) #text
#define _NAME_STR #_NAME
#define _VERS_STR #_VERS

/* headers */

#include "unistd.h"
#include "stdarg.h"

#include "cstdio"
#include "cstring"
#include "cstdlib"

#include "string"
#include "fstream"
#include "iostream"
#include "filesystem"

/* defines */

#   define NS_OPEN namespace _NAME {
#   define NS_SHUT }

NS_OPEN

/* typedefs */

enum ecode_e: int
{
    ecode_none = 0x0,

    ecode_argc = 0x1,
    ecode_argv = 0x2,

    ecode_last = 0xf,
};

struct config_t
{
    std::filesystem::path path;
    struct {
        std::filesystem::path path;
    } target;
    struct {
        std::filesystem::path path;
    } editor;
    struct {
        std::string time;
    } format;
};

/* constants */

constexpr bool FALSE = false;
constexpr bool TRUTH = true;

constexpr bool ZERO = 0;
constexpr bool UNIT = 1;

std::string CONFIG_PREFIX = _TO_STR( _NAME );
std::string CONFIG_SUFFIX = "lua";
std::string CONFIG_PATH = _TO_STR( _NAME ) ".lua";

std::string TARGET_PREFIX = _TO_STR( _NAME );
std::string TARGET_SUFFIX = "txt";
std::string TARGET_PATH = _TO_STR( _NAME ) ".txt";

std::string TIME_FORMAT = "y%ym%md%d-h%Hm%Ms%S";

/* variables */

config_t config;

/* functions */

inline std::string get_env( const char* name )
{
    const char* nstr = name;
    auto estr =  std::getenv( nstr );

    return estr == NULL ? std::string() : std::string( estr );
}
inline std::string get_env( const std::string& name )
{
    return get_env( name.c_str() );
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

int help( ecode_e ecode )
{
    const char* text;
    switch( ecode )
    {
        case ecode_argc:
            text = "argc";
            break;
        case ecode_argv:
            text = "argv";
            break;
        default:
            return ecode;
    }
    std::cerr << "[" _TO_STR( _NAME ) "]" << "[help]" << std::endl;
    {
        std::cerr << "- usage" << std::endl;
        {
            std::cerr << "> " << _TO_STR( _NAME ) << std::endl;
        }
        std::cerr << "- flags" << std::endl;
        {
            std::cerr << "[-c|--config=<config-file-path>]="
                << "${LIVANOTA_CONFIG_PATH}" << "|" 
                << "${XDG_CONFIG_HOME}/" << CONFIG_PATH << "|"
                << "${HOME}/" << CONFIG_PATH << "|"
                << CONFIG_PATH << "|"
                << std::endl;
            std::cerr << "[-t|--target=<target-file-path>]="
                << "config.target.path" << "|" 
                << "${LIVANOTA_TARGET_PATH}" << "|" 
                << "${XDG_DATA_HOME}/" << TARGET_PATH << "|"
                << "${HOME}/" << TARGET_PATH << "|"
                << TARGET_PATH << std::endl;
            std::cerr << "[-e|--editor=<editor-file-path>]="
                << "config.editor.path" << "|"
                << "${LIVANOTA_EDITOR_PATH}" << "|"
                << "${EDITOR}" << "|"
                << "nvim" << "|"
                << "vim" << "|"
                << "vi" << std::endl;
        }
        std::cerr << "- ecode: " << ecode << std::endl;
    }
    std::cerr << std::endl;

    return static_cast< int >( ecode );
}

int main( int argc, const char** argv )
{
    switch( argc )
    {
        case 1:
            break;
        case 2:
            break;
        default:
            return help( ecode_argc );
    }

    config.path = get_nodefault_string( get_env( "LIVANOTA_CONFIG_PATH" ), get_env( "XDG_CONFIG_HOME" ) + "/" + CONFIG_PATH, get_env( "HOME" ), CONFIG_PATH );

    config.target.path = get_nodefault_string( get_env( "LIVANOTA_TARGET_PATH" ), get_env( "XDG_DATA_HOME" ) + "/" + TARGET_PATH, get_env( "HOME" ) + TARGET_PATH, TARGET_PATH );

    config.editor.path = get_nodefault_string( get_env( "LIVANOTA_EDITOR_PATH" ), get_env( "EDITOR" ), std::string( "nvim" ) );

    config.format.time = TIME_FORMAT;

    std::string command = ( config.editor.path.string() + " " + config.target.path.string() ).c_str();
    std::system( command.c_str() );

    return help( ecode_none );
}

NS_SHUT

#ifdef _TYPE_RUN
int main( int argc, const char** argv )
{
    return _NAME::main( argc, argv );
}
#endif
