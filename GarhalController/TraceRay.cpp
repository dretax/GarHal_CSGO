/**
 * @author     ReactiioN
 * @date       08.03.2016
 * @visit      https://github.com/ReactiioN1337
 *             https://reactiion.pw
 *             https://www.unknowncheats.me/forum/members/264622.html
 */
#include "TraceRay.hpp"
using namespace hazedumper;
using namespace BSP;

bool TraceRay::is_visible( const Vector3& origin, const Vector3& final, BSPFile* pBSPFile )
{
    if( !pBSPFile ) {
        return false;
    }

    trace_t trace;
    ray_cast( origin, final, pBSPFile, &trace );

    return !( trace.m_Fraction < 1.f ); // no collision!
}

void TraceRay::ray_cast( const Vector3& origin, const Vector3& final, BSPFile* pBSPFile, trace_t* pTrace )
{
    if( pBSPFile->m_Planes.empty() ) {
        return;
    }

    memset( pTrace, 0, sizeof trace_t );
    pTrace->m_Fraction          = 1.0f;
    pTrace->m_FractionLeftSolid = 0.f;
    ray_cast_node( pBSPFile, 0, 0.f, 1.f, origin, final, pTrace );

    if( pTrace->m_Fraction < 1.0f ) {
        for( size_t i = 0; i < 3; ++i ) {
            pTrace->m_EndPos( i ) = origin( i ) + pTrace->m_Fraction * ( final( i ) - origin( i ) );
        }
    }
    else {
        pTrace->m_EndPos = final;
    }
}

void TraceRay::ray_cast_node( BSPFile* pBSPFile, const int32_t node_index, const float start_fraction, const float end_fraction, const Vector3& origin, const Vector3& final, trace_t* pTrace )
{
    if( pTrace->m_Fraction <= start_fraction ) {
        return;
    }
    if( node_index < 0 ) {
        auto* pLeaf = &pBSPFile->m_Leaves.at( static_cast< size_t >( -node_index - 1 ) );
        for( auto i = 0; i < static_cast< int32_t >( pLeaf->m_Numleafbrushes ); ++i ) {

            auto iBrushIndex = static_cast< int32_t >( pBSPFile->m_Leafbrushes.at( pLeaf->m_Firstleafbrush + i ) );
            auto* pBrush = &pBSPFile->m_Brushes.at( iBrushIndex );
            if( !pBrush ) {
                continue;
            }
            if( !( pBrush->m_Contents & MASK_SHOT_HULL ) ) {
                continue;
            }

            ray_cast_brush( pBSPFile, pBrush, pTrace, origin, final );
            if( !pTrace->m_Fraction ) {
                return;
            }
        }
        if( pTrace->m_StartSolid ) {
            return;
        }
        if( pTrace->m_Fraction < 1.f ) {
            return;
        }
        for( auto i = 0; i < static_cast< int32_t >( pLeaf->m_Numleaffaces ); ++i ) {
            ray_cast_surface( pBSPFile, static_cast< int32_t >( pBSPFile->m_Leaffaces.at( pLeaf->m_Firstleafface + i ) ), pTrace, origin, final );
        }

        return;
    }

    auto* pNode = &pBSPFile->m_Nodes.at( static_cast< size_t >( node_index ) );
    if( !pNode ) {
        return;
    }
    auto* pPlane = pNode->m_pPlane;
    if( !pPlane ) {
        return;
    }

    float start_distance, end_distance;

    if( pPlane->m_Type < 3 ) {
        start_distance = origin( static_cast< size_t >( pPlane->m_Type ) ) - pPlane->m_Distance;
        end_distance   = final( static_cast< size_t >( pPlane->m_Type ) ) - pPlane->m_Distance;
    }
    else {
        start_distance = origin.dot( pPlane->m_Normal ) - pPlane->m_Distance;
        end_distance   = final.dot( pPlane->m_Normal ) - pPlane->m_Distance;
    }

    if( start_distance >= 0.f && end_distance >= 0.f ) {
        ray_cast_node( pBSPFile, pNode->m_Children.at( 0 ), start_fraction, end_fraction, origin, final, pTrace );
    }
    else if( start_distance < 0.f && end_distance < 0.f ) {
        ray_cast_node( pBSPFile, pNode->m_Children.at( 1 ), start_fraction, end_fraction, origin, final, pTrace );
    }
    else
    {
        int32_t side_id;
        float fraction_first, fraction_second, fraction_middle;
        Vector3 middle;

        if( start_distance < end_distance ) {
            /// Back
            side_id = 1;
            const auto inversed_distance = 1.f / ( start_distance - end_distance );

            fraction_first  = ( start_distance + FLT_EPSILON ) * inversed_distance;
            fraction_second = ( start_distance + FLT_EPSILON ) * inversed_distance;
        }
        else if( end_distance < start_distance ) {
            /// Front
            side_id = 0;
            const auto inversed_distance = 1.0f / ( start_distance - end_distance );

            fraction_first  = ( start_distance + FLT_EPSILON ) * inversed_distance;
            fraction_second = ( start_distance - FLT_EPSILON ) * inversed_distance;
        }
        else {
            /// Front
            side_id = 0;
            fraction_first  = 1.f;
            fraction_second = 0.f;
        }
        if( fraction_first < 0.f ) {
            fraction_first = 0.f;
        }
        else if( fraction_first > 1.f ) {
            fraction_first = 1.f;
        }
        if( fraction_second < 0.f ) {
            fraction_second = 0.f;
        }
        else if( fraction_second > 1.f ) {
            fraction_second = 1.f;
        }

        fraction_middle = start_fraction + ( end_fraction - start_fraction ) * fraction_first;
        for( size_t i = 0; i < 3; i++ ) {
            middle( i ) = origin( i ) + fraction_first * ( final( i ) - origin( i ) );
        }

        ray_cast_node( pBSPFile, pNode->m_Children.at( side_id ), start_fraction, fraction_middle, origin, middle, pTrace );
        fraction_middle = start_fraction + ( end_fraction - start_fraction ) * fraction_second;
        for( size_t i = 0; i < 3; i++ ) {
            middle( i ) = origin( i ) + fraction_second * ( final( i ) - origin( i ) );
        }

        ray_cast_node( pBSPFile, pNode->m_Children.at( !side_id ), fraction_middle, end_fraction, middle, final, pTrace );
    }
}

