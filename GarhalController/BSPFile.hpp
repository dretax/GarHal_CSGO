/**
 * @author     ReactiioN
 * @date       08.03.2016
 * @visit      https://github.com/ReactiioN1337
 *             https://reactiion.pw
 *             https://www.unknowncheats.me/forum/members/264622.html
 */
#pragma once
#include "BSPStructure.hpp"
#include <fstream>
#include <vector>

namespace hazedumper {

    class BSPFile
    {
    public:
        BSPFile( void ) = default;

        /**
         * @brief      Optional constructor, parses the map.
         *
         * @param[in]  bsp_directory  The bsp directory
         * @param[in]  bsp_file       The bsp file
         */
        BSPFile( const std::string& bsp_directory, const std::string& bsp_file );

        /**
         * @brief      Parse a bsp file.
         *
         * @param[in]  bsp_directory  The bsp directory, last character must be '\'
         * @param[in]  bsp_file       The bsp file + extension(.bsp)
         *
         * @return     True if the BSP version and the ident is valid plus if all
         *             lumps got parsed, False otherwise or when an exception got
         *             throwed.
         */
        bool parse( const std::string& bsp_directory, const std::string& bsp_file );

        friend std::ostream& operator <<( std::ostream& os, const BSPFile& bsp_file )
        {
            os << "/// map: "       << bsp_file.m_FileName            << "\n"
               << "> BSP-Version: " << bsp_file.m_BSPHeader.m_Version << "\n"
               << "> Vertexes: "    << bsp_file.m_Vertexes.size()     << "\n"
               << "> Planes: "      << bsp_file.m_Edges.size()        << "\n"
               << "> Surfedges: "   << bsp_file.m_Surfedges.size()    << "\n"
               << "> Leaves: "      << bsp_file.m_Leaves.size()       << "\n"
               << "> Nodes: "       << bsp_file.m_Nodes.size()        << "\n"
               << "> Surfaces: "    << bsp_file.m_Surfaces.size()     << "\n"
               << "> Texinfos: "    << bsp_file.m_Texinfos.size()     << "\n"
               << "> Brushes: "     << bsp_file.m_Brushes.size()      << "\n"
               << "> Brushsides: "  << bsp_file.m_Brushsides.size()   << "\n"
               << "> Leaffaces: "   << bsp_file.m_Leaffaces.size()    << "\n"
               << "> Leafbrushes: " << bsp_file.m_Leafbrushes.size()  << "\n"
               << "> Polygons: "    << bsp_file.m_Polygons.size();

            return os;
        }

    private:
        /**
         * @brief      Parse map planes.
         *
         * @param      bsp_binary  The bsp binary
         *
         * @return     False if an exception got throwed, True otherwise.
         */
        bool parse_planes( std::ifstream& bsp_binary );

        /**
         * @brief      Parse map nodes.
         *
         * @param      bsp_binary  The bsp binary
         *
         * @return     False if an exception got throwed, True otherwise.
         */
        bool parse_nodes( std::ifstream& bsp_binary );

        /**
         * @brief      Parse map leaffaces.
         *
         * @param      bsp_binary  The bsp binary
         *
         * @return     False if an exception got throwed, True otherwise.
         */
        bool parse_leaffaces( std::ifstream& bsp_binary );

        /**
         * @brief      Parse map leafbrushes.
         *
         * @param      bsp_binary  The bsp binary
         *
         * @return     False if an exception got throwed, True otherwise.
         */
        bool parse_leafbrushes( std::ifstream& bsp_binary );

        /**
         * @brief      Parse map polygons.
         *
         * @return     False if an exception got throwed, True otherwise.
         */
        bool parse_polygons( void );

        /**
         * @brief      Print function specific exception.
         *
         * @param[in]  function_name  The function name
         * @param[in]  e              The exception
         */
        void print_exception( const std::string& function_name, const  std::exception& e ) const;

        /**
         * @brief      Parse specific lump data from bsp binary.
         *
         * @param      bsp_binary  The bsp binary
         * @param[in]  lump_index  The lump index
         * @param      buffer      The buffer
         *
         * @tparam     T           The lump struct declaration
         */
        template< typename T >
        void parse_lump_data( std::ifstream& bsp_binary, const BSP::eLumpIndex lump_index, std::vector< T >& buffer ) const;

    public:
        std::string                      m_FileName;
        BSP::dheader_t                   m_BSPHeader;
        std::vector< BSP::mvertex_t >    m_Vertexes;
        std::vector< BSP::cplane_t >     m_Planes;
        std::vector< BSP::dedge_t >      m_Edges;
        std::vector< int32_t >           m_Surfedges;
        std::vector< BSP::dleaf_t >      m_Leaves;
        std::vector< BSP::snode_t >      m_Nodes;
        std::vector< BSP::dface_t >      m_Surfaces;
        std::vector< BSP::texinfo_t >    m_Texinfos;
        std::vector< BSP::dbrush_t >     m_Brushes;
        std::vector< BSP::dbrushside_t > m_Brushsides;
        std::vector< uint16_t >          m_Leaffaces;
        std::vector< uint16_t >          m_Leafbrushes;
        std::vector< BSP::Polygon >      m_Polygons;
    };

    template< typename T >
    void BSPFile::parse_lump_data( std::ifstream& bsp_binary, const BSP::eLumpIndex lump_index, std::vector< T >& buffer ) const
    {
        auto& lump = m_BSPHeader.m_Lumps.at( static_cast< size_t >( lump_index ) );
        const auto lump_size = lump.m_Filelen / sizeof T;
        if( !lump_size ) {
            return;
        }

        buffer = std::vector< T >( lump_size );

        bsp_binary.seekg( lump.m_Fileofs );
        bsp_binary.read( reinterpret_cast< char* >( buffer.data() ), lump_size * sizeof T );
    }
}
