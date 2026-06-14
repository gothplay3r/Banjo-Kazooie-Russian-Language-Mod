#include <ultra64.h>
#include "core1/core1.h"
#include "functions.h"
#include "variables.h"
#include "modding.h"
#include "string.h"
#ifndef MIN
#define MIN(s,t) (((s)<(t))?(s):(t))
#endif
#ifndef MAX
#define MAX(s,t) (((s)>(t))?(s):(t))
#endif

typedef struct { s16 x; s16 y; s16 unk4; s16 unk6; u8 fmtString[8]; f32 unk10; u8 *string; u8 rgba[4]; } PrintBuffer;
typedef struct font_letter { BKSpriteTextureBlock *unk0; void *unk4; } FontLetter;
typedef struct { u8 unk0; u8 unk1; s8 unk2; s8 unk3; } Struct_6DA30_0_s;

extern Struct_6DA30_0_s D_80369000[];
extern s32 D_80369068[];
extern s32 D_80380AB0;
extern void *D_80380AB8[];
extern PrintBuffer *print_sCurrentPtr;
extern s32 D_80380AE8;
extern s32 D_80380AEC;
extern s32 D_80380AF0;
extern s32 D_80380AF4;
extern s32 D_80380AF8;
extern s32 D_80380AFC;
extern s32 D_80380B00;
extern s32 D_80380B04;
extern s32 print_sInFontFormatMode;
extern s32 D_80380B0C;
extern s32 D_80380B10;
extern s32 D_80380B14;
extern u8 D_80380B20[];
extern s8 D_80380F20[];
extern f32 D_80380FA0;
extern s32 g_bk_rus_font_style_classic;
extern s32 gFramebufferWidth;
extern s32 gFramebufferHeight;
extern int map_get(void);
extern BKSpriteTextureBlock *func_802F5494(s32 letterId, s32 *fontType);
extern void *func_802F55A8(u8 arg0);

// v38 active font metric tables. Classic uses original 0.9.7 logical sampling metrics.
static const s16 g_bk_rus_hd_draw_w[75] = {
    4, 6, 7, 8, 11, 8, 3, 5, 5, 7, 9, 4, 6, 4, 8, 7, 5, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4, 10, 10, 11, 7, 8, 8, 8, 8, 8, 7, 8, 9, 8, 7, 7, 8, 8, 10, 10, 8, 8, 11, 8, 8, 7, 8, 8, 11, 8, 8, 7, 8, 10, 8, 10, 7, 9, 8, 7, 7, 7, 10, 7, 8, 8, 8, 8, 10
};
static const s16 g_bk_rus_hd_draw_h[75] = {
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13
};
static const s16 g_bk_rus_hd_tex_w[75] = {
    16, 24, 28, 32, 44, 32, 12, 20, 20, 28, 36, 16, 24, 16, 32, 28, 20, 28, 28, 28, 28, 28, 28, 28, 28, 16, 16, 40, 40, 44, 28, 32, 32, 32, 32, 32, 28, 32, 36, 32, 28, 28, 32, 32, 40, 40, 32, 32, 44, 32, 32, 28, 32, 32, 44, 32, 32, 28, 32, 40, 32, 40, 28, 36, 32, 28, 28, 28, 40, 28, 32, 32, 32, 32, 40
};
static const s16 g_bk_rus_hd_tex_h[75] = {
    52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52
};
static const s16 g_bk_rus_cl_draw_w[75] = {
    4, 6, 9, 9, 10, 10, 4, 6, 6, 8, 9, 4, 8, 3, 9, 7, 6, 7, 7, 8, 7, 6, 8, 8, 6, 3, 4, 10, 10, 11, 8, 10, 9, 9, 8, 9, 8, 7, 11, 9, 3, 8, 9, 7, 10, 9, 10, 8, 10, 8, 9, 8, 8, 10, 11, 9, 9, 10, 8, 10, 8, 9, 8, 8, 8, 8, 8, 8, 7, 6, 8, 8, 8, 8, 8
};
static const s16 g_bk_rus_cl_draw_h[75] = {
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13
};
static const s16 g_bk_rus_cl_tex_w[75] = {
    4, 6, 9, 9, 10, 10, 4, 6, 6, 8, 9, 4, 8, 3, 9, 7, 6, 7, 7, 8, 7, 6, 8, 8, 6, 3, 4, 10, 10, 11, 8, 10, 9, 9, 8, 9, 8, 7, 11, 9, 3, 8, 9, 7, 10, 9, 10, 8, 10, 8, 9, 8, 8, 10, 11, 9, 9, 10, 8, 10, 8, 9, 8, 8, 8, 8, 8, 8, 7, 6, 8, 8, 8, 8, 8
};
static const s16 g_bk_rus_cl_tex_h[75] = {
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13
};


static s32 gothplay3r_bk_rus_is_hd_dialog_chunk(BKSpriteTextureBlock *t) {
    return (D_80380AE8 == 0 && t != 0 && t->w > t->x && t->h > t->y);
}

static s32 gothplay3r_bk_rus_is_dual_font_dialog_chunk(BKSpriteTextureBlock *t) {
    return (gothplay3r_bk_rus_is_hd_dialog_chunk(t) && t->h > t->y);
}

static s32 gothplay3r_bk_rus_valid_font_slot(s32 slot) {
    return slot >= 0 && slot < 75;
}

