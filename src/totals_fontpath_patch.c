#include <ultra64.h>
#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "string.h"

// Totals screen styled-font test.
// v0.8.1 uses the Russian bold-letter mask and native bold renderer again for headers/counters.

typedef struct GcZoombox GcZoombox;

typedef struct BkRusPauseEntry {
    f32 delay;
    f32 unk4;
    u8 *str;
    s16 y;
    u8 portrait;
    u8 unkF;
} BkRusPauseEntry;

typedef struct BkRusPauseTotalsEntry {
    s16 level_id;
    s16 x;
    u8 *string;
} BkRusPauseTotalsEntry;

extern BkRusPauseEntry D_8036C520[4];
extern BkRusPauseTotalsEntry D_8036C58C[0xD];
extern void gcpausemenu_zoombox_callback(s32, s32);
extern GcZoombox *gczoombox_new(s32 y, s32 portrait_id, s32 arg2, s32 arg3, void (*callback)(s32, s32));
extern void gczoombox_func_803184C8(GcZoombox *this, f32 arg1, s32 arg2, s32 arg3, f32 animation_duration, s32 arg5, s32 arg6);
extern void func_80318640(GcZoombox *this, s32 arg1, f32 arg2, f32 arg3, s32 arg4);
extern void func_80318760(GcZoombox *this, s32 arg1);
extern void func_80318734(GcZoombox *this, f32 arg1);
extern void print_dialog(s32 x, s32 y, u8 *string);
extern void print_bold_overlapping(s32 x, s32 y, f32 arg2, u8 *string);

extern void gcpausemenu_getLevelNoteScore(enum level_e level, s32 *valPtr, s32 *maxPtr);
extern void gcpausemenu_getLevelJiggyScore(enum level_e level, s32 *valPtr, s32 *maxPtr);
extern void gcpausemenu_getLevelHoneycombScore(enum level_e level, s32 *valPtr, s32 *maxPtr);
extern void gcpausemenu_getTotalNoteScore(s32 *dst);
extern void gcpausemenu_getTotalJiggyScore(s32 *dst);
extern void gcpausemenu_getTotalHoneycombScore(s32 *dst);
extern char *gcpausemenu_TimeToA(int time);
extern u16 itemscore_timeScores_get(enum level_e level_id);
extern s32 itemscore_timeScores_getTotal(void);

typedef struct PauseMenuShadow {
    u8  state;
    u8  menu;
    u8  selection;
    u8  flags3;
    s8  zoombox_opening_count;
    s8  zoombox_closing_count;
    u8  unk6;
    u8  unk7;
    s8  unk8;
    s8  page;
    s8  joystick_frame;
    u8  joystick_frame_count;
    f32 unkC;
    GcZoombox *zoombox[4];
} PauseMenuShadow;

extern PauseMenuShadow D_80383010;

static u8 s_totals_title_game[]      = "OD>OP PE>YUITATOB";   // ОБЗОР РЕЗУЛЬТАТОВ
static u8 s_totals_title_spiral[]    = "CVRPAUIHA) JOPA";     // СПИРАЛЬНАЯ ГОРА
static u8 s_totals_title_lair[]      = "UOJOBO JPYHTRUIG<";  // ЛОГОВО ГРУНТИЛЬДЫ
static u8 s_totals_title_mumbo[]     = "JOPA MAMDO";          // ГОРА МАМБО
static u8 s_totals_title_treasure[]  = "DYXTA COKPOBRW";      // БУХТА СОКРОВИЩ
static u8 s_totals_title_clanker[]   = "VEWEPA KUFHKEPA";     // ПЕЩЕРА КЛЭНКЕРА
static u8 s_totals_title_swamp[]     = "DYUIKAQWEE DOUOTO";  // БУЛЬКАЮЩЕЕ БОЛОТО
static u8 s_totals_title_freezeezy[] = "ODUEGEHEU<S VRK";    // ОБЛЕДЕНЕЛЫЙ ПИК
static u8 s_totals_title_gobi[]      = "GOURHA JODR";         // ДОЛИНА ГОБИ
static u8 s_totals_title_mansion[]   = "\"DE>YMH<S MOHCTP\""; // "БЕЗУМНЫЙ МОНСТР"
static u8 s_totals_title_rusty[]     = "\"PNABOE KOP<TO\"";   // "РЖАВОЕ КОРЫТО"
static u8 s_totals_title_click[]     = "UEC";                // ЛЕС
static u8 s_totals_title_sns[]       = "B<KUQ(ASKA-ODMEH)SKA"; // ВЫКЛЮЧАЙКА-ОБМЕНЯЙКА

