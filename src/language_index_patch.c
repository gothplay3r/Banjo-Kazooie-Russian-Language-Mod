#include <ultra64.h>
#include "modding.h"
#include "functions.h"
#include "variables.h"

// The original PAL/game-select code indexes adjacent string pointers by language id.
// In BKR + this Russian port we need the first slot only; otherwise top file-select
// text can become ERASE_CONFIRMATION or another adjacent pointer.
RECOMP_FORCE_PATCH s32 code94620_func_8031B5B0(void) {
    return 0;
}
