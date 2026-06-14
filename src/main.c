\
#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "rus_assets.h"
#include "string.h"

RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_register_replacement(enum asset_e asset_id, void *asset_data));
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_register_replacement_with_size(enum asset_e asset_id, void *asset_data, u32 asset_size));
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_unregister_replacement(enum asset_e asset_id));
s32 g_bk_rus_font_style_classic = 0;

static void gothplay3r_bk_rus_update_font_style_config(void) {
    // Enum index: 0 = HD, 1 = Classic.
    g_bk_rus_font_style_classic = (recomp_get_config_u32("font_style") == 1);
}


static s32 s_bk_rus_registered = 0;

typedef struct BkRusPauseEntry {
    f32 delay;
    f32 unk4;
    u8 *str;
    s16 y;
    u8 portrait;
    u8 unkF;
} BkRusPauseEntry;

extern BkRusPauseEntry D_8036C4E0[4];

static u8 s_bk_rus_pause_return[] = "BEPHYT#C& B ]fPY";
static u8 s_bk_rus_pause_lair[]   = "B gOfOBO BE`#M<";
static u8 s_bk_rus_pause_totals[] = "O[_OP PE_Yg#TATOB";
static u8 s_bk_rus_pause_quit[]   = "COXPAH]T# ] B<iT]";

extern u8 *CONTROL_STICK_INSTRUCTIONS;
extern u8 *ERASE_INSTRUCTIONS;
extern u8 *ERASE_CONFIRMATION;
extern void gothplay3r_bk_rus_patch_parade_strings(void);

static void gothplay3r_bk_rus_patch_pause_strings(void) {
    D_8036C4E0[0].str = s_bk_rus_pause_return;
    D_8036C4E0[1].str = s_bk_rus_pause_lair;
    D_8036C4E0[2].str = s_bk_rus_pause_totals;
    D_8036C4E0[3].str = s_bk_rus_pause_quit;
}

static void gothplay3r_bk_rus_patch_game_select_strings(void) {
    CONTROL_STICK_INSTRUCTIONS = (u8*)"]CjOg#_YiTE CT]K `g& B<[OPA ]fP<.";
    ERASE_INSTRUCTIONS = (u8*)"A - ]fPAT#, Z - CTEPET#.";
    ERASE_CONFIRMATION = (u8*)"B< YBEPEH<? A - `A, B - HET.";
}

static s32 gothplay3r_bk_rus_should_skip_asset(s32 asset_id) {
    // HD full title-logo is handled by the stable RT64 block.
    // The working stable release needs the whole 0x054C-0x054F logo block skipped in HD.
    // Classic keeps the embedded low-res Russian logo assets.
    if ((asset_id >= 0x054C && asset_id <= 0x054F) && !g_bk_rus_font_style_classic) {
        return 1;
    }

    // HD score/totals letters must expose the native 0x06EC mask so RT64 catches the HD glyphs.
    // Classic may keep the embedded/AEP Russian 0x06EC path; the final Classic glyph chunks are
    // anti-hash-touched later in font_recolor_reset_patch.c so RT64 stops mistaking them for HD.
    if (asset_id == 0x06EC && !g_bk_rus_font_style_classic) {
        return 1;
    }

    // Keep original numeric bold mask (0x06ED) out of AEP, like stable.
    // HD needs the native numeric mask for RT64; Classic gets the same native look but receives
    // a tiny post-build alpha nudge to avoid matching the HD RT64 hashes.
    if (asset_id == 0x06ED) {
        return 1;
    }

    // Probe11 / mutable-map-model isolation.
    // Furnace Fun moves quiz-square meshes down with BKModel_transformMesh/func_8034DEB4
    // after answer/fail events. The AEP replacement pointer for ASSET_14E8_MODEL_GL_FURNACE_FUN_OPA
    // can be mutated and then reused after death/reload, making all quiz squares stay black/lowered
    // and walkable. Skip only this mutable OPA map model for diagnosis; text and HD fixes stay active.
    if (asset_id == 0x14E8) {
        return 1;
    }

    return 0;
}

RECOMP_HOOK_RETURN("assetCache_init")
void gothplay3r_bk_rus_v08_register_assets_after_asset_cache_init(void) {
    gothplay3r_bk_rus_update_font_style_config();
    if (s_bk_rus_registered) return;

    // Clear stale bold-font replacements before registering the Russian assets.
    // Skipping 0x06EC/0x06ED is not enough if AEP keeps a previously registered replacement
    // in the same Banjo: Recompiled process. Those two masks drive HUD variables/counters.
    bk_recomp_aep_unregister_replacement((enum asset_e)0x06EC);
    bk_recomp_aep_unregister_replacement((enum asset_e)0x06ED);

    for (s32 i = 0; i < g_bk_rus_asset_count; i++) {
        if (gothplay3r_bk_rus_should_skip_asset(g_bk_rus_assets[i].id)) {
            continue;
        }
        bk_recomp_aep_register_replacement((enum asset_e)g_bk_rus_assets[i].id,
                                           (void*)g_bk_rus_assets[i].data);
        bk_recomp_aep_register_replacement_with_size((enum asset_e)g_bk_rus_assets[i].id,
                                                     (void*)g_bk_rus_assets[i].data,
                                                     g_bk_rus_assets[i].size);
    }
    gothplay3r_bk_rus_patch_game_select_strings();
    gothplay3r_bk_rus_patch_pause_strings();
    gothplay3r_bk_rus_patch_parade_strings();
    s_bk_rus_registered = 1;
    recomp_printf("[Russian Language Mod 0.9.6] registered %d replacements\n", g_bk_rus_asset_count);
}
