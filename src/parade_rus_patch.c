#include <ultra64.h>
#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "string.h"

// Character parade hardcoded names.
// Character parade uses hardcoded .data string pointers and the same bold path that broke totals.
// We patch only visible parade names and route printing through the readable dialog renderer.
// Encoding uses the imported PAL/RU font map.

typedef struct BkRusParadeInfo {
    u8 map;
    s8 exit;
    s16 x;
    char *str;
    s8 unk8;
} BkRusParadeInfo;

typedef struct BkRusParadeRuntime {
    u8 state;
    u8 indx;
    u8 count;
    s8 y_position;
    s8 scroll_frame;
    u8 unk5;
    u8 parade_id;
    u8 jiggyscore;
    s32 unk8;
    BkRusParadeInfo *parade_element;
} BkRusParadeRuntime;

extern BkRusParadeInfo D_8036D9A0[0x1B];
extern BkRusParadeInfo D_8036DAE4[0x3A];
extern BkRusParadeRuntime D_803830F0;
extern void print_dialog(s32 x, s32 y, u8 *string);

extern s8 D_8036DD9C[];
extern void snackerctl_update(void);
extern void func_8028F918(s32);
extern void func_80311714(s32);
extern void gcparade_setState(s32 next_state);
extern void timedFunc_set_1(f32 time, void (*func)(s32), s32 arg);
extern int map_get(void);
extern int mapSpecificFlags_get(s32 flag);
extern void mapSpecificFlags_set(s32 flag, s32 val);
extern void gcparade_beginFinalParade(void);
extern void gcparade_textCallback(ActorMarker *caller, enum asset_e text_id, s32 arg2);


// Post Furnace Fun parade.
static u8 s_parade_ff_00[] = "fPYHT]g#`A";          // ГРУНТИЛЬДА
static u8 s_parade_ff_01[] = "TYTT]";                // ТУТТИ
static u8 s_parade_ff_02[] = "[OTTgC";              // БОТТЛС
static u8 s_parade_ff_03[] = "`^]H`^O";             // ДЖИНДЖО
static u8 s_parade_ff_04[] = "MAM[O-%M[O";        // МАМБО-ЮМБО
static u8 s_parade_ff_05[] = "`^Y-`^Y";             // ДЖУ-ДЖУ
static u8 s_parade_ff_06[] = "KOHfA";               // КОНГА
static u8 s_parade_ff_07[] = "=]Mj]";               // ШИМПИ
static u8 s_parade_ff_08[] = "[PEHT]g#`A";          // БРЕНТИЛЬДА
static u8 s_parade_ff_09[] = "KAJ]TAH [gA[[EP";     // КАПИТАН БЛАББЕР
static u8 s_parade_ff_10[] = "H]jjEP";              // НИППЕР
static u8 s_parade_ff_11[] = "Kg$HKEP";             // КЛЭНКЕР
static u8 s_parade_ff_12[] = "TAHK-TAj";             // ТАНК-ТАП
static u8 s_parade_ff_13[] = "M-P BAig";         // М-Р ВАЙЛ
static u8 s_parade_ff_14[] = "T]j-TAj";              // ТИП-ТАП
static u8 s_parade_ff_15[] = "[Off]";               // БОГГИ
static u8 s_parade_ff_16[] = "YO__A";               // УОЗЗА
static u8 s_parade_ff_17[] = "TPAHKEP";             // ТРАНКЕР
static u8 s_parade_ff_18[] = "fO[]";                // ГОБИ
static u8 s_parade_ff_19[] = "PY[] ] TYTC";         // РУБИ И ТУТС
static u8 s_parade_ff_20[] = "H$jjEP";              // НЭППЕР
static u8 s_parade_ff_21[] = "gOffO";               // ЛОГГО
static u8 s_parade_ff_22[] = "=HOPKEg#";             // ШНОРКЕЛЬ
static u8 s_parade_ff_23[] = "HABT]";               // НАВТИ
static u8 s_parade_ff_24[] = "AiP]";                // АЙРИ
static u8 s_parade_ff_25[] = "HA[-HAT";              // НАБ-НАТ
static u8 s_parade_ff_26[] = "[AH`^O ] KA_Y]";      // БАНДЖО И КАЗУИ

