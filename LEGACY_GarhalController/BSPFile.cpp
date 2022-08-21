/**
 * @author     ReactiioN
 * @date       08.03.2016
 * @visit      https://github.com/ReactiioN1337
 *             https://reactiion.pw
 *             https://www.unknowncheats.me/forum/members/264622.html
 */
#include "BSPFile.hpp"
#include <iostream>
using namespace hazedumper;
using namespace BSP;

BSPFile::BSPFile( const std::string& bsp_directory, const std::string& bsp_file )
{
    parse( bsp_directory, bsp_file );
}

bool BSPFile::parse( const std::string& bsp_directory, const std::string& bsp_file )
{
    if( bsp_directory.empty() || bsp_file.empty() ) {
        return false;
    }

    std::ifstream bsp_binary( bsp_directory + "\\" + bsp_file, std::ios::binary );
    if( !bsp_binary.is_open() ) {
        return false;
    }
    m_FileName = bsp_file;
    try {
        /// parse the bsp header
        bsp_binary.read( reinterpret_cast< char* >( &m_BSPHeader ), sizeof dheader_t );

        /// check bsp version/ident
        if( m_BSPHeader.m_Version < BSPVERSION ) {
            std::cout << "BSPFile::parse(): " << bsp_file << "has an unknown BSP version, trying to parse it anyway..." << std::endl;
        }
        if( m_BSPHeader.m_Ident != IDBSPHEADER ) {
            std::cout << "BSPFile::parse(): " << bsp_file << "isn't a (valid) BSP file!" << std::endl;
            return false;
        }

        parse_lump_data( bsp_binary, LUMP_VERTEXES, m_Vertexes );
        if( !parse_planes( bsp_binary ) ) {
            return false;
        }

        parse_lump_data( bsp_binary, LUMP_EDGES, m_Edges );
        parse_lump_data( bsp_binary, LUMP_SURFEDGES, m_Surfedges );
        parse_lump_data( bsp_binary, LUMP_LEAFS, m_Leaves );
        if( !parse_nodes( bsp_binary ) ) {
            return false;
        }

        parse_lump_data( bsp_binary, LUMP_FACES, m_Surfaces );
        parse_lump_data( bsp_binary, LUMP_TEXINFO, m_Texinfos );
        parse_lump_data( bsp_binary, LUMP_BRUSHES, m_Brushes );
        parse_lump_data( bsp_binary, LUMP_BRUSHSIDES, m_Brushsides );
        if( !parse_leaffaces( bsp_binary )
            || !parse_leafbrushes( bsp_binary )
            || !parse_polygons() ) {
            return false;
        }
    }
    catch( const std::exception& e ) {
        print_exception( "parse", e );
        return false;
    }

    return true;
}

bool BSPFile::parse_planes( std::ifstream& bsp_binary )
{
    try {
        std::vector< dplane_t > planes;
        parse_lump_data( bsp_binary, LUMP_PLANES, planes );

        m_Planes = std::vector< cplane_t >( planes.size() );

        int32_t plane_bits;
        for( size_t i = 0; i < planes.size(); ++i ) {
            auto& out = m_Planes.at( i );
            auto& in = planes.at( i );

            plane_bits = 0;
            for( size_t j = 0; j < 3; ++j ) {
                out.m_Normal( j ) = in.m_Normal( j );
                if( out.m_Normal( j ) < 0.f ) {
                    plane_bits |= 1 << static_cast< int32_t >( j );
                }
            }

            out.m_Distance = in.m_Distance;
            out.m_Type = static_cast< uint8_t >( in.m_Type );
            out.m_SignBits = static_cast< uint8_t >( plane_bits );
        }
    }
    catch( const std::exception& e ) {
        print_exception( "parse_planes", e );
        return false;
    }
    return true;
}