static f32 gothplay3r_bk_rus_draw_w(BKSpriteTextureBlock *t, s32 slot) {
    if (gothplay3r_bk_rus_is_dual_font_dialog_chunk(t) && gothplay3r_bk_rus_valid_font_slot(slot)) {
        return (f32)(g_bk_rus_font_style_classic ? g_bk_rus_cl_draw_w[slot] : g_bk_rus_hd_draw_w[slot]);
    }
    return (f32)t->x;
}

static f32 gothplay3r_bk_rus_draw_h(BKSpriteTextureBlock *t, s32 slot) {
    if (gothplay3r_bk_rus_is_dual_font_dialog_chunk(t) && gothplay3r_bk_rus_valid_font_slot(slot)) {
        return (f32)(g_bk_rus_font_style_classic ? g_bk_rus_cl_draw_h[slot] : g_bk_rus_hd_draw_h[slot]);
    }
    return (f32)t->y;
}

static f32 gothplay3r_bk_rus_tex_w(BKSpriteTextureBlock *t, s32 slot) {
    if (gothplay3r_bk_rus_is_dual_font_dialog_chunk(t) && gothplay3r_bk_rus_valid_font_slot(slot)) {
        return (f32)(g_bk_rus_font_style_classic ? g_bk_rus_cl_tex_w[slot] : g_bk_rus_hd_tex_w[slot]);
    }
    return (f32)((gothplay3r_bk_rus_is_hd_dialog_chunk(t)) ? t->w : t->x);
}

static f32 gothplay3r_bk_rus_tex_h(BKSpriteTextureBlock *t, s32 slot) {
    if (gothplay3r_bk_rus_is_dual_font_dialog_chunk(t) && gothplay3r_bk_rus_valid_font_slot(slot)) {
        return (f32)(g_bk_rus_font_style_classic ? g_bk_rus_cl_tex_h[slot] : g_bk_rus_hd_tex_h[slot]);
    }
    return (f32)((gothplay3r_bk_rus_is_hd_dialog_chunk(t)) ? t->h : t->y);
}

static s32 gothplay3r_bk_rus_tex_offset_bytes(BKSpriteTextureBlock *t, s32 slot) {
    if (gothplay3r_bk_rus_is_dual_font_dialog_chunk(t) &&
        gothplay3r_bk_rus_valid_font_slot(slot) &&
        g_bk_rus_font_style_classic) {
        return t->w * g_bk_rus_hd_tex_h[slot];
    }
    return 0;
}

// Small HD dialog-only placement nudge to visually match Classic better.
// This affects only the dialog font path (not Classic, not score/totals, not title/logo).
static f32 gothplay3r_bk_rus_hd_dialog_x_nudge(BKSpriteTextureBlock *t) {
    return (!g_bk_rus_font_style_classic && gothplay3r_bk_rus_is_hd_dialog_chunk(t)) ? 0.75f : 0.0f;
}

static f32 gothplay3r_bk_rus_hd_dialog_y_nudge(BKSpriteTextureBlock *t) {
    return (!g_bk_rus_font_style_classic && gothplay3r_bk_rus_is_hd_dialog_chunk(t)) ? 0.25f : 0.0f;
}


// HD-only C-button direction renderer override.
// Shared text assets must keep raw C-UP/C-DOWN/C-LEFT/C-RIGHT so Classic stays correct.
// In HD dialog font only, remap the visible direction letters during drawing without changing string length.
// Probe4: old draw-time C-direction Cyrillic rewrite is disabled.
// Direction labels are now handled by push-time HD Latin markers below,
// so C-UP/C-DOWN/C-LEFT/C-RIGHT display as real Latin gothtoon glyphs in HD.
static u8 s_bk_rus_hd_cdir_state = 0;

static char gothplay3r_bk_rus_hd_cdir_remap_char(char letter) {
    s_bk_rus_hd_cdir_state = 0;
    return letter;
}

static f32 gothplay3r_bk_rus_hd_dialog_spacing_nudge(BKSpriteTextureBlock *t) {
    return (!g_bk_rus_font_style_classic && gothplay3r_bk_rus_is_hd_dialog_chunk(t)) ? 0.12f : 0.0f;
}

// HD-only Latin fallback probe for Cheato code words.
// The shared text assets keep original English bytes for cheat-code logic.
// In HD mode only, exact token spans are drawn from a separate gothtoon Latin bank,
// instead of reusing the Russian ASCII-slot glyphs. Classic remains untouched.
#define BK_RUS_HD_LATIN_STRIDE_W 44
#define BK_RUS_HD_LATIN_TEX_H 52
#define BK_RUS_HD_LATIN_DRAW_H 13
#define BK_RUS_HD_LATIN_MARKER_BASE 0x80
#define BK_RUS_HD_LATIN_MARKER_END  0x9A
extern BKSpriteTextureBlock *gothplay3r_bk_rus_hd_latin_block(void);
extern void *gothplay3r_bk_rus_hd_latin_pixels(s32 idx);
extern s32 gothplay3r_bk_rus_hd_latin_draw_w(s32 idx);
extern s32 gothplay3r_bk_rus_hd_latin_tex_w(s32 idx);

static u8 *s_bk_rus_hd_latin_current_string = 0;
static s32 s_bk_rus_hd_latin_current_index = 0;

static s32 gothplay3r_bk_rus_hd_latin_token_len(const char *token) {
    s32 i = 0;
    while (token[i] != 0) i++;
    return i;
}