static u8 *russian_totals_title(s32 page_id) {
    switch (page_id) {
        case 0: return s_totals_title_game;
        case 1: return s_totals_title_spiral;
        case 2: return s_totals_title_lair;
        case 3: return s_totals_title_mumbo;
        case 4: return s_totals_title_treasure;
        case 5: return s_totals_title_clanker;
        case 6: return s_totals_title_swamp;
        case 7: return s_totals_title_freezeezy;
        case 8: return s_totals_title_gobi;
        case 9: return s_totals_title_mansion;
        case 10: return s_totals_title_rusty;
        case 11: return s_totals_title_click;
        case 12: return s_totals_title_sns;
        default: return D_8036C58C[page_id].string;
    }
}

static s16 russian_totals_title_visual_width(u8 *str) {
    s16 width = 0;

    // Native Banjo bold titles behave close enough to a 14px fixed step here.
    // The known-good center reference is ГОРА МАМБО at x = 0x4A.
    while (*str != 0) {
        width += 14;
        str++;
    }

    return width;
}

static s16 russian_totals_title_x(s32 page_id) {
    u8 *title = russian_totals_title(page_id);
    s16 mumbo_width = russian_totals_title_visual_width(s_totals_title_mumbo);
    s16 title_width = russian_totals_title_visual_width(title);
    s16 center_x = 0x4A + (mumbo_width / 2);
    s16 x = center_x - (title_width / 2);

    // Keep very long headers from wrapping into negative screen coordinates.
    // This only affects the totals header position, not item counters or textures.
    if (x < 0x02) {
        x = 0x02;
    }

    return x;
}

static void rus_ratio_string(char *dst, s32 val, s32 max) {
    strcpy(dst, "");
    strIToA(dst, val);
    // Styled test: native bold font contains the slash glyph in the expected table slot.
    strcat(dst, "/");
    strIToA(dst, max);
}

RECOMP_FORCE_PATCH void gcpausemenu_zoomboxes_initTotalsMenu(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        D_80383010.zoombox[i] = gczoombox_new(D_8036C520[i].y, D_8036C520[i].portrait, (i == 3) ? 1 : 2, 0, gcpausemenu_zoombox_callback);
        gczoombox_func_803184C8(D_80383010.zoombox[i], 40.0f, 5, 2, 0.3f, 0, 0);
        // Styled test: native bold renderer for totals counters/time.
        func_80318640(D_80383010.zoombox[i], 0x46, 0.9f, 0.7f, 1);
        func_80318760(D_80383010.zoombox[i], 8000);
    }
    func_80318734(D_80383010.zoombox[3], 24.0f);
}

RECOMP_FORCE_PATCH void gcpausemenu_printTotalsHeader(s32 page_id) {
    // Styled test: Russian title encoded for the bold glyph table.
    print_bold_overlapping(russian_totals_title_x(page_id), D_80383010.unk8, -1.05f, russian_totals_title(page_id));
}

RECOMP_FORCE_PATCH void gcpausemenu_printLevelTotals(enum level_e level) {
    s32 val;
    s32 max;

    gcpausemenu_getLevelNoteScore(level, &val, &max);
    rus_ratio_string(D_8036C520[0].str, val, max);

    gcpausemenu_getLevelJiggyScore(level, &val, &max);
    rus_ratio_string(D_8036C520[1].str, val, max);

    gcpausemenu_getLevelHoneycombScore(level, &val, &max);
    rus_ratio_string(D_8036C520[2].str, val, max);

    strcpy(D_8036C520[3].str, "");
    strcat(D_8036C520[3].str, gcpausemenu_TimeToA(itemscore_timeScores_get(level)));
}

RECOMP_FORCE_PATCH void gcpausemenu_printTotals(void) {
    s32 val;

    gcpausemenu_getTotalNoteScore(&val);
    strcpy(D_8036C520[0].str, "");
    strIToA(D_8036C520[0].str, val);

    gcpausemenu_getTotalJiggyScore(&val);
    strcpy(D_8036C520[1].str, "");
    strIToA(D_8036C520[1].str, val);

    gcpausemenu_getTotalHoneycombScore(&val);
    strcpy(D_8036C520[2].str, "");
    strIToA(D_8036C520[2].str, val);

    strcpy(D_8036C520[3].str, "");
    strcat(D_8036C520[3].str, gcpausemenu_TimeToA(itemscore_timeScores_getTotal()));
}