// Final parade.
static u8 s_parade_final_00[] = "`]HfjOT";          // ДИНГПОТ
static u8 s_parade_final_01[] = "KgAHfO";           // КЛАНГО
static u8 s_parade_final_02[] = "TOjjEP";           // ТОППЕР
static u8 s_parade_final_03[] = "[OBg";             // БОВЛ
static u8 s_parade_final_04[] = "KOgg]BY[g";       // КОЛЛИВУБЛ
static u8 s_parade_final_05[] = "KYOPP]";           // КУОРРИ
static u8 s_parade_final_06[] = "fPYHTg]Hf";        // ГРУНТЛИНГ
static u8 s_parade_final_07[] = "T]KEP";            // ТИКЕР
static u8 s_parade_final_08[] = "[Og#=EPOf";          // БОЛЬШЕРОГ
static u8 s_parade_final_09[] = "fPY[g]H";          // ГРУБЛИН
static u8 s_parade_final_10[] = "g]K]";             // ЛИКИ
static u8 s_parade_final_11[] = "_Aj]PATOP";            // ЗАПИРАТОР
static u8 s_parade_final_12[] = "_Aj]PATOP-Mg.";      // ЗАПИРАТОР-МЛ.
static u8 s_parade_final_13[] = "H&M-H&M";            // НЯМ-НЯМ
static u8 s_parade_final_14[] = "CH]jjET";          // СНИППЕТ
static u8 s_parade_final_15[] = "=PAjHEg#";          // ШРАПНЕЛЬ
static u8 s_parade_final_16[] = "CH$KEP";           // СНЭКЕР
static u8 s_parade_final_17[] = "fgYj";             // ГЛУП
static u8 s_parade_final_18[] = "CETKA+Aj";     // СЕТКАЦАП
static u8 s_parade_final_19[] = "CH]jjET-MYTAHT";     // СНИППЕТ-МУТАНТ
static u8 s_parade_final_20[] = "CEKYH";           // СЕКУН
static u8 s_parade_final_21[] = "KPOKTYC";          // КРОКТУС
static u8 s_parade_final_22[] = "\\g][[]T";        // ФЛИББИТ
static u8 s_parade_final_23[] = "^Y^E[YM";         // ЖУЖЕБУМ
static u8 s_parade_final_24[] = "XOP T]j-TAjA";      // ХОР ТИП-ТАПА
static u8 s_parade_final_25[] = "fPOff]";           // ГРОГГИ
static u8 s_parade_final_26[] = "COff]";            // СОГГИ
static u8 s_parade_final_27[] = "MOff]";            // МОГГИ
static u8 s_parade_final_28[] = "OfOH*K";         // ОГОНЁК
static u8 s_parade_final_29[] = "OfOH#KO^OP";   // ОГОНЬКОЖОР
static u8 s_parade_final_30[] = "f-H fP&_HOB]K";         // Г-Н ГРЯЗНОВИК
static u8 s_parade_final_31[] = "h]HKEP";           // ЧИНКЕР
static u8 s_parade_final_32[] = "`^]HKC]";          // ДЖИНКСИ
static u8 s_parade_final_33[] = "`PEBH]E";          // ДРЕВНИЕ
static u8 s_parade_final_34[] = "fPA[[A";           // ГРАББА
static u8 s_parade_final_35[] = "jAP=EB]K";           // ПАРШЕВИК
static u8 s_parade_final_36[] = "XgOjA[[A";           // ХЛОПАББА
static u8 s_parade_final_37[] = "MYM-MYM";          // МУМ-МУМ
static u8 s_parade_final_38[] = "X]_-TAj";           // ХИЗ-ТАП
static u8 s_parade_final_39[] = "MOT_AH`";          // МОТЗАНД
static u8 s_parade_final_40[] = "TAM[g$P";          // ТАМБЛЭР
static u8 s_parade_final_41[] = "jOPTPE+Aj";    // ПОРТРЕЦАП
static u8 s_parade_final_42[] = "X]X]KA";            // ХИХИКА
static u8 s_parade_final_43[] = "g]M[";            // ЛИМБ
static u8 s_parade_final_44[] = "P]jjEP";           // РИППЕР
static u8 s_parade_final_45[] = ">]jh]K";           // ЩИПЧИК
static u8 s_parade_final_46[] = "KOPO[OM[A";         // КОРОБОМБА
static u8 s_parade_final_47[] = "[OCC KOPO[OM[";    // БОСС КОРОБОМБ
static u8 s_parade_final_48[] = "fP]MgET";          // ГРИМЛЕТ
static u8 s_parade_final_49[] = "[PO=EHE+";        // БРОШЕНЕЦ
static u8 s_parade_final_50[] = "MOP&hOK fPY[g]H";    // МОРЯЧОК ГРУБЛИН
static u8 s_parade_final_51[] = "[A=KAh";             // БАШКАЧ
static u8 s_parade_final_52[] = "ME`BEgOBKA";          // МЕДВЕЛОВКА
static u8 s_parade_final_53[] = "jYCTO_BOH";       // ПУСТОЗВОН
static u8 s_parade_final_54[] = "_Y[[A";           // ЗУББА
static u8 s_parade_final_55[] = "fPY[g]H fY`";      // ГРУБЛИН ГУД
static u8 s_parade_final_56[] = "XgECTYH";          // ХЛЕСТУН
static u8 s_parade_final_57[] = "h]TO";             // ЧИТО