static s32 gothplay3r_bk_rus_hd_latin_match_at(u8 *s, s32 start, const char *token) {
    s32 i;
    if (s == 0 || start < 0) return 0;
    for (i = 0; token[i] != 0; i++) {
        if (s[start + i] != (u8)token[i]) return 0;
    }
    return 1;
}

static s32 gothplay3r_bk_rus_hd_latin_index_for_token(u8 *s, s32 index, const char *token, u8 raw_letter) {
    s32 len = gothplay3r_bk_rus_hd_latin_token_len(token);
    s32 start;
    if (raw_letter < 'A' || raw_letter > 'Z') return -1;
    for (start = index - len + 1; start <= index; start++) {
        if (start <= index && index < start + len && gothplay3r_bk_rus_hd_latin_match_at(s, start, token)) {
            return raw_letter - 'A';
        }
    }
    return -1;
}

static s32 gothplay3r_bk_rus_hd_latin_marker_index(u8 raw_letter) {
    return (raw_letter >= BK_RUS_HD_LATIN_MARKER_BASE && raw_letter < BK_RUS_HD_LATIN_MARKER_END) ?
        (raw_letter - BK_RUS_HD_LATIN_MARKER_BASE) : -1;
}

static s32 gothplay3r_bk_rus_hd_latin_cheato_index(u8 raw_letter) {
    u8 *s;
    s32 index;
    s32 result;

    if (g_bk_rus_font_style_classic || D_80380AE8 != 0 || print_sCurrentPtr == 0 || print_sCurrentPtr->string == 0) {
        return -1;
    }

    s = print_sCurrentPtr->string;
    if (s_bk_rus_hd_latin_current_string != s) {
        s_bk_rus_hd_latin_current_string = s;
        s_bk_rus_hd_latin_current_index = 0;
    }

    if (raw_letter == 0) {
        s_bk_rus_hd_latin_current_index = 0;
        return -1;
    }

    // Format-control calls are made before/after the real string. Do not count them as text.
    if (raw_letter == 0xFD || print_sInFontFormatMode) {
        return -1;
    }

    if (s[s_bk_rus_hd_latin_current_index] != raw_letter) {
        return -1;
    }

    index = s_bk_rus_hd_latin_current_index;
    s_bk_rus_hd_latin_current_index++;

    result = gothplay3r_bk_rus_hd_latin_index_for_token(s, index, "BLUEEGGS", raw_letter);
    if (result >= 0) return result;
    result = gothplay3r_bk_rus_hd_latin_index_for_token(s, index, "REDFEATHERS", raw_letter);
    if (result >= 0) return result;
    result = gothplay3r_bk_rus_hd_latin_index_for_token(s, index, "GOLDFEATHERS", raw_letter);
    if (result >= 0) return result;

    return -1;
}


