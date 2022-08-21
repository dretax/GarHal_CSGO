/**
 * @author     ReactiioN
 * @date       08.03.2016
 * @visit      https://github.com/ReactiioN1337
 *             https://reactiion.pw
 *             https://www.unknowncheats.me/forum/members/264622.html
 */
#include "BSPParser.hpp"
#include "TraceRay.hpp"
using namespace hazedumper;

bool BSPParser::parse_map( const std::string& bsp_directory, const std::string& bsp_file )
{
    if( bsp_directory.empty() || bsp_file.empty() ) {
        return false;
    }
    if( m_LastMap == bsp_file ) {
        return true;
    }

    std::unique_lock< std::shared_timed_mutex > lock( m_mutex );
    if( m_BSPFile.parse( bsp_directory, bsp_file ) ) {
        m_LastMap = bsp_file;
        return true;
    }
    return false;
}

bool BSPParser::is_visible( const Vector3& origin, const Vector3& final )
{
    std::shared_lock< std::shared_timed_mutex > lock( m_mutex );
    return TraceRay::is_visible( origin, final, &m_BSPFile );
}

BSPFile BSPParser::get_bsp( void ) const
{
    return m_BSPFile;
}
