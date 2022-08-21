/**
 * @author     ReactiioN
 * @date       08.03.2016
 * @visit      https://github.com/ReactiioN1337
 *             https://reactiion.pw
 *             https://www.unknowncheats.me/forum/members/264622.html
 */
#pragma once
#include "BSPFile.hpp"

namespace hazedumper {

    class trace_t
    {
    public:
        /// Determine if plane is NOT valid
        bool           m_AllSolid          = true;
        /// Determine if the start point was in a solid area
        bool           m_StartSolid        = true;
        /// Time completed, 1.0 = didn't hit anything :)
        float          m_Fraction          = 1.f;
        float          m_FractionLeftSolid = 1.f;
        /// Final trace position
        Vector3        m_EndPos            = 0.f;
        BSP::cplane_t* m_pPlane            = nullptr;
        int32_t        m_Contents          = 0;
        BSP::dbrush_t* m_pBrush            = nullptr;
        int32_t        m_nBrushSide        = 0;
    };

    class TraceRay
    {
    public:
        /**
         * @brief      Determines if visible.
         *
         * @param[in]  origin     The origin
         * @param[in]  final      The final
         * @param      pBSPFile   The bsp file
         *
         * @return     True if visible, False otherwise.
         */
        static bool is_visible( const Vector3& origin, const Vector3& final, BSPFile* pBSPFile );

        /**
         * @brief      Perform world trace.
         *
         * @param[in]  origin     The origin
         * @param[in]  final      The final point
         * @param      pBSPFile   The bsp file
         * @param      pTrace     The trace
         */
        static void ray_cast( const Vector3& origin, const Vector3& final, BSPFile* pBSPFile, trace_t* pTrace );

    protected:
        /**
         * @brief      Trace a bsp node.
         *
         * @param      pBSPFile        The bsp file
         * @param[in]  node_index      The node index
         * @param[in]  start_fraction  The start fraction
         * @param[in]  end_fraction    The end fraction
         * @param[in]  origin          The origin
         * @param[in]  final           The final point
         * @param      pTrace          The trace
         */
        static void ray_cast_node( BSPFile* pBSPFile, const int32_t node_index, const float start_fraction, const float end_fraction, const Vector3& origin, const Vector3& final, trace_t* pTrace );

        /**
         * @brief      Trace a bsp brush.
         *
         * @param      pBSPFile   The bsp file
         * @param      pBrush     The brush
         * @param      pTrace     The trace
         * @param[in]  origin     The origin
         * @param[in]  final      The final point
         */
        static void ray_cast_brush( BSPFile* pBSPFile, BSP::dbrush_t *pBrush, trace_t *pTrace, const Vector3& origin, const Vector3& final );

        /**
         * @brief      Trace a bsp surfaces.
         *
         * @param      pBSPFile       The bsp file
         * @param[in]  surface_index  The surface index
         * @param      pTrace         The trace
         * @param[in]  origin         The origin
         * @param[in]  final          The final point
         */
        static void ray_cast_surface( BSPFile* pBSPFile, const int32_t surface_index, trace_t *pTrace, const Vector3& origin, const Vector3& final );
    };
}
