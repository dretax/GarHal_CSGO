/**
 * @author     ReactiioN
 * @date       08.03.2016
 * @visit      https://github.com/ReactiioN1337
 *             https://reactiion.pw
 *             https://www.unknowncheats.me/forum/members/264622.html
 */
#pragma once
#include "BSPFlags.hpp"
#include "Matrix.hpp"

namespace hazedumper
{
    using std::array;
    using Vector3 = Matrix<float, 3, 1>;
    using Vector2 = Matrix<float, 2, 1>;
}

namespace hazedumper
{
    namespace BSP
    {
        enum eLumpIndex : size_t
        {
            LUMP_ENTITIES = 0,
            LUMP_PLANES = 1,
            LUMP_TEXDATA = 2,
            LUMP_VERTEXES = 3,
            LUMP_VISIBILITY = 4,
            LUMP_NODES = 5,
            LUMP_TEXINFO = 6,
            LUMP_FACES = 7,
            LUMP_LIGHTING = 8,
            LUMP_OCCLUSION = 9,
            LUMP_LEAFS = 10,
            LUMP_EDGES = 12,
            LUMP_SURFEDGES = 13,
            LUMP_MODELS = 14,
            LUMP_WORLDLIGHTS = 15,
            LUMP_LEAFFACES = 16,
            LUMP_LEAFBRUSHES = 17,
            LUMP_BRUSHES = 18,
            LUMP_BRUSHSIDES = 19,
            LUMP_AREAS = 20,
            LUMP_AREAPORTALS = 21,
            LUMP_PORTALS = 22,
            LUMP_CLUSTERS = 23,
            LUMP_PORTALVERTS = 24,
            LUMP_CLUSTERPORTALS = 25,
            LUMP_DISPINFO = 26,
            LUMP_ORIGINALFACES = 27,
            LUMP_PHYSCOLLIDE = 29,
            LUMP_VERTNORMALS = 30,
            LUMP_VERTNORMALINDICES = 31,
            LUMP_DISP_LIGHTMAP_ALPHAS = 32,
            LUMP_DISP_VERTS = 33,
            LUMP_DISP_LIGHTMAP_SAMPLE_POSITIONS = 34,
            LUMP_GAME_LUMP = 35,
            LUMP_LEAFWATERDATA = 36,
            LUMP_PRIMITIVES = 37,
            LUMP_PRIMVERTS = 38,
            LUMP_PRIMINDICES = 39,
            LUMP_PAKFILE = 40,
            LUMP_CLIPPORTALVERTS = 41,
            LUMP_CUBEMAPS = 42,
            LUMP_TEXDATA_STRING_DATA = 43,
            LUMP_TEXDATA_STRING_TABLE = 44,
            LUMP_OVERLAYS = 45,
            LUMP_LEAFMINDISTTOWATER = 46,
            LUMP_FACE_MACRO_TEXTURE_INFO = 47,
            LUMP_DISP_TRIS = 48
        };

        class lump_t
        {
        public:
            int32_t m_Fileofs; /// 0x0
            int32_t m_Filelen; /// 0x4
            int32_t m_Version; /// 0x8
            array<char, 4> m_FourCC; /// 0xC
        }; ///Size=0x10

        class dheader_t
        {
        public:
            int32_t m_Ident; /// 0x000
            int32_t m_Version; /// 0x004
            array<lump_t, HEADER_LUMPS> m_Lumps; /// 0x008
            int32_t m_MapRevision; /// 0x408
        }; ///Size=0x40C

        class dplane_t
        {
        public:
            Vector3 m_Normal; /// 0x00
            float m_Distance; /// 0x0C
            int32_t m_Type; /// 0x10
        }; ///Size=0x14

        class cplane_t
        {
        public:
            Vector3 m_Normal; /// 0x00
            float m_Distance; /// 0x0C
            uint8_t m_Type; /// 0x10
            uint8_t m_SignBits; /// 0x11
        private:
            uint8_t m_Pad[0x2]; /// 0x12
        }; ///Size=0x14

        class dedge_t
        {
        public:
            array<uint16_t, 2> m_V; /// 0x0
        }; ///Size=0x4

        class mvertex_t
        {
        public:
            Vector3 m_Position; /// 0x0
        }; ///Size=0xC

        class dleaf_t
        {
        public:
            int32_t m_Contents; /// 0x00
            int16_t m_Cluster; /// 0x04
            int16_t m_Area : 9; /// 0x06
            int16_t m_Flags : 7; /// 0x11
            array<int16_t, 3> m_Mins; /// 0x1A
            array<int16_t, 3> m_Maxs; /// 0x20
            uint16_t m_Firstleafface; /// 0x26
            uint16_t m_Numleaffaces; /// 0x28
            uint16_t m_Firstleafbrush; /// 0x2A
            uint16_t m_Numleafbrushes; /// 0x2C
            int16_t m_LeafWaterDataID; /// 0x2E
        }; ///Size=0x30