void TraceRay::ray_cast_brush( BSPFile* pBSPFile, dbrush_t *pBrush, trace_t *pTrace, const Vector3& origin, const Vector3& final )
{
    if( !pBrush->m_Numsides )
        return;

    auto fraction_to_enter = -99.f;
    auto fraction_to_leave = 1.f;
    auto starts_out        = false;
    auto ends_out          = false;
    for( auto i = 0; i < pBrush->m_Numsides; ++i ) {
        auto* pBrushSide = &pBSPFile->m_Brushsides.at( pBrush->m_Firstside + i );
        if( !pBrushSide || pBrushSide->m_Bevel ) {
            continue;
        }

        auto* pPlane = &pBSPFile->m_Planes.at( pBrushSide->m_Planenum );
        if( !pPlane ) {
            continue;
        }

        const auto start_distance = origin.dot( pPlane->m_Normal ) - pPlane->m_Distance;
        const auto end_distance   = final.dot( pPlane->m_Normal ) - pPlane->m_Distance;
        if( start_distance > 0.f ) {
            starts_out = true;
            if( end_distance > 0.f ) {
                return;
            }
        }
        else {
            if( end_distance <= 0.f ) {
                continue;
            }
            ends_out = true;
        }
        if( start_distance > end_distance ) {
            auto fraction = std::max( ( start_distance - DIST_EPSILON ), 0.f );
            fraction = fraction / ( start_distance - end_distance );
            if( fraction > fraction_to_enter ) {
                fraction_to_enter = fraction;
            }
        }
        else {
            auto fraction = ( start_distance + DIST_EPSILON ) / ( start_distance - end_distance );
            if( fraction < fraction_to_leave ) {
                fraction_to_leave = fraction;
            }
        }
    }

    if( starts_out ) {
        if( pTrace->m_FractionLeftSolid - fraction_to_enter > 0.f ) {
            starts_out = false;
        }
    }

    if( !starts_out ) {
        pTrace->m_StartSolid = true;
        pTrace->m_Contents = pBrush->m_Contents;

        if( !ends_out ) {
            pTrace->m_AllSolid = true;
            pTrace->m_Fraction = 0.f;
            pTrace->m_FractionLeftSolid = 1.f;
        }
        else {
            if( fraction_to_leave != 1.f && fraction_to_leave > pTrace->m_FractionLeftSolid ) {
                pTrace->m_FractionLeftSolid = fraction_to_leave;
                if( pTrace->m_Fraction <= fraction_to_leave ) {
                    pTrace->m_Fraction = 1.f;
                }
            }
        }
        return;
    }

    if( fraction_to_enter < fraction_to_leave ) {
        if( fraction_to_enter > -99.f && fraction_to_enter < pTrace->m_Fraction ) {
            if( fraction_to_enter < 0.f ) {
                fraction_to_enter = 0.f;
            }

            pTrace->m_Fraction = fraction_to_enter;
            pTrace->m_pBrush = pBrush;
            pTrace->m_Contents = pBrush->m_Contents;
        }
    }
}

void TraceRay::ray_cast_surface( BSPFile* pBSPFile, const int32_t surface_index, trace_t *pTrace, const Vector3& origin, const Vector3& final )
{
    auto* pPolygon = &pBSPFile->m_Polygons.at( static_cast< size_t >( surface_index ) );
    if( !pPolygon ) {
        return;
    }

    auto* pPlane    = &pPolygon->m_Plane;
    const auto dot1 = pPlane->dist_to( origin );
    const auto dot2 = pPlane->dist_to( final );

    if( dot1 > 0.f != dot2 > 0.f ) {
        if( dot1 - dot2 < DIST_EPSILON ) {
            return;
        }

        auto t = dot1 / ( dot1 - dot2 );
        if( t <= 0 ) {
            return;
        }

        size_t i;
        auto intersection = origin + ( final - origin ) * t;
        for( i = 0; i < pPolygon->m_nVerts; ++i ) {
            auto* pEdgePlane = &pPolygon->m_EdgePlanes.at( i );
            if( pEdgePlane->m_Origin.empty() ) {
                pEdgePlane->m_Origin = pPlane->m_Origin - ( pPolygon->m_Verts.at( i ) - pPolygon->m_Verts.at( ( i + 1 ) % pPolygon->m_nVerts ) );
                pEdgePlane->m_Origin.normalize();
                pEdgePlane->m_Distance = pEdgePlane->m_Origin.dot( pPolygon->m_Verts.at( i ) );
            }
            if( pEdgePlane->dist_to( intersection ) < 0.0f ) {
                break;
            }
        }
        if( i == pPolygon->m_nVerts ) {
            pTrace->m_Fraction = 0.2f;
            pTrace->m_EndPos = intersection;
        }
    }
}
