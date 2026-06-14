#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "modding.h"
#include "string.h"
extern s32 gameNumber;
extern u8 D_8037DCCE[];
typedef struct GcZoombox GcZoombox;
extern GcZoombox *chGameSelectBottomZoombox;
extern s32 itemscore_noteScores_getTotal(void);
extern s32 func_8031877C(GcZoombox *this);
extern s32 gczoombox_setStrings(GcZoombox *this, s32 str_cnt, char **str_ptrs);
extern void gczoombox_maximize(GcZoombox *this);
extern void gczoombox_resolve_minimize(GcZoombox *this);
extern void debugScoreStates(void);
extern void clearScoreStates(void);
extern s32 gameFile_isNotEmpty(s32 gamenum);
extern void gameFile_load(s32 gamenum);
extern u16 itemscore_timeScores_get(enum level_e level_id);
extern s32 itemscore_timeScores_getTotal(void);
extern char *gcpausemenu_TimeToA(int time);
extern s32 jiggyscore_total(void);


RECOMP_FORCE_PATCH void setGameInformationZoombox(s32 gamenum) {
    u8 *zoombox_strings[2];
    static u8 upperTextLine[0x30];
    static u8 lowerTextLine[0x30];

    debugScoreStates();
    gameNumber = gamenum;
    clearScoreStates();

    if (gameFile_isNotEmpty(gamenum)) {
        gameFile_load(gamenum);
        D_8037DCCE[gamenum] = (itemscore_timeScores_get(LEVEL_6_LAIR)) ? 1 : 0;
        strcpy(upperTextLine, "]fPA ");
        switch (gamenum) {
            case 0: strIToA(upperTextLine, 1); break;
            case 1: strIToA(upperTextLine, 3); break;
            case 2: strIToA(upperTextLine, 2); break;
        }
        strcat(upperTextLine, ": BPEM& ");
        strcat(upperTextLine, gcpausemenu_TimeToA(itemscore_timeScores_getTotal()));
        strcat(upperTextLine, ",");

        strcpy(lowerTextLine, "");
        strIToA(lowerTextLine, jiggyscore_total());
        strcat(lowerTextLine, ": jA_g<, ");
        strIToA(lowerTextLine, itemscore_noteScores_getTotal());
        strcat(lowerTextLine, ": HOT<.");
    } else {
        D_8037DCCE[gamenum] = 0;
        strcpy(upperTextLine, "]fPA ");
        switch (gamenum) {
            case 0: strIToA(upperTextLine, 1); break;
            case 1: strIToA(upperTextLine, 3); break;
            case 2: strIToA(upperTextLine, 2); break;
        }
        strcat(upperTextLine, ": jYCTO");
        strcpy(lowerTextLine, "");
    }

    zoombox_strings[0] = upperTextLine;
    zoombox_strings[1] = lowerTextLine;
    func_8031877C(chGameSelectBottomZoombox);
    gczoombox_setStrings(chGameSelectBottomZoombox, 2, zoombox_strings);
    gczoombox_maximize(chGameSelectBottomZoombox);
    gczoombox_resolve_minimize(chGameSelectBottomZoombox);
}