// Post-Furnace-Fun lower credits replacement printed directly by the parade renderer.
// This suppresses vanilla 0x11AF..0x11C8 text launch in gcparade_update, so no lower kra-ko-zyabry.
// Keep compact: two lines max, no AEP entries, no dialogBin hook.
static u8 s_postff_cred_00_00[] = "fgABH<i ]`Ei"; // ГЛАВНЫЙ ИДЕЙ
static u8 s_postff_cred_00_01[] = "fPEff fPYHT] MEig_"; // ГРЕГГ ГРУНТИ МЕЙЛЗ
static u8 s_postff_cred_01_00[] = "E>E ]`E]"; // ЕЩЕ ИДЕИ
static u8 s_postff_cred_01_01[] = "`^OP`^ [gA[[EP"; // ДЖОРДЖ БЛАББЕР
static u8 s_postff_cred_02_00[] = "fgABH<i KgAB]=H]K"; // ГЛАВНЫЙ КЛАВИШНИК
static u8 s_postff_cred_02_01[] = "KP]C CH$KEP CA_EPgEH`"; // КРИС СНЭКЕР САЗЕРЛЕНД
static u8 s_postff_cred_03_00[] = "_AM KgAB]=H]KA"; // ЗАМ КЛАВИШНИКА
static u8 s_postff_cred_03_01[] = "MOPTEH [A__[OM["; // МОРТЕН БАЗЗБОМБ
static u8 s_postff_cred_04_00[] = "_AM KgAB]=H]KA"; // ЗАМ КЛАВИШНИКА
static u8 s_postff_cred_04_01[] = "jOg []f KgAKEP"; // ПОЛ БИГ КЛАКЕР
static u8 s_postff_cred_05_00[] = "_AM KgAB]=H]KA"; // ЗАМ КЛАВИШНИКА
static u8 s_postff_cred_05_01[] = "fP$M CH]jjET CM]T"; // ГРЭМ СНИППЕТ СМИТ
static u8 s_postff_cred_06_00[] = "KgAB]=H]K"; // КЛАВИШНИК
static u8 s_postff_cred_06_01[] = "K]PAH KOHfA KOHHEgg"; // КИРАН КОНГА КОННЕЛЛ
static u8 s_postff_cred_07_00[] = "KgAB]=H]K"; // КЛАВИШНИК
static u8 s_postff_cred_07_01[] = "P]C gOKAj g#%]C"; // РИС ЛОКАП ЛЬЮИС
static u8 s_postff_cred_08_00[] = "E>E CTYK jO KgABAM"; // ЕЩЕ СТУК ПО КЛАВАМ
static u8 s_postff_cred_08_01[] = "MAPK MOT_AH` Y]gCOH"; // МАРК МОТЗАНД УИЛСОН
static u8 s_postff_cred_09_00[] = "fgAB XY`O^H]K"; // ГЛАВ ХУДОЖНИК
static u8 s_postff_cred_09_01[] = "CT]B `]HfjOT MEig_"; // СТИВ ДИНГПОТ МЕЙЛЗ
static u8 s_postff_cred_10_00[] = "MYg#T&=K] ] P]CYHK]"; // МУЛЬТЯШКИ И РИСУНКИ
static u8 s_postff_cred_10_01[] = "$` `^]H`^O [PAiH"; // ЭД ДЖИНДЖО БРАЙН
static u8 s_postff_cred_11_00[] = "C+EHH<i MEgOK"; // СЦЕННЫЙ МЕЛОК
static u8 s_postff_cred_11_01[] = "CT]BEH C$H`][ATT"; // СТИВЕН СЭНДИБАТТ
static u8 s_postff_cred_12_00[] = "E>E HA[POCK]"; // ЕЩЕ НАБРОСКИ
static u8 s_postff_cred_12_01[] = "`^OH H$jjEP H$="; // ДЖОН НЭППЕР НЭШ
static u8 s_postff_cred_13_00[] = "E>E KAPAKYg]"; // ЕЩЕ КАРАКУЛИ
static u8 s_postff_cred_13_01[] = "KP]C hOMjA jEig"; // КРИС ЧОМПА ПЕЙЛ
static u8 s_postff_cred_14_00[] = "fgABH<i =YMO`Eg"; // ГЛАВНЫЙ ШУМОДЕЛ
static u8 s_postff_cred_14_01[] = "fPAHT Kg$HKEP"; // ГРАНТ КЛЭНКЕР
static u8 s_postff_cred_15_00[] = "fgABH<i TECTEP"; // ГЛАВНЫЙ ТЕСТЕР
static u8 s_postff_cred_15_01[] = "X#% YO__A YOP`"; // ХЬЮ УОЗЗА УОРД
static u8 s_postff_cred_16_00[] = "TECTEP< P$iP"; // ТЕСТЕРЫ РЭЙР
static u8 s_postff_cred_16_01[] = "KOMAH`A P$iP"; // КОМАНДА РЭЙР
static u8 s_postff_cred_17_00[] = "TECTEP< HOA"; // ТЕСТЕРЫ НОА
static u8 s_postff_cred_17_01[] = "KOMAH`A HOA"; // КОМАНДА НОА
static u8 s_postff_cred_18_00[] = "[YMA^H<E `EgA"; // БУМАЖНЫЕ ДЕЛА
static u8 s_postff_cred_18_01[] = "gOffO ] TYTT]"; // ЛОГГО И ТУТТИ
static u8 s_postff_cred_19_00[] = "KH]fA hAP"; // КНИГА ЧАР
static u8 s_postff_cred_19_01[] = "g] g]K] gAB`Ei"; // ЛИ ЛИКИ ЛАВДЕЙ
static u8 s_postff_cred_20_00[] = "CTPAHH<E _BYK]"; // СТРАННЫЕ ЗВУКИ
static u8 s_postff_cred_20_01[] = "KOMAH`A _BYKA"; // КОМАНДА ЗВУКА
static u8 s_postff_cred_21_00[] = "CjAC][O HOA"; // СПАСИБО НОА
static u8 s_postff_cred_21_01[] = "KEH \\g][[]T gO[["; // КЕН ФЛИББИТ ЛОББ
static u8 s_postff_cred_22_00[] = "CjAC][O [OCCAM"; // СПАСИБО БОССАМ
static u8 s_postff_cred_22_01[] = "H]HTEH`O ] P$iP"; // НИНТЕНДО И РЭЙР
static u8 s_postff_cred_23_00[] = "jO`ChET [O[OB"; // ПОДСЧЕТ БОБОВ
static u8 s_postff_cred_23_01[] = "`^O$g fPA[[A"; // ДЖОЭЛ ГРАББА
static u8 s_postff_cred_24_00[] = "`B]^YXA ] CYETA"; // ДВИЖУХА И СУЕТА
static u8 s_postff_cred_24_01[] = "T]M TAHK TAj"; // ТИМ ТАНК ТАП
static u8 s_postff_cred_25_00[] = "jPABA 1998"; // ПРАВА 1998
static u8 s_postff_cred_25_01[] = "H]HTEH`O P$iP gT`"; // НИНТЕНДО РЭЙР ЛТД