bool BSPFile::parse_nodes( std::ifstream& bsp_binary )
{
    try {
        std::vector< dnode_t > nodes;
        parse_lump_data( bsp_binary, LUMP_NODES, nodes );

        const auto num_nodes = nodes.size();
        m_Nodes = std::vector< snode_t >( num_nodes );

        for( size_t i = 0; i < num_nodes; ++i ) {
            auto& in = nodes.at( i );
            auto& out = m_Nodes.at( i );

            out.m_Mins = in.m_Mins;
            out.m_Maxs = in.m_Maxs;
            out.m_PlaneNum = in.m_Planenum;
            out.m_pPlane = m_Planes.data() + in.m_Planenum;
            out.m_Firstface = in.m_Firstface;
            out.m_Numfaces = in.m_Numfaces;

            for( size_t j = 0; j < 2; ++j ) {
                auto child_index = in.m_Children.at( j );
                out.m_Children.at( j ) = child_index;

                if( child_index >= 0 ) {
                    out.m_LeafChildren = nullptr;
                    out.m_NodeChildren = m_Nodes.data() + child_index;
                }
                else {
                    out.m_LeafChildren = m_Leaves.data() + ( -1 - child_index );
                    out.m_NodeChildren = nullptr;
                }
            }
        }
    }
    catch( const std::exception& e ) {
        print_exception( "parse_nodes", e );
        return false;
    }
    return true;
}

bool BSPFile::parse_leaffaces( std::ifstream& bsp_binary )
{
    try {
        parse_lump_data( bsp_binary, LUMP_LEAFFACES, m_Leaffaces );

        const auto num_leaffaces = m_Leaffaces.size();
        if( num_leaffaces > MAX_MAP_LEAFBRUSHES ) {
            std::cout << "BSPFile::parse_leaffaces(): map has to many leaffaces, parsed more than required.." << std::endl;
        }
        else if( !num_leaffaces ) {
            std::cout << "BSPFile::parse_leaffaces(): map has no leaffaces to parse!" << std::endl;
        }
    }
    catch( const std::exception& e ) {
        print_exception( "parse_leaffaces", e );
        return false;
    }
    return true;
}

bool BSPFile::parse_leafbrushes( std::ifstream& bsp_binary )
{
    try {
        parse_lump_data( bsp_binary, LUMP_LEAFBRUSHES, m_Leafbrushes );

        const auto num_leaffaces = m_Leaffaces.size();
        if( num_leaffaces > MAX_MAP_LEAFBRUSHES ) {
            std::cout << "BSPFile::parse_leaffaces(): map has to many leafbrushes, parsed more than required.." << std::endl;
        }
        else if( !num_leaffaces ) {
            std::cout << "BSPFile::parse_leaffaces(): map has no leafbrushes to parse!" << std::endl;
        }
    }
    catch( const std::exception& e ) {
        print_exception( "parse_leafbrushes", e );
        return false;
    }
    return true;
}

bool BSPFile::parse_polygons( void )
{
    try {
        m_Polygons = std::vector< Polygon >( m_Surfaces.size() );
        for( auto& surface : m_Surfaces ) {
            auto first_edge = surface.m_Firstedge;
            auto num_edges = surface.m_Numedges;

            if( num_edges < 3 || static_cast< size_t >( num_edges ) > MAX_SURFINFO_VERTS ) {
                continue;
            }
            if( surface.m_Texinfo <= 0 ) {
                continue;
            }

            Polygon polygon;
            Vector3 edge;
            for( auto i = 0; i < num_edges; ++i ) {
                auto edge_index = m_Surfedges.at( first_edge + i );
                if( edge_index >= 0 ) {
                    edge = m_Vertexes.at( m_Edges[ edge_index ].m_V.at( 0 ) ).m_Position;
                }
                else {
                    edge = m_Vertexes.at( m_Edges[ -edge_index ].m_V.at( 1 ) ).m_Position;
                }
                polygon.m_Verts.at( i ) = edge;
            }

            polygon.m_nVerts = static_cast< size_t >( num_edges );
            polygon.m_Plane.m_Origin = m_Planes.at( surface.m_Planenum ).m_Normal;
            polygon.m_Plane.m_Distance = m_Planes.at( surface.m_Planenum ).m_Distance;
            m_Polygons.push_back( polygon );
        }
    }
    catch( const std::exception& e ) {
        print_exception( "parse_polygons", e );
        return false;
    }
    return true;
}

void BSPFile::print_exception( const std::string& function_name, const std::exception& e ) const
{
    std::cout << "BSPFile::"
        << ( function_name.empty() ? "print_exception" : function_name ).c_str()
        << "() exception("
        << e.what()
        << ") while parsing map: "
        << m_FileName
        << std::endl;
}