RECOMP_FORCE_PATCH void _printbuffer_draw_letter(char letter, f32* xPtr, f32* yPtr, f32 arg3, Gfx **gfx, Mtx **mtx, Vtx **vtx){
    static f32 D_80380FA0;
    
    // u8 letter = arg0;
    BKSpriteTextureBlock *sp214;
    s32 sp210;
    s32 sp20C;
    s32 t0;
    s8 t1;
    f32 sp200;
    f32 f28;    
    f32 sp1F8;
    s32 sp1F4; //font_type;

    int i;



    t0 = 0;
    sp200 = *xPtr;
    f28 = *yPtr;
    t1 = 0;

    if(!D_80380B04 && !letter){
        D_80380FA0 = 0.0f;
    }//L802F563C

    u8 bk_rus_raw_letter = (u8)letter;
    s32 bk_rus_hd_latin_marker_idx = gothplay3r_bk_rus_hd_latin_marker_index(bk_rus_raw_letter);
    s32 bk_rus_hd_latin_idx = (bk_rus_hd_latin_marker_idx >= 0) ?
        bk_rus_hd_latin_marker_idx : gothplay3r_bk_rus_hd_latin_cheato_index(bk_rus_raw_letter);

    if (bk_rus_hd_latin_marker_idx < 0) {
        letter = gothplay3r_bk_rus_hd_cdir_remap_char(letter);
    }

    switch(D_80380AE8){
        case 0: //L802F5678
            if (bk_rus_hd_latin_marker_idx >= 0) {
                sp20C = 0;
                t0 = 1;
            }
            else if(letter >= '\x21' && letter < '\x6c'){
                sp20C = letter - '\x21';
                t0 = 1;
            }
            break;
        case 1: //L802F56A0
            if(letter < '\x80' && D_80380F20[letter] >= 0){
                for(i = 0; D_80369000[i].unk0 != 0; i++){
                    if(letter == D_80369000[i].unk1 && D_80380AB0 == D_80369000[i].unk0){
                        t1 = D_80369000[i].unk3;
                        break;
                    }
                }//L802F5710
                sp20C = D_80380F20[letter];
                t0 = 1;
                D_80380AB0 = letter;
                f28 += (f32)t1*arg3;
            }//L802F5738
            break;
        case 2: //L802F5740
            sp20C = letter;
            if(D_80380B04){
                t0 = 1;
                sp20C += (D_80380B04 << 8) - 0x100;
                D_80380B04 = 0;
            }
            else{//L802F5764
                if(sp20C > 0 && sp20C < 0xfD)
                    t0 = 1;
            }
            break;
    }//L802F5778

    if(!t0 || print_sInFontFormatMode){
        print_sInFontFormatMode = FALSE;
        switch(letter){
            case ' '://802F5818
                *xPtr += ((D_80380AF0) ? D_80369068[D_80380AE8]: D_80369068[D_80380AE8]*0.8) * arg3;
                break;

            case 'b': //L802F5890
                //toggle background
                D_80380B00  = D_80380B00 ^ 1;
                break;

            case 'f': //L802F58A8
                D_80380AEC = D_80380AE8 = D_80380AE8 ^ 1;
                break;

            case 'l': //L802F58BC
                D_80380B10 = 0;
                break;

            case 'h': //L802F58C8
                D_80380B10 = 1;
                break;

            case 'j': //L802F58D4
                if(D_80380AFC == 0){
                    D_80380AFC = 1;
                    D_80380AEC = D_80380AE8;
                    D_80380AE8 = 2;
                    // D_80380AE8 = 2;
                }
                break;

            case 'e': //L802F58FC
                if(D_80380AFC){
                    D_80380AFC = 0;
                    D_80380AE8 = D_80380AEC;
                }
                break;

            case 'p': //L802F5924
                D_80380AF0 = D_80380AF0 ^1;
                break;

            case 'q': //L802F593C
                D_80380B14 = D_80380B14^1;
                if(D_80380B14){
                    gDPSetTextureFilter((*gfx)++, G_TF_POINT);
                }
                else{//L802F5978
                    gDPSetTextureFilter((*gfx)++, G_TF_BILERP);
                }
                break;

            case 'v': //L802F59A0 
                //toggle letter gradient
                D_80380AF4 ^= 1;
                if(D_80380AF4){
                    viewport_setRenderViewportAndOrthoMatrix(gfx, mtx);
                    gDPPipeSync((*gfx)++);
                    gDPSetTexturePersp((*gfx)++, G_TP_PERSP);
                    gDPSetPrimColor((*gfx)++, 0, 0, 0x00, 0x00, 0x00, 0xFF);
                    gDPSetCombineLERP((*gfx)++, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0);
                }
                else{//L802F5A44
                    gDPSetCombineMode((*gfx)++, G_CC_DECALRGBA, G_CC_DECALRGBA);
                    gDPSetTexturePersp((*gfx)++, G_TP_NONE);
                }
                break;

            case 'd': //L802F5A8C
                D_80380AF8 ^= 1;
                if(D_80380AF8){
                    gDPPipeSync((*gfx)++);
                    gDPSetCycleType((*gfx)++, G_CYC_2CYCLE);
                    gDPSetRenderMode((*gfx)++, G_RM_PASS, G_RM_XLU_SURF2);
                    gDPSetTextureLOD((*gfx)++, G_TL_TILE);
                    gDPSetCombineLERP((*gfx)++, 0, 0, 0, TEXEL0, TEXEL0, 0, TEXEL1, 0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
                }
                else{//L802F5B48
                    gDPPipeSync((*gfx)++);
                    gDPSetCombineMode((*gfx)++, G_CC_DECALRGBA, G_CC_DECALRGBA);
                    gDPSetCycleType((*gfx)++, G_CYC_1CYCLE);
                    gDPSetTextureLOD((*gfx)++, G_TL_LOD);
                    gDPSetRenderMode((*gfx)++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
                }
                break;

            case 0xfd: //L802F5BEC
                print_sInFontFormatMode = TRUE;
                break;

            case 0xfe://L802F5BF4
                D_80380B04 = 1;
                break;

            case 0xff://L802F5BFC
                D_80380B04 = 2;
                break;
            default:
                break;
        }
    }
    else{//L802F5C08
        sp214 = func_802F5494(sp20C, &sp1F4);
        if (D_80380B10 != 0) {
               sp200 += randf2(-2.0f, 2.0f);
               f28 += randf2(-2.0f, 2.0f);
        }
        s32 bk_rus_use_hd_latin = (!g_bk_rus_font_style_classic && D_80380AE8 == 0 && bk_rus_hd_latin_idx >= 0 && (bk_rus_hd_latin_marker_idx >= 0 || gothplay3r_bk_rus_is_hd_dialog_chunk(sp214)));
        f32 bk_rus_draw_w = bk_rus_use_hd_latin ? (f32)gothplay3r_bk_rus_hd_latin_draw_w(bk_rus_hd_latin_idx) : gothplay3r_bk_rus_draw_w(sp214, sp20C);
        f32 bk_rus_draw_h = bk_rus_use_hd_latin ? (f32)BK_RUS_HD_LATIN_DRAW_H : gothplay3r_bk_rus_draw_h(sp214, sp20C);
        f32 bk_rus_tex_w = bk_rus_use_hd_latin ? (f32)gothplay3r_bk_rus_hd_latin_tex_w(bk_rus_hd_latin_idx) : gothplay3r_bk_rus_tex_w(sp214, sp20C);
        f32 bk_rus_tex_h = bk_rus_use_hd_latin ? (f32)BK_RUS_HD_LATIN_TEX_H : gothplay3r_bk_rus_tex_h(sp214, sp20C);

        if (bk_rus_use_hd_latin) {
            sp1F4 = SPRITE_TYPE_IA8;
        }

        sp1F8 = (D_80380AF0 != 0) ? D_80369068[D_80380AE8] : bk_rus_draw_w;

        // temp_f2 = D_80380FA0;
        // phi_f2 = temp_f2;
        if (D_80380FA0 == 0.0f) {
            D_80380FA0 = -sp1F8 * 0.5;
        }

        sp200 += (D_80380FA0 + (sp1F8 - bk_rus_draw_w) * 0.5);
        sp200 += gothplay3r_bk_rus_hd_dialog_x_nudge(sp214);
        f28 -= bk_rus_draw_h*0.5;
        f28 += gothplay3r_bk_rus_hd_dialog_y_nudge(sp214);
        if (bk_rus_use_hd_latin) {
            sp214 = gothplay3r_bk_rus_hd_latin_block();
            sp210 = (s32)gothplay3r_bk_rus_hd_latin_pixels(bk_rus_hd_latin_idx);
        } else {
            sp210 = (s32)(sp214 + 1);
            while(sp210 % 8){
                sp210++;
            }
        }

        f32 bk_rus_s_step = (g_bk_rus_font_style_classic) ? (1024.0f / arg3) : ((bk_rus_draw_w != 0.0f) ? (1024.0f * (bk_rus_tex_w / bk_rus_draw_w) / arg3) : (1024.0f / arg3));
        f32 bk_rus_t_step = (g_bk_rus_font_style_classic) ? (1024.0f / arg3) : ((bk_rus_draw_h != 0.0f) ? (1024.0f * (bk_rus_tex_h / bk_rus_draw_h) / arg3) : (1024.0f / arg3));
        s32 bk_rus_hd_scroll_mask = ((bk_rus_use_hd_latin || gothplay3r_bk_rus_is_hd_dialog_chunk(sp214)) && D_80380AF8 != 0);

        if (!bk_rus_use_hd_latin) {
            sp210 += gothplay3r_bk_rus_tex_offset_bytes(sp214, sp20C);
        }

        if (bk_rus_use_hd_latin || gothplay3r_bk_rus_is_hd_dialog_chunk(sp214)) {
            gDPSetTextureFilter((*gfx)++, g_bk_rus_font_style_classic ? G_TF_POINT : G_TF_BILERP);
        }
        if (sp1F4 == SPRITE_TYPE_RGBA32) { 
            gDPLoadTextureTile((*gfx)++, sp210, G_IM_FMT_RGBA, G_IM_SIZ_32b, sp214->w, sp214->h, 0, 0, sp214->x-1, sp214->y - 1, NULL, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        } else if (sp1F4 == SPRITE_TYPE_IA8) {
            s32 bk_rus_texture_stride_w = bk_rus_use_hd_latin ? BK_RUS_HD_LATIN_STRIDE_W : sp214->w;
            gDPLoadTextureTile((*gfx)++, sp210, G_IM_FMT_IA, G_IM_SIZ_8b, bk_rus_texture_stride_w, (s32)bk_rus_tex_h, 0, 0, (s32)bk_rus_tex_w - 1, (s32)bk_rus_tex_h - 1, NULL, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        } else if (sp1F4 == SPRITE_TYPE_I8) {
            gDPLoadTextureTile((*gfx)++, sp210, G_IM_FMT_I, G_IM_SIZ_8b, sp214->w, sp214->h, 0, 0, sp214->x-1, sp214->y - 1, NULL, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        } else if (sp1F4 == SPRITE_TYPE_I4) {
            gDPLoadTextureTile_4b((*gfx)++, sp210, G_IM_FMT_I, sp214->w, sp214->h, 0, 0, sp214->x-1, sp214->y-1, NULL, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        } else if (sp1F4 == SPRITE_TYPE_CI8) {
            void * pal = func_802F55A8(sp20C);
            gDPLoadTLUT_pal256((*gfx)++, pal);
            gDPLoadTextureTile((*gfx)++, sp210, G_IM_FMT_CI, G_IM_SIZ_8b, sp214->w, sp214->h, 0, 0, sp214->x-1, sp214->y-1, NULL, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            gDPSetTextureLUT((*gfx)++, G_TT_RGBA16);
        }//L802F6570

        if (bk_rus_hd_scroll_mask) {
            // For HD dialog glyphs in the scrolling "delq" path, bypass the special
            // d-mask render branch and draw normally. This avoids underline-like
            // artifacts under lower-heavy glyphs during upward text movement.
            gDPPipeSync((*gfx)++);
            gDPSetCombineMode((*gfx)++, G_CC_DECALRGBA, G_CC_DECALRGBA);
            gDPSetCycleType((*gfx)++, G_CYC_1CYCLE);
            gDPSetTextureLOD((*gfx)++, G_TL_LOD);
            gDPSetRenderMode((*gfx)++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        }

        if (D_80380AF8 != 0 && !bk_rus_hd_scroll_mask) {
            s32 temp_t1;
            s32 phi_a0;
            temp_t1 = ((print_sCurrentPtr->unk4 - print_sCurrentPtr->y) - D_80380B0C) + 1;
            phi_a0 =  - MAX(1 - D_80380B0C, MIN(0, temp_t1));
            
            gDPSetTextureImage((*gfx)++, G_IM_FMT_I, G_IM_SIZ_8b, 32, &D_80380B20);
            gDPSetTile((*gfx)++, G_IM_FMT_I, G_IM_SIZ_8b, ((s32)bk_rus_draw_w + 8) >> 3, 0x0100, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPLoadSync((*gfx)++);
            gDPLoadTile((*gfx)++, G_TX_LOADTILE, 0 << G_TEXTURE_IMAGE_FRAC, (phi_a0) << G_TEXTURE_IMAGE_FRAC, ((s32)bk_rus_draw_w) << G_TEXTURE_IMAGE_FRAC, (D_80380B0C - 1) << G_TEXTURE_IMAGE_FRAC);
            gDPPipeSync((*gfx)++);
            gDPSetTile((*gfx)++, G_IM_FMT_I, G_IM_SIZ_8b, ((((s32)bk_rus_draw_w) - 0 + 1)*G_IM_SIZ_8b_LINE_BYTES + 7) >> 3, 0x0100, 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPSetTileSize((*gfx)++, 1, 0 << G_TEXTURE_IMAGE_FRAC, (MAX(0, temp_t1) + phi_a0) << G_TEXTURE_IMAGE_FRAC, ((s32)bk_rus_draw_w) << G_TEXTURE_IMAGE_FRAC, (MAX(0, temp_t1) - (1 - D_80380B0C))<<G_TEXTURE_IMAGE_FRAC);
            
            // gDPLoadMultiTile((*gfx)++, &D_80380B20,)
            
        }//L802F677C
        if (D_80380AF4 != 0) {
            f32 temp_f24;
            f32 spD0;
            f32 ix;
            f32 iy;
            f32 temp_f26;
            f32 spC0;

            temp_f24 = (bk_rus_draw_w - 1.0);
            spD0 = bk_rus_draw_h - 1.0;
            temp_f26 = (f64) sp200 - (f32) gFramebufferWidth * 0.5;
            spC0 = (f64)f28 - (f32)gFramebufferHeight*0.5 -0.5f;
            gSPVertex((*gfx)++, *vtx, 4, 0);
            for(iy = 0.0f; iy < 2.0; iy+= 1.0){
                for(ix = 0.0f; ix < 2.0; ix += 1.0){
                    s32 s = (ix * (bk_rus_tex_w - 1.0f) * 64.0f);
                    (*vtx)->v.ob[0] = (s16)(s32)((f64) (temp_f26 + (temp_f24 *  arg3  * ix)) * 4.0);
                    {
                        s32 t = (iy * (bk_rus_tex_h - 1.0f) * 64.0f);
                        (*vtx)->v.ob[1] = (s16) (s32) ((f64) (spC0 + (spD0 * arg3 * iy)) * -4.0);
                        (*vtx)->v.ob[2] = -0x14;
                        (*vtx)->v.tc[0] = s;
                        (*vtx)->v.tc[1] = t;
                    }
                    (*vtx)->v.cn[3] =(iy != 0.0f) ? print_sCurrentPtr->unk6 : print_sCurrentPtr->unk4;
                    
                    (*vtx)++;
                }    
            }
            
            gSP1Quadrangle((*gfx)++, 0, 1, 3, 2, 0);
        }
        else{
            gSPScisTextureRectangle((*gfx)++, (s32)(sp200*4.0f), (s32)(f28*4.0f), (s32)((sp200 + bk_rus_draw_w*arg3)*4.0f), (s32)((f28 + bk_rus_draw_h*arg3)*4.0f), 0, 0, 0, (s32)bk_rus_s_step, (s32)bk_rus_t_step);
        }
        *xPtr += (sp1F8 + gothplay3r_bk_rus_hd_dialog_spacing_nudge(sp214)) * arg3;
    }

}

// Probe4: push-time HD Latin marker remap.
// Cheato code words and technical labels use the separate Latin fallback glyph bank.
// C-UP/C-DOWN/C-LEFT/C-RIGHT/R are no longer rewritten to Cyrillic lookalikes.
// Real shared asset bytes stay untouched so Classic keeps its original control labels.
static s32 gothplay3r_bk_rus_hd_latin_token_starts(u8 *s, s32 pos, const char *token) {
    s32 i;
    if (s == 0 || token == 0 || pos < 0) return 0;
    for (i = 0; token[i] != 0; i++) {
        if (s[pos + i] != (u8)token[i]) return 0;
    }
    return 1;
}

static s32 gothplay3r_bk_rus_hd_latin_token_len2(const char *token) {
    s32 i = 0;
    while (token[i] != 0) i++;
    return i;
}

static s32 gothplay3r_bk_rus_hd_latin_token_prefix_to_string_end(u8 *s, s32 pos, const char *token) {
    s32 i;
    if (s == 0 || token == 0 || pos < 0) return 0;
    for (i = 0; s[pos + i] != 0; i++) {
        if (token[i] == 0 || s[pos + i] != (u8)token[i]) return 0;
    }
    return (i > 0) ? i : 0;
}

static s32 gothplay3r_bk_rus_hd_latin_token_at(u8 *s, s32 pos) {
    s32 len;
    if (gothplay3r_bk_rus_hd_latin_token_starts(s, pos, "BLUEEGGS")) return gothplay3r_bk_rus_hd_latin_token_len2("BLUEEGGS");
    if (gothplay3r_bk_rus_hd_latin_token_starts(s, pos, "REDFEATHERS")) return gothplay3r_bk_rus_hd_latin_token_len2("REDFEATHERS");
    if (gothplay3r_bk_rus_hd_latin_token_starts(s, pos, "GOLDFEATHERS")) return gothplay3r_bk_rus_hd_latin_token_len2("GOLDFEATHERS");

    // Zoombox may feed a typewriter/reveal prefix first: "R", "RE", "RED"...
    // If the remaining string is a prefix of a known Cheato word, remap it too so
    // the word does not appear as Cyrillic garbage while it is being revealed.
    len = gothplay3r_bk_rus_hd_latin_token_prefix_to_string_end(s, pos, "BLUEEGGS");
    if (len > 0) return len;
    len = gothplay3r_bk_rus_hd_latin_token_prefix_to_string_end(s, pos, "REDFEATHERS");
    if (len > 0) return len;
    len = gothplay3r_bk_rus_hd_latin_token_prefix_to_string_end(s, pos, "GOLDFEATHERS");
    if (len > 0) return len;

    // Technical control labels must stay Latin in HD. These are exact spans
    // inside a full print-buffer string; no global ASCII slot rewrite.
    if (gothplay3r_bk_rus_hd_latin_token_starts(s, pos, "C-UP")) return gothplay3r_bk_rus_hd_latin_token_len2("C-UP");
    if (gothplay3r_bk_rus_hd_latin_token_starts(s, pos, "C-DOWN")) return gothplay3r_bk_rus_hd_latin_token_len2("C-DOWN");
    if (gothplay3r_bk_rus_hd_latin_token_starts(s, pos, "C-LEFT")) return gothplay3r_bk_rus_hd_latin_token_len2("C-LEFT");
    if (gothplay3r_bk_rus_hd_latin_token_starts(s, pos, "C-RIGHT")) return gothplay3r_bk_rus_hd_latin_token_len2("C-RIGHT");

    return 0;
}

static s32 gothplay3r_bk_rus_hd_single_r_token_at(u8 *s, s32 pos) {
    // Do not remap every bounded R/P. In this encoding raw ASCII slots are also
    // Russian letters in existing assets. Only explicitly known button-token shapes
    // are safe:
    //   1) the full print-buffer string is literally R/P;
    //   2) the old camera tutorial workaround encoded R as quoted "P" inside text.
    //      In HD that quoted "P" must now draw as Latin R via fallback.
    // This avoids global P->R damage in normal Russian text. Humans invented shared
    // byte slots; now we get to mop the floor.
    if (s == 0 || pos < 0) return 0;
    if (pos == 0 && (s[0] == 'R' || s[0] == 'P') && s[1] == 0) return 1;
    if (s[pos] == 'P' && pos > 0 && s[pos - 1] == '"' && s[pos + 1] == '"') return 1;
    if (s[pos] == 'R' && pos > 0 && s[pos - 1] == '"' && s[pos + 1] == '"') return 1;
    return 0;
}

static s32 gothplay3r_bk_rus_hd_cdir_token_at(u8 *s, s32 pos, const char **replacement) {
    // Probe4 keeps this stub only so old call sites stay harmless.
    // Direction tokens are markerized by gothplay3r_bk_rus_hd_latin_token_at().
    if (replacement) *replacement = 0;
    return 0;
}

static u8 *gothplay3r_bk_rus_hd_latin_remap_cheato_tokens(u8 *s) {
    static u8 scratch[8][256];
    static s32 slot = 0;
    s32 i;
    s32 j;
    s32 len;
    s32 changed = 0;
    u8 *out;
    const char *replacement;

    if (s == 0 || g_bk_rus_font_style_classic) return s;

    for (i = 0; s[i] != 0; i++) {
        if (gothplay3r_bk_rus_hd_latin_token_at(s, i) != 0 ||
            gothplay3r_bk_rus_hd_cdir_token_at(s, i, 0) != 0 ||
            gothplay3r_bk_rus_hd_single_r_token_at(s, i)) {
            changed = 1;
            break;
        }
    }
    if (!changed) return s;

    slot = (slot + 1) & 7;
    out = scratch[slot];

    for (i = 0, j = 0; s[i] != 0 && j < 255; ) {
        len = gothplay3r_bk_rus_hd_latin_token_at(s, i);
        if (len > 0) {
            s32 k;
            for (k = 0; k < len && j < 255; k++) {
                u8 c = s[i + k];
                if (c >= 'A' && c <= 'Z') {
                    out[j++] = (u8)(BK_RUS_HD_LATIN_MARKER_BASE + (c - 'A'));
                } else {
                    out[j++] = c;
                }
            }
            i += len;
            continue;
        }

        replacement = 0;
        len = gothplay3r_bk_rus_hd_cdir_token_at(s, i, &replacement);
        if (len > 0 && replacement != 0) {
            s32 k;
            for (k = 0; replacement[k] != 0 && j < 255; k++) {
                out[j++] = (u8)replacement[k];
            }
            i += len;
            continue;
        }

        if (gothplay3r_bk_rus_hd_single_r_token_at(s, i)) {
            out[j++] = (u8)(BK_RUS_HD_LATIN_MARKER_BASE + ('R' - 'A')); // Latin R marker
            i++;
            continue;
        }

        out[j++] = s[i++];
    }
    out[j] = 0;
    return out;
}

extern void _printbuffer_push_new(s32 x, s32 y, u8 *string);

RECOMP_FORCE_PATCH void print_dialog(s32 x, s32 y, u8* string){
    _printbuffer_push_new(x, y, gothplay3r_bk_rus_hd_latin_remap_cheato_tokens(string));
    if(print_sCurrentPtr){
        strcpy(print_sCurrentPtr->fmtString, "elq");
    }
}

RECOMP_FORCE_PATCH void print_dialog_w_bg(s32 x, s32 y, u8* string){
    _printbuffer_push_new(x, y, gothplay3r_bk_rus_hd_latin_remap_cheato_tokens(string));
    if(print_sCurrentPtr){
        strcpy(print_sCurrentPtr->fmtString, "pb");
    }
}

RECOMP_FORCE_PATCH void print_dialog_gradient(s32 x, s32 y, u8* string, u8 arg3, u8 arg4){
    _printbuffer_push_new(x, y, gothplay3r_bk_rus_hd_latin_remap_cheato_tokens(string));
    if(print_sCurrentPtr){
        print_sCurrentPtr->unk4 = arg3;
        print_sCurrentPtr->unk6 = arg4;
        strcpy(print_sCurrentPtr->fmtString, "v");
    }
}

RECOMP_FORCE_PATCH void func_802F79D0(s32 x, s32 y, u8* string, s32 arg3, s32 arg4){
    _printbuffer_push_new(x, y, gothplay3r_bk_rus_hd_latin_remap_cheato_tokens(string));
    if(print_sCurrentPtr){
        print_sCurrentPtr->unk4 = arg3;
        print_sCurrentPtr->unk6 = arg4;
        strcpy(print_sCurrentPtr->fmtString, "delq");
    }
}

// Numeric/score UI safety valve.
// Banjo uses print_bold_spaced() for item/life counters and some score/timer strings.
// The RU/PAL font layer can make the bold number lookup show the wrong glyph in Recompiled
// (for example 3 drawn as 0/circle). Do not touch game values: only route compact numeric
// UI strings through the already-working dialog font path. Full text stays on the original path.
//
// The imported RU dialog font path is readable, but '/' can land
// on the wrong glyph in this port. Compact numeric ratios are copied to a small scratch buffer
// and '/' is displayed as '-' for readability, matching the safe totals page.
static s32 gothplay3r_bk_rus_is_numeric_ui_string(u8 *s) {
    s32 i;
    s32 has_digit = 0;
    if (s == 0 || s[0] == 0) return 0;

    for (i = 0; s[i] != 0; i++) {
        u8 c = s[i];
        if (c >= '0' && c <= '9') {
            has_digit = 1;
            continue;
        }

        // Allow only compact score/timer punctuation.
        // Covers: 3, 10, 0:35, 1:02:03, 3-10, 3/10, x3, X3, +3, 99.
        if (c == ' ' || c == ':' || c == '-' || c == '/' || c == '.' ||
            c == ',' || c == '+' || c == 'x' || c == 'X') {
            continue;
        }

        return 0;
    }

    return has_digit;
}

static u8 *gothplay3r_bk_rus_numeric_ui_scratch(u8 *s) {
    static u8 scratch[8][64];
    static s32 slot = 0;
    s32 i;
    s32 needs_copy = 0;

    if (s == 0) return s;

    for (i = 0; s[i] != 0; i++) {
        if (s[i] == '/') {
            needs_copy = 1;
            break;
        }
    }

    if (!needs_copy) return s;

    slot = (slot + 1) & 7;
    for (i = 0; i < 63 && s[i] != 0; i++) {
        scratch[slot][i] = (s[i] == '/') ? '-' : s[i];
    }
    scratch[slot][i] = 0;
    return scratch[slot];
}

extern void _printbuffer_push_new(s32 x, s32 y, u8 *string);


static s32 gothplay3r_bk_rus_is_end_beach_map(void) {
    s32 map = map_get();
    return map == MAP_20_CS_END_NOT_100 ||
           map == MAP_95_CS_END_ALL_100 ||
           map == MAP_97_CS_END_BEACH_2 ||
           map == MAP_98_CS_END_SPIRAL_MOUNTAIN_1 ||
           map == MAP_99_CS_END_SPIRAL_MOUNTAIN_2;
}

RECOMP_FORCE_PATCH void print_bold_spaced(s32 x, s32 y, u8* string){
    if (gothplay3r_bk_rus_is_end_beach_map() && gothplay3r_bk_rus_is_numeric_ui_string(string)) {
        _printbuffer_push_new(x, y, gothplay3r_bk_rus_numeric_ui_scratch(string));
        if(print_sCurrentPtr){
            // "elq" is the safe dialog-font path used by print_dialog().
            strcpy(print_sCurrentPtr->fmtString, "elq");
        }
        return;
    }

    _printbuffer_push_new(x, y, string);
    if(print_sCurrentPtr){
        strcpy(print_sCurrentPtr->fmtString, "f");
    }
}

RECOMP_FORCE_PATCH void print_bold_overlapping(s32 x, s32 y, f32 arg2, u8* string){
    if (gothplay3r_bk_rus_is_end_beach_map() && gothplay3r_bk_rus_is_numeric_ui_string(string)) {
        _printbuffer_push_new(x, y, gothplay3r_bk_rus_numeric_ui_scratch(string));
        if(print_sCurrentPtr){
            strcpy(print_sCurrentPtr->fmtString, "elq");
            print_sCurrentPtr->unk10 = 1.0f;
        }
        return;
    }

    _printbuffer_push_new(x, y, string);
    if(print_sCurrentPtr){
        strcpy(print_sCurrentPtr->fmtString, "fl");
        print_sCurrentPtr->unk10 = arg2;
    }
}