static u8 *s_postff_credit_lines[0x1A][2] = {
    { s_postff_cred_00_00, s_postff_cred_00_01 },
    { s_postff_cred_01_00, s_postff_cred_01_01 },
    { s_postff_cred_02_00, s_postff_cred_02_01 },
    { s_postff_cred_03_00, s_postff_cred_03_01 },
    { s_postff_cred_04_00, s_postff_cred_04_01 },
    { s_postff_cred_05_00, s_postff_cred_05_01 },
    { s_postff_cred_06_00, s_postff_cred_06_01 },
    { s_postff_cred_07_00, s_postff_cred_07_01 },
    { s_postff_cred_08_00, s_postff_cred_08_01 },
    { s_postff_cred_09_00, s_postff_cred_09_01 },
    { s_postff_cred_10_00, s_postff_cred_10_01 },
    { s_postff_cred_11_00, s_postff_cred_11_01 },
    { s_postff_cred_12_00, s_postff_cred_12_01 },
    { s_postff_cred_13_00, s_postff_cred_13_01 },
    { s_postff_cred_14_00, s_postff_cred_14_01 },
    { s_postff_cred_15_00, s_postff_cred_15_01 },
    { s_postff_cred_16_00, s_postff_cred_16_01 },
    { s_postff_cred_17_00, s_postff_cred_17_01 },
    { s_postff_cred_18_00, s_postff_cred_18_01 },
    { s_postff_cred_19_00, s_postff_cred_19_01 },
    { s_postff_cred_20_00, s_postff_cred_20_01 },
    { s_postff_cred_21_00, s_postff_cred_21_01 },
    { s_postff_cred_22_00, s_postff_cred_22_01 },
    { s_postff_cred_23_00, s_postff_cred_23_01 },
    { s_postff_cred_24_00, s_postff_cred_24_01 },
    { s_postff_cred_25_00, s_postff_cred_25_01 }
};

