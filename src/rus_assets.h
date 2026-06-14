#ifndef BK_RUS_ASSETS_H
#define BK_RUS_ASSETS_H
#include "ultra64.h"
typedef struct BkRusAsset { s32 id; const u8 *data; u32 size; s32 needs_size; } BkRusAsset;
extern const BkRusAsset g_bk_rus_assets[];
extern const s32 g_bk_rus_asset_count;
#endif
