/**
 * @author     ReactiioN
 * @date       08.03.2016
 * @visit      https://github.com/ReactiioN1337
 *             https://reactiion.pw
 *             https://www.unknowncheats.me/forum/members/264622.html
 */
#pragma once
#include "BSPFile.hpp"
#include <shared_mutex>

namespace hazedumper {

    class BSPParser
    {
    public:
        BSPParser( void ) = default;

        /**
         * @brief      Parse a bsp file.
         *
         * @param[in]  bsp_directory  The bsp directory
         * @param[in]  bsp_file       The bsp file
         *
         * @return     True if the bsp file got parsed or is currently cached,
         *             False if BSPFile::parse() fails.
         */
        bool parse_map( const std::string& bsp_directory, const std::string& bsp_file );

        /**
         * @brief      Determines if visible.
         *
         * @param[in]  origin     The origin
         * @param[in]  final      The final position
         *
         * @return     True if visible, False otherwise.
         */
        bool is_visible( const Vector3& origin, const Vector3& final );

        /**
         * @brief      Gets the bsp file.
         *
         * @return     The bsp file.
         */
        BSPFile get_bsp( void ) const;

    private:
        BSPFile                 m_BSPFile;
        std::string             m_LastMap;
        std::shared_timed_mutex m_mutex;
    };
}