static void gothplay3r_bk_rus_print_postff_credit_lines(void) {
    if (D_803830F0.parade_id != 0 || D_803830F0.indx >= 0x1A) {
        return;
    }
    print_dialog(64, 170, s_postff_credit_lines[D_803830F0.indx][0]);
    print_dialog(64, 188, s_postff_credit_lines[D_803830F0.indx][1]);
}

void gothplay3r_bk_rus_patch_parade_strings(void) {
    static u8 *ff_names[0x1B] = {
        s_parade_ff_00, s_parade_ff_01, s_parade_ff_02, s_parade_ff_03, s_parade_ff_04,
        s_parade_ff_05, s_parade_ff_06, s_parade_ff_07, s_parade_ff_08, s_parade_ff_09,
        s_parade_ff_10, s_parade_ff_11, s_parade_ff_12, s_parade_ff_13, s_parade_ff_14,
        s_parade_ff_15, s_parade_ff_16, s_parade_ff_17, s_parade_ff_18, s_parade_ff_19,
        s_parade_ff_20, s_parade_ff_21, s_parade_ff_22, s_parade_ff_23, s_parade_ff_24,
        s_parade_ff_25, s_parade_ff_26
    };
    static u8 *final_names[0x3A] = {
        s_parade_final_00, s_parade_final_01, s_parade_final_02, s_parade_final_03, s_parade_final_04,
        s_parade_final_05, s_parade_final_06, s_parade_final_07, s_parade_final_08, s_parade_final_09,
        s_parade_final_10, s_parade_final_11, s_parade_final_12, s_parade_final_13, s_parade_final_14,
        s_parade_final_15, s_parade_final_16, s_parade_final_17, s_parade_final_18, s_parade_final_19,
        s_parade_final_20, s_parade_final_21, s_parade_final_22, s_parade_final_23, s_parade_final_24,
        s_parade_final_25, s_parade_final_26, s_parade_final_27, s_parade_final_28, s_parade_final_29,
        s_parade_final_30, s_parade_final_31, s_parade_final_32, s_parade_final_33, s_parade_final_34,
        s_parade_final_35, s_parade_final_36, s_parade_final_37, s_parade_final_38, s_parade_final_39,
        s_parade_final_40, s_parade_final_41, s_parade_final_42, s_parade_final_43, s_parade_final_44,
        s_parade_final_45, s_parade_final_46, s_parade_final_47, s_parade_final_48, s_parade_final_49,
        s_parade_final_50, s_parade_final_51, s_parade_final_52, s_parade_final_53, s_parade_final_54,
        s_parade_final_55, s_parade_final_56, s_parade_final_57
    };
    s32 i;

    for (i = 0; i < 0x1B; i++) {
        D_8036D9A0[i].str = (char*)ff_names[i];
    }
    for (i = 0; i < 0x3A; i++) {
        D_8036DAE4[i].str = (char*)final_names[i];
    }
}

