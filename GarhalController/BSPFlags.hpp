/**
 * @author     ReactiioN
 * @date       08.03.2016
 * @visit      https://github.com/ReactiioN1337
 *             https://reactiion.pw
 *             https://www.unknowncheats.me/forum/members/264622.html
 */
#pragma once
#include <stdint.h>

namespace hazedumper { namespace BSP {
#define IDBSPHEADER	( ( 'P' << 24 ) + ( 'S' << 16 ) + ( 'B' << 8 ) + 'V' )

    static constexpr int32_t MAX_BRUSH_LIGHTMAP_DIM_WITHOUT_BORDER   = 32;
    static constexpr int32_t MAX_BRUSH_LIGHTMAP_DIM_INCLUDING_BORDER = 35;
    static constexpr int32_t MAX_DISP_LIGHTMAP_DIM_WITHOUT_BORDER    = 128;
    static constexpr int32_t MAX_DISP_LIGHTMAP_DIM_INCLUDING_BORDER  = 131;
    static constexpr int32_t MAX_LIGHTMAP_DIM_WITHOUT_BORDER         = MAX_DISP_LIGHTMAP_DIM_WITHOUT_BORDER;
    static constexpr int32_t MAX_LIGHTMAP_DIM_INCLUDING_BORDER       = MAX_DISP_LIGHTMAP_DIM_INCLUDING_BORDER;

    static constexpr float   DIST_EPSILON              = 0.03125f;
    static constexpr size_t  MAX_SURFINFO_VERTS        = 32;
    static constexpr int32_t BSPVERSION                = 19;
    static constexpr size_t  HEADER_LUMPS              = 64;
    static constexpr size_t  MAX_POLYGONS              = 50120;
    static constexpr size_t  MAX_MOD_KNOWN             = 512;
    static constexpr size_t  MAX_MAP_MODELS            = 1024;
    static constexpr size_t  MAX_MAP_BRUSHES           = 8192;
    static constexpr size_t  MAX_MAP_ENTITIES          = 4096;
    static constexpr size_t  MAX_MAP_ENTSTRING         = 256 * 1024;
    static constexpr size_t  MAX_MAP_NODES             = 65536;
    static constexpr size_t  MAX_MAP_TEXINFO           = 12288;
    static constexpr size_t  MAX_MAP_TEXDATA           = 2048;
    static constexpr size_t  MAX_MAP_LEAFBRUSHES       = 65536;
    static constexpr size_t  MIN_MAP_DISP_POWER        = 2;
    static constexpr size_t  MAX_MAP_DISP_POWER        = 4;
    static constexpr size_t  MAX_MAP_SURFEDGES         = 512000;
    static constexpr size_t  MAX_DISP_CORNER_NEIGHBORS = 4;

    /// NOTE: These are stored in a short in the engine now.  Don't use more than 16 bits
    static constexpr int32_t SURF_LIGHT     = 0x0001; /// value will hold the light strength
    static constexpr int32_t SURF_SLICK     = 0x0002; /// effects game physics
    static constexpr int32_t SURF_SKY       = 0x0004; /// don't draw, but add to skybox
    static constexpr int32_t SURF_WARP      = 0x0008; /// turbulent water warp
    static constexpr int32_t SURF_TRANS     = 0x0010;
    static constexpr int32_t SURF_WET       = 0x0020; /// the surface is wet
    static constexpr int32_t SURF_FLOWING   = 0x0040; /// scroll towards angle
    static constexpr int32_t SURF_NODRAW    = 0x0080; /// don't bother referencing the texture
    static constexpr int32_t SURF_Hint32_t  = 0x0100; /// make a primary bsp splitter
    static constexpr int32_t SURF_SKIP      = 0x0200; /// completely ignore, allowing non-closed brushes
    static constexpr int32_t SURF_NOLIGHT   = 0x0400; /// Don't calculate light
    static constexpr int32_t SURF_BUMPLIGHT = 0x0800; /// calculate three lightmaps for the surface for bumpmapping
    static constexpr int32_t SURF_HITBOX    = 0x8000; /// surface is part of a hitbox