        class dnode_t
        {
        public:
            int32_t m_Planenum; /// 0x00
            array<int32_t, 2> m_Children; /// 0x04
            array<int16_t, 3> m_Mins; /// 0x0C
            array<int16_t, 3> m_Maxs; /// 0x12
            uint16_t m_Firstface; /// 0x18
            uint16_t m_Numfaces; /// 0x1A
            int16_t m_Area; /// 0x1C
        private:
            uint8_t m_Pad[0x2]; /// 0x1E
        }; ///Size=0x20

        class snode_t
        {
        public:
            int32_t m_PlaneNum; /// 0x00
            cplane_t* m_pPlane; /// 0x04
            array<int32_t, 2> m_Children; /// 0x08
            dleaf_t* m_LeafChildren; /// 0x10
            snode_t* m_NodeChildren; /// 0x14
            array<int16_t, 3> m_Mins; /// 0x18
            array<int16_t, 3> m_Maxs; /// 0x1E
            uint16_t m_Firstface; /// 0x24
            uint16_t m_Numfaces; /// 0x26
            int16_t m_Area; /// 0x28
            uint8_t m_Pad[0x2]; /// 0x2A
        }; ///Size=0x2C

        class dface_t
        {
        public:
            uint16_t m_Planenum; /// 0x00
            uint8_t m_Side; /// 0x02
            uint8_t m_OnNode; /// 0x03
            int32_t m_Firstedge; /// 0x04
            int16_t m_Numedges; /// 0x08
            int16_t m_Texinfo; /// 0x0A
            int16_t m_Dispinfo; /// 0x0C
            int16_t m_SurfaceFogVolumeID; /// 0x0E
            array<uint8_t, 4> m_Styles; /// 0x10
            int32_t m_Lightofs; /// 0x18
            float m_Area; /// 0x1C
            array<int32_t, 2> m_LightmapTextureMinsInLuxels; /// 0x20
            array<int32_t, 2> m_LightmapTextureSizeInLuxels; /// 0x28
            int32_t m_OrigFace; /// 0x30
            uint16_t m_NumPrims; /// 0x34
            uint16_t m_FirstPrimID; /// 0x36
            uint16_t m_SmoothingGroups; /// 0x38
        }; ///Size=0x3A

        class dbrush_t
        {
        public:
            int32_t m_Firstside; /// 0x0
            int32_t m_Numsides; /// 0x4
            int32_t m_Contents; /// 0x8
        }; ///Size=0xC

        class dbrushside_t
        {
        public:
            uint16_t m_Planenum; /// 0x0
            int16_t m_Texinfo; /// 0x2
            int16_t m_Dispinfo; /// 0x4
            uint8_t m_Bevel; /// 0x6
            uint8_t m_Thin; /// 0x7
        }; ///Size=0x8

        class texinfo_t
        {
        public:
            array<array<float, 4>, 2> m_TextureVecs; /// 0x00
            array<array<float, 4>, 2> m_LightmapVecs; /// 0x20
            int32_t m_Flags; /// 0x40
            int32_t m_Texdata; /// 0x44
        }; ///Size=0x48

        class VPlane
        {
        public:
            Vector3 m_Origin = 0.f;
            float m_Distance = 0.f;

        public:
            VPlane(void) = default;

            VPlane(const Vector3& origin, const float distance) :
                m_Origin(origin),
                m_Distance(distance)
            {
            }

            VPlane(const VPlane& other)
            {
                *this = other;
            }

            VPlane(VPlane&& other) noexcept
            {
                *this = other;
            }

            VPlane& operator =(const VPlane& other)
            {
                init(other.m_Origin, other.m_Distance);
                return *this;
            }

            VPlane& operator =(VPlane&& other) noexcept
            {
                init(other.m_Origin, other.m_Distance);
                return *this;
            }

            float dist_to(const Vector3& location) const
            {
                return m_Origin.dot(location) - m_Distance;
            }

            void init(const Vector3& origin, const float distance)
            {
                m_Origin = origin;
                m_Distance = distance;
            }
        };

        class Polygon
        {
        public:
            array<Vector3, MAX_SURFINFO_VERTS> m_Verts;
            size_t m_nVerts;
            VPlane m_Plane;
            array<VPlane, MAX_SURFINFO_VERTS> m_EdgePlanes;
            array<Vector3, MAX_SURFINFO_VERTS> m_Vec2D;
            int32_t m_Skip;
        };
    }
}
