#include <ultra64.h>
#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "string.h"
#include "core1/sprite.h"

typedef struct font_letter { BKSpriteTextureBlock *unk0; void *unk4; } FontLetter;

extern BKSprite *D_80380AB8[];
extern FontLetter *print_sFonts[];
extern void *print_sPrintBuffer;
extern s32 D_80380AE8;
extern s32 D_80380B18;
extern s32 D_80380B1C;
extern s32 g_bk_rus_font_style_classic;

extern u32 func_802546E4(void *arg0);
extern s32 code_B3A80_func_8033BDAC(enum asset_e id, void *dst, s32 size);
extern FontLetter *func_802F4C3C(BKSprite *alphaMask, BKSprite *textureSprite);
extern void assetcache_release(void *arg0);
extern void *assetcache_get(enum asset_e id);
extern void free(void *ptr);
extern void *malloc(s32 size);
extern void *defrag(void *ptr);

// Generated in rus_assets_00.c. Used only as a clean private Classic letter mask.
// HD uses the native 0x06EC mask so RT64 can replace the score/location glyphs.
extern const u8 g_bk_rus_v12_asset_06EC_from_06EC[];
#define BK_RUS_BOLD_LETTERS_06EC_SIZE 59072

static s32 s_bk_rus_bold_letters_private = 0;

static void bk_rus_release_bold_letters_mask(void) {
    if (D_80380AB8[3] != NULL) {
        if (s_bk_rus_bold_letters_private) {
            free(D_80380AB8[3]);
        } else {
            assetcache_release(D_80380AB8[3]);
        }
        D_80380AB8[3] = NULL;
    }
    s_bk_rus_bold_letters_private = 0;
}

static void bk_rus_load_private_clean_bold_letters_mask(void) {
    D_80380AB8[3] = (BKSprite*)malloc(BK_RUS_BOLD_LETTERS_06EC_SIZE);
    if (D_80380AB8[3] != NULL) {
        memcpy(D_80380AB8[3], (void*)g_bk_rus_v12_asset_06EC_from_06EC, BK_RUS_BOLD_LETTERS_06EC_SIZE);
        s_bk_rus_bold_letters_private = 1;
    } else {
        // Extremely unlikely on PC, but keep the game alive instead of crashing.
        D_80380AB8[3] = assetcache_get(SPRITE_BOLD_FONT_LETTERS_ALPHAMASK);
        s_bk_rus_bold_letters_private = 0;
    }
}

static void bk_rus_load_native_bold_letters_mask(void) {
    D_80380AB8[3] = assetcache_get(SPRITE_BOLD_FONT_LETTERS_ALPHAMASK);
    s_bk_rus_bold_letters_private = 0;
}

static void bk_rus_load_bold_letters_for_current_mode(void) {
    if (g_bk_rus_font_style_classic) {
        bk_rus_load_private_clean_bold_letters_mask();
    } else {
        bk_rus_load_native_bold_letters_mask();
    }
}

// Classic anti-RT64-hash guard.
// RT64 replacements are global: if Classic produces the exact same final glyph texture hash
// as the HD/native donor, RT64 will replace it too. To avoid the old broad transparent-pixel
// salt that caused black artifacts, touch exactly one visible pixel per glyph by changing only
// the alpha byte by 1. Visually this is effectively invisible, but the runtime texture hash changes.
static void bk_rus_classic_anti_rt64_hash_font(FontLetter *font, BKSprite *sprite) {
    BKSpriteFrame *frame;
    s32 i;

    if (!g_bk_rus_font_style_classic || font == NULL || sprite == NULL) {
        return;
    }

    frame = sprite_getFramePtr(sprite, 0);
    if (frame == NULL || frame->chunkCnt <= 0) {
        return;
    }

    for (i = 0; i < frame->chunkCnt; i++) {
        BKSpriteTextureBlock *chunk = font[i].unk0;
        u32 *pxl;
        s32 count;
        s32 j;

        if (chunk == NULL || chunk->w <= 0 || chunk->h <= 0) {
            continue;
        }

        pxl = (u32*)(chunk + 1);
        count = chunk->w * chunk->h;

        for (j = 0; j < count; j++) {
            u32 a = pxl[j] & 0xFF;
            if (a != 0) {
                if (a > 1) {
                    pxl[j] = (pxl[j] & 0xFFFFFF00) | (a - 1);
                } else {
                    pxl[j] = (pxl[j] & 0xFFFFFF00) | (a + 1);
                }
                break;
            }
        }
    }
}

// Targeted extra guard for the digit 0 in Classic score/totals.
// v6 proved the zero-spy issue, but its XOR/alpha tweaks could be applied repeatedly
// and make the zero visually jitter on some score pages. This version is idempotent:
// every call writes the same final pixels, so the texture hash stays stable.
static void bk_rus_classic_force_zero_antihash(void) {
    BKSpriteTextureBlock *chunk;
    u32 *pxl;
    s32 count;
    s32 j;
    s32 touched = 0;

    if (!g_bk_rus_font_style_classic || print_sFonts[1] == NULL) {
        return;
    }

    chunk = print_sFonts[1][0].unk0;
    if (chunk == NULL || chunk->w <= 0 || chunk->h <= 0) {
        return;
    }

    pxl = (u32*)(chunk + 1);
    count = chunk->w * chunk->h;

    for (j = 0; j < count && touched < 8; j++) {
        u32 a = pxl[j] & 0xFF;
        if (a != 0) {
            // Stronger but still visually invisible fixed signature for Classic zero.
            // v7 fixed the blue spy, but one red zero could still collide with an RT64 donor.
            // Touch more early opaque pixels, always in the same way, so repeated calls stay stable
            // and no page-flip jitter appears.
            if (a > 250) {
                pxl[j] = (pxl[j] & 0xFFFFFF00) | 254;
            } else if (a > 1) {
                pxl[j] = (pxl[j] & 0xFFFFFF00) | (a - 1);
            } else {
                pxl[j] = (pxl[j] & 0xFFFFFF00) | (a + 1);
            }

            // Write a deterministic RGB signature. Alternate between two fixed bit clears so the
            // resulting Classic zero hash is farther away from every HD donor colour variant.
            if ((touched & 1) == 0) {
                pxl[j] = pxl[j] & 0xFFFEFFFF;
            } else {
                pxl[j] = pxl[j] & 0xFFFFFEFF;
            }
            touched++;
        }
    }
}