RECOMP_FORCE_PATCH void gcparade_print(s32 index) {
    BkRusParadeInfo *v0 = D_803830F0.parade_element;
    (void)index;
    if (v0 != NULL && v0->str != NULL) {
        print_dialog(v0->x, D_803830F0.y_position, (u8*)v0->str);
    }
    gothplay3r_bk_rus_print_postff_credit_lines();
}

RECOMP_FORCE_PATCH void gcparade_update(void) {
    s32 sp34;

    if ((map_get() == MAP_96_CS_END_BEACH_1) && mapSpecificFlags_get(4)) {
        mapSpecificFlags_set(4, FALSE);
        gcparade_beginFinalParade();
        return;
    }
    if (volatileFlag_get(VOLATILE_FLAG_1F_IN_CHARACTER_PARADE) != 0) {
        snackerctl_update();
        switch (D_803830F0.state) {
            case 3: // PARADE_STATE_3_WARP
                if (D_803830F0.unk5 == 0) {
                    if (map_get() == D_803830F0.parade_element->map) {
                        if (D_803830F0.parade_id == 0) {
                            // For credits entries 0x11AF..0x11C8, do NOT launch the vanilla text asset.
                            // It prints lower mojibake through the RU glyph route. We drive the timing manually.
                            if (D_803830F0.indx < 0x1A) {
                                timedFunc_set_1(1.0f, (void (*)(s32))func_80311714, 0);
                                timedFunc_set_1(1.0f, (void (*)(s32))func_80311714, 1);
                                timedFunc_set_1(5.2f, (void (*)(s32))gcparade_setState, 6);
                            } else {
                                // Keep the last Banjo/Kazooie text route. It was already handled in the base.
                                sp34 = 0xA0;
                                if (D_803830F0.parade_element->exit >= 0) {
                                    sp34 = 0xA8;
                                }
                                timedFunc_set_1(1.0f, (void (*)(s32))func_80311714, 0);
                                func_80324DBC(1.0f, D_803830F0.indx + 0x11AF, sp34, NULL, NULL, gcparade_textCallback, NULL);
                                timedFunc_set_1(1.0f, (void (*)(s32))func_80311714, 1);
                            }
                        } else if (D_803830F0.parade_element->exit >= 0) {
                            func_8028F918(2);
                        }
                        timedFunc_set_1(1.0f, (void (*)(s32))gcparade_setState, 4);
                        D_803830F0.unk5 = 1;
                    }
                }
                break;
            case 4: // PARADE_STATE_4_NAME_APPEAR
                gcparade_print(D_803830F0.indx);
                D_803830F0.scroll_frame++;
                D_803830F0.y_position += D_8036DD9C[D_803830F0.scroll_frame];
                if (D_803830F0.scroll_frame == 0xC) {
                    gcparade_setState(5);
                }
                break;
            case 5: // PARADE_STATE_5_WAIT_ON_NAME
                gcparade_print(D_803830F0.indx);
                break;
            case 6: // PARADE_STATE_6_NAME_DISAPPEAR
                gcparade_print(D_803830F0.indx);
                D_803830F0.scroll_frame--;
                D_803830F0.y_position -= D_8036DD9C[D_803830F0.scroll_frame];
                if (D_803830F0.scroll_frame == 0) {
                    gcparade_setState(7);
                }
                break;
        }
    }
}