    static constexpr int32_t CONTENTS_EMPTY         = 0;           /// No contents
    static constexpr int32_t CONTENTS_SOLID         = 0x1;         /// an eye is never valid in a solid
    static constexpr int32_t CONTENTS_WINDOW        = 0x2;         /// translucent, but not watery (glass)
    static constexpr int32_t CONTENTS_AUX           = 0x4;
    static constexpr int32_t CONTENTS_GRATE         = 0x8;         /// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
    static constexpr int32_t CONTENTS_SLIME         = 0x10;
    static constexpr int32_t CONTENTS_WATER         = 0x20;
    static constexpr int32_t CONTENTS_MIST          = 0x40;
    static constexpr int32_t CONTENTS_OPAQUE        = 0x80;        /// things that cannot be seen through (may be non-solid though)
    static constexpr int32_t LAST_VISIBLE_CONTENTS  = 0x80;
    static constexpr int32_t ALL_VISIBLE_CONTENTS   = LAST_VISIBLE_CONTENTS | LAST_VISIBLE_CONTENTS - 1;
    static constexpr int32_t CONTENTS_TESTFOGVOLUME = 0x100;
    static constexpr int32_t CONTENTS_UNUSED3       = 0x200;
    static constexpr int32_t CONTENTS_UNUSED4       = 0x400;
    static constexpr int32_t CONTENTS_UNUSED5       = 0x800;
    static constexpr int32_t CONTENTS_UNUSED6       = 0x1000;
    static constexpr int32_t CONTENTS_UNUSED7       = 0x2000;
    static constexpr int32_t CONTENTS_MOVEABLE      = 0x4000;      /// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
    /// remaining contents are non-visible, and don't eat brushes
    static constexpr int32_t CONTENTS_AREAPORTAL    = 0x8000;
    static constexpr int32_t CONTENTS_PLAYERCLIP    = 0x10000;
    static constexpr int32_t CONTENTS_MONSTERCLIP   = 0x20000;
    /// currents can be added to any other contents, and may be mixed
    static constexpr int32_t CONTENTS_CURRENT_0     = 0x40000;
    static constexpr int32_t CONTENTS_CURRENT_90    = 0x80000;
    static constexpr int32_t CONTENTS_CURRENT_180   = 0x100000;
    static constexpr int32_t CONTENTS_CURRENT_270   = 0x200000;
    static constexpr int32_t CONTENTS_CURRENT_UP    = 0x400000;
    static constexpr int32_t CONTENTS_CURRENT_DOWN  = 0x800000;
    static constexpr int32_t CONTENTS_ORIGIN        = 0x1000000;   /// removed before bsping an entity
    static constexpr int32_t CONTENTS_MONSTER       = 0x2000000;   /// should never be on a brush, only in game
    static constexpr int32_t CONTENTS_DEBRIS        = 0x4000000;
    static constexpr int32_t CONTENTS_DETAIL        = 0x8000000;   /// brushes to be added after vis leafs
    static constexpr int32_t CONTENTS_TRANSLUCENT   = 0x10000000;  /// int32_t set if any surface has trans
    static constexpr int32_t CONTENTS_LADDER        = 0x20000000;
    static constexpr int32_t CONTENTS_HITBOX        = 0x40000000;  /// use accurate hitboxes on trace

                                                            /// everyhting
    static constexpr int32_t MASK_ALL                   = 0xFFFFFFFF;
    /// everything that is normally solid
    static constexpr int32_t MASK_SOLID                 = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE;
    /// everything that blocks player movement
    static constexpr int32_t MASK_PLAYERSOLID           = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE;
    /// blocks npc movement
    static constexpr int32_t MASK_NPCSOLID              = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE;
    /// water physics in these contents
    static constexpr int32_t MASK_WATER                 = CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME;
    /// everything that blocks line of sight
    static constexpr int32_t MASK_OPAQUE                = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_SLIME | CONTENTS_OPAQUE;
    /// bullets see these as solid
    static constexpr int32_t MASK_SHOT                  = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX;
    /// non-raycasted weapons see this as solid (includes grates)
    static constexpr int32_t MASK_SHOT_HULL             = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE;
    /// everything normally solid, except monsters (world+brush only)
    static constexpr int32_t MASK_SOLID_BRUSHONLY       = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE;
    /// everything normally solid for player movement, except monsters (world+brush only)
    static constexpr int32_t MASK_PLAYERSOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE;
    /// everything normally solid for npc movement, except monsters (world+brush only)
    static constexpr int32_t MASK_NPCSOLID_BRUSHONLY    = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE;
    /// just the world, used for route rebuilding
    static constexpr int32_t MASK_NPCWORLDSTATIC        = CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE;
    /// UNDONE: This is untested, any moving water
    static constexpr int32_t MASK_CURRENT               = CONTENTS_CURRENT_0 | CONTENTS_CURRENT_90 | CONTENTS_CURRENT_180 | CONTENTS_CURRENT_270 | CONTENTS_CURRENT_UP | CONTENTS_CURRENT_DOWN;
    static constexpr int32_t MASK_DEADSOLID             = CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_GRATE;
} }