RECOMP_FORCE_PATCH void func_802F5060(s32 textureId){
    s32 tmp_a2;
    s32 had_bold_letters = (D_80380AB8[3] != NULL);

    // Keep the numeric rebuild close to the original/stable path.
    // v2/v3 private numeric copies could make Classic counters visibly darker after page flips.
    tmp_a2 = func_802546E4(D_80380AB8[1]);
    if(tmp_a2 & 0xF)
        tmp_a2 += 0x10 - (tmp_a2 & 0xF);
    if(!code_B3A80_func_8033BDAC(SPRITE_BOLD_FONT_NUMBERS_ALPHAMASK, D_80380AB8[1], tmp_a2)){
        assetcache_release(D_80380AB8[1]);
        D_80380AB8[1] = assetcache_get(SPRITE_BOLD_FONT_NUMBERS_ALPHAMASK);
    }

    D_80380AB8[4] = assetcache_get(textureId);
    free(print_sFonts[1]);
    print_sFonts[1] = func_802F4C3C(D_80380AB8[1], D_80380AB8[4]);
    bk_rus_classic_anti_rt64_hash_font(print_sFonts[1], D_80380AB8[1]);
    bk_rus_classic_force_zero_antihash();

    if(had_bold_letters){
        if (print_sFonts[3]) {
            free(print_sFonts[3]);
            print_sFonts[3] = NULL;
        }
        bk_rus_release_bold_letters_mask();
        bk_rus_load_bold_letters_for_current_mode();
        print_sFonts[3] = func_802F4C3C(D_80380AB8[3], D_80380AB8[4]);
        bk_rus_classic_anti_rt64_hash_font(print_sFonts[3], D_80380AB8[3]);
    }

    assetcache_release(D_80380AB8[4]);
    D_80380AB8[4] = NULL;
    D_80380B1C = textureId;
}

// Initial print-font setup creates the number font before page-flip rebuilds.
// Touch it in Classic too, otherwise the very first totals page can still show HD spy digits.
RECOMP_HOOK_RETURN("func_802F51B8")
void bk_rus_classic_anti_hash_after_print_init(void) {
    bk_rus_classic_anti_rt64_hash_font(print_sFonts[1], D_80380AB8[1]);
    bk_rus_classic_force_zero_antihash();
}

RECOMP_FORCE_PATCH void func_802F5374(void){
    if(D_80380B18 > 0 && --D_80380B18 == 0){
        bk_rus_release_bold_letters_mask();
        if(print_sFonts[3]){
            free(print_sFonts[3]);
            print_sFonts[3] = NULL;
        }
    }
}

RECOMP_FORCE_PATCH void func_802F53D0(void){
    bk_rus_release_bold_letters_mask();
    if(print_sFonts[3]){
        free(print_sFonts[3]);
        print_sFonts[3] = NULL;
    }
    D_80380B18 = 0;
}

RECOMP_FORCE_PATCH void func_802F4F64(void){
    s32 i;
    for(i = 0; i < 5; i++){
        if(i == 3){
            bk_rus_release_bold_letters_mask();
        } else {
            assetcache_release(D_80380AB8[i]);
            D_80380AB8[i] = NULL;
        }
        if(i < 4){
            free(print_sFonts[i]);
            print_sFonts[i] = NULL;
        }
    }
    free(print_sPrintBuffer);
    print_sPrintBuffer = NULL;
}

RECOMP_FORCE_PATCH BKSpriteTextureBlock *func_802F5494(s32 letterId, s32 *fontType){
    if(D_80380AE8 != 1 || (D_80380AE8 == 1 && letterId < 0xA)){
        if (g_bk_rus_font_style_classic && D_80380AE8 == 1 && letterId == 0) {
            bk_rus_classic_force_zero_antihash();
        }
        *fontType = D_80380AB8[D_80380AE8]->type;
        return print_sFonts[D_80380AE8][letterId].unk0;
    }
    else{
        if(!D_80380AB8[3]){
            bk_rus_load_bold_letters_for_current_mode();
            D_80380AB8[4] = assetcache_get(D_80380B1C);
            print_sFonts[3] = func_802F4C3C(D_80380AB8[3], D_80380AB8[4]);
            bk_rus_classic_anti_rt64_hash_font(print_sFonts[3], D_80380AB8[3]);
            assetcache_release(D_80380AB8[4]);
            D_80380AB8[4] = NULL;
        }
        D_80380B18 = 5;
        *fontType = D_80380AB8[3]->type;
        return print_sFonts[3][letterId - 10].unk0;
    }
}
