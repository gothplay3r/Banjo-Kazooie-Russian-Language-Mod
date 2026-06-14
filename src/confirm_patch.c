#include <ultra64.h>
#include "modding.h"
#include "functions.h"
#include "variables.h"

typedef struct GcZoombox {
    u8 unk0[0x13A];
    u8 unk13A;
} GcZoombox;

extern bool gczoombox_setStrings(GcZoombox *this, s32 str_cnt, char **str_ptrs);

static u8 s_rus_confirm_title[] = "B< YBEPEH<?";
static u8 s_rus_confirm_buttons[] = "A - `A, B - HET.";
static u8 s_rus_quiz_placeholder[] = "BOjPOC"; // ВОПРОС. Safe fallback for the one remaining hardcoded quiz placeholder.

static s32 rus_streq(char *a, char *b) {
    if (a == NULL || b == NULL) return 0;
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return (*a == 0 && *b == 0);
}

RECOMP_PATCH bool func_803183A4(GcZoombox *this, char *arg1) {
    char *sp1C;

    if ((this->unk13A & 4) || (arg1 == NULL)) {
        return FALSE;
    }

    if (rus_streq(arg1, "ARE YOU SURE?")) {
        arg1 = (char*)s_rus_confirm_title;
    } else if (rus_streq(arg1, "A - YES, B - NO")) {
        arg1 = (char*)s_rus_confirm_buttons;
    } else if (rus_streq(arg1, "THIS IS A SLIGHTLY LONGER PIECE OF TEXT FOR THE QUIZ DIALOGS!")) {
        // This is the only remaining obvious English user-facing placeholder found in the hardcode scan.
        // Do not touch Sandcastle/cheat/code strings here: those are logic input, not UI text.
        arg1 = (char*)s_rus_quiz_placeholder;
    }

    sp1C = arg1;
    return gczoombox_setStrings(this, 1, &sp1C);
}
