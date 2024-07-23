// Sound.c : サウンドライブラリ
#include "bios.h"
#include "System.h"

// 定数の定義
// サウンド
static const i16 const soundToneFrequencyTable[] = {
    0x07f2, 0x0780, 0x0714, 0x06af, 0x0d5d, 0x0c9c, 0x0be7, 0x0b3c, 0x0a9b, 0x0a02, 0x0a02, 0x0973, 0x08eb, 0x086b, 0x0000, 0x0000,  // O1
    0x03f9, 0x03c0, 0x038a, 0x0357, 0x06af, 0x064e, 0x05f4, 0x059e, 0x054e, 0x0501, 0x0501, 0x04ba, 0x0476, 0x0436, 0x0000, 0x0000,  // O2
    0x01fd, 0x01e0, 0x01c5, 0x01ac, 0x0357, 0x0327, 0x02fa, 0x02cf, 0x02a7, 0x0281, 0x0281, 0x025d, 0x023b, 0x021b, 0x0000, 0x0000,  // O3
    0x00fe, 0x00f0, 0x00e3, 0x00d6, 0x01ac, 0x0194, 0x017d, 0x0168, 0x0153, 0x0140, 0x0140, 0x012e, 0x011d, 0x010d, 0x0000, 0x0000,  // O4
    0x007f, 0x0078, 0x0071, 0x006b, 0x00d6, 0x00ca, 0x00be, 0x00b4, 0x00aa, 0x00a0, 0x00a0, 0x0097, 0x008f, 0x0087, 0x0000, 0x0000,  // O5
    0x0040, 0x003c, 0x0039, 0x0035, 0x006b, 0x0065, 0x005f, 0x005a, 0x0055, 0x0050, 0x0050, 0x004c, 0x0047, 0x0043, 0x0000, 0x0000,  // O6
    0x0020, 0x001e, 0x001c, 0x001b, 0x0035, 0x0032, 0x0030, 0x002d, 0x002a, 0x0028, 0x0028, 0x0026, 0x0024, 0x0022, 0x0000, 0x0000,  // O7
    0x0010, 0x000d, 0x000e, 0x000d, 0x001b, 0x0019, 0x0018, 0x0016, 0x0015, 0x0014, 0x0014, 0x0013, 0x0012, 0x0011, 0x0000, 0x0000,  // O8
};
static const u8 const soundLengthTable[] = {
    1,       // T1 L0 32
    2,       // T1 L1 16
    3,       // T1 L2 16.
    4,       // T1 L3  8
    6,       // T1 L4  8.
    8,       // T1 L5  4
    12,      // T1 L6  4.
    16,      // T1 L7  2
    20,      // T1 L8  2.
    24,      // T1 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    2,       // T2 L0 32
    4,       // T2 L1 16
    6,       // T2 L2 16.
    8,       // T2 L3  8
    12,      // T2 L4  8.
    16,      // T2 L5  4
    24,      // T2 L6  4.
    32,      // T2 L7  2
    48,      // T2 L8  2.
    64,      // T2 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    3,       // T3 L0 32
    6,       // T3 L1 16
    9,       // T3 L2 16.
    12,      // T3 L3  8
    18,      // T3 L4  8.
    24,      // T3 L5  4
    36,      // T3 L6  4.
    48,      // T3 L7  2
    72,      // T3 L8  2.
    96,      // T3 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    4,       // T4 L0 32
    8,       // T4 L1 16
    12,      // T4 L2 16.
    16,      // T4 L3  8
    24,      // T4 L4  8.
    32,      // T4 L5  4
    48,      // T4 L6  4.
    64,      // T4 L7  2
    96,      // T4 L8  2.
    128,     // T4 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    5,       // T5 L0 32
    10,      // T5 L1 16
    15,      // T5 L2 16.
    20,      // T5 L3  8
    30,      // T5 L4  8.
    40,      // T5 L5  4
    60,      // T5 L6  4.
    80,      // T5 L7  2
    120,     // T5 L8  2.
    160,     // T5 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    6,       // T6 L0 32
    12,      // T6 L1 16
    18,      // T6 L2 16.
    24,      // T6 L3  8
    32,      // T6 L4  8.
    48,      // T6 L5  4
    72,      // T6 L6  4.
    96,      // T6 L7  2
    144,     // T6 L8  2.
    192,     // T6 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    7,       // T7 L0 32
    14,      // T7 L1 16
    21,      // T7 L2 16.
    28,      // T7 L3  8
    42,      // T7 L4  8.
    56,      // T7 L5  4
    84,      // T7 L6  4.
    112,     // T7 L7  2
    168,     // T7 L8  2.
    224,     // T7 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    8,       // T8 L0 32
    16,      // T8 L1 16
    24,      // T8 L2 16.
    32,      // T8 L3  8
    48,      // T8 L4  8.
    64,      // T8 L5  4
    96,      // T8 L6  4.
    128,     // T8 L7  2
    192,     // T8 L8  2.
    0,       // T8 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
};
static const i16 const soundEnvelopeTable[] = {
    0,  1000,  2000,  3000,  4000,  5000,  6000,  7000,  8000,  9000, 10000, 11000, 12000, 13000, 14000, 15000,
};
// 変数の定義
const u8* soundRequest[4];    // サウンド
u8 soundHead[8];
u16 soundPlay[4];
static u8 soundT[4];
static u8 soundS;
static u8 soundM;
static u8 soundV[4];
static u8 soundO[4];
static u8 soundL[4];
static u8 soundFrequency[8];
static u8 soundRest[4];
static u8 soundUpdate[4];
static u8 soundRegister[16];
static u8 soundBuffer[2];

static void SystemClearSoundChannel(u8 a);
void SystemInitializeSound(void) { // サウンドを初期化する
    __asm;
    // レジスタの保存
        push    de
    // 1. PSG の初期化
        call    GICINI
        ld      e, #0b10111111
        ld      a, #0x07
        call    WRTPSG
    // レジスタの復帰
        pop     de
    __endasm;
    // 2. サウンドレジスタの初期化
    soundRegister[0x00] = #0b01010101;
    soundRegister[0x01] = #0b00000000;
    soundRegister[0x02] = #0b00000000;
    soundRegister[0x03] = #0b00000000;
    soundRegister[0x04] = #0b00000000;
    soundRegister[0x05] = #0b00000000;
    soundRegister[0x06] = #0b00000000;
    soundRegister[0x07] = #0b10111111;
    soundRegister[0x08] = #0b00000000;
    soundRegister[0x09] = #0b00000000;
    soundRegister[0x0a] = #0b00000000;
    soundRegister[0x0b] = #0b00001011;
    soundRegister[0x0c] = #0b00000000;
    soundRegister[0x0d] = #0b00000000;
    soundRegister[0x0e] = #0b00000000;
    soundRegister[0x0f] = #0b00000000;
    // 3. サウンドデータの初期化
        soundRequest[0] = 0;
        soundRequest[1] = 0;
        soundRequest[2] = 0;
        soundRequest[3] = 0;
        *(i16*)&soundHead[0] = 0;
        *(i16*)&soundHead[2] = 0;
        *(i16*)&soundHead[4] = 0;
        *(i16*)&soundHead[6] = 0;
        soundPlay[0] = 0;
        soundPlay[1] = 0;
        soundPlay[2] = 0;
        soundPlay[3] = 0;
    // 4. サウンドパラメータの初期化
        for(u8 i=0;i<4;i++) SystemClearSoundChannel(i);
        soundS = 0;
        soundM = 0x09;
}
void SystemUpdateSound(void) __naked { // サウンドを更新する
    __asm;
    // レジスタの保存
        push    hl
        push    bc
        push    de
        push    ix
        push    iy
    // スリープの確認
        ld      hl, #_flag
        bit     #FLAG_SOUND_SLEEP, (hl)
        IFNZERO(90$)
            ld      e, #0b10111111
            ld      a, #0x07
            call    WRTPSG
            jp      SystemUpdateSoundEnd
        90$:
    // チャンネルの走査
        ld      bc, #0x0000
        ld      de, #0x0000
    SystemUpdateSoundChannel: // １チャンネルの処理
        // リクエスト
            ld      ix, #_soundRequest
            add     ix, de
            ld      a, 0(ix)
            or      1(ix)
            IFNZERO(00$)
                ld      l, 0(ix)
                ld      h, 1(ix)
                xor     a
                ld      0(ix), a
                ld      1(ix), a
                ld      ix, #_soundHead
                add     ix, de
                ld      0(ix), l
                ld      1(ix), h
                ld      ix, #_soundPlay
                add     ix, de
                ld      0(ix), l
                ld      1(ix), h
                ld      a, c
                call    _SystemClearSoundChannel
            00$:
        // サウンドデータの存在
            ld      ix, #_soundPlay
            add     ix, de
            ld      a, 0(ix)
            or      1(ix)
            jp      z, SystemUpdateSoundNext
        // 待機
            ld      hl, #_soundRest
            add     hl, bc
            dec     (hl)
            jp      nz, SystemUpdateSoundNext
        // 再生ポインタの取得
            ld      ix, #_soundPlay
            add     ix, de
            ld      l, 0(ix)
            ld      h, 1(ix)
    SystemUpdateSoundMml: // MML の解析
        ld      a, (hl)
        inc     hl
        SystemUpdateSoundMml00: // 0x00 : 終端コード
            IF_A_IS_ZERO(SystemUpdateSoundMmlFf)
                ld      ix, #_soundHead
                add     ix, de
                ld      0(ix), a
                ld      1(ix), a
                ld      ix, #_soundPlay
                add     ix, de
                ld      0(ix), a
                ld      1(ix), a
                ld      ix, #_soundFrequency
                add     ix, de
                ld      0(ix), a
                ld      1(ix), a
                ld      ix, #_soundUpdate
                add     ix, bc
                ld      0(ix), #0x01
                jp      SystemUpdateSoundNext
        SystemUpdateSoundMmlFf: // $ff : 繰り返し
            IF_CP(#0xff, SystemUpdateSoundMmlT)
                ld      ix, #_soundHead
                add     ix, de
                ld      iy, #_soundPlay
                add     iy, de
                LDA(0(iy), 0(ix))
                ld      l, a
                LDA(1(iy), 1(ix))
                ld      h, a
                jr      SystemUpdateSoundMml
        SystemUpdateSoundMmlT: // 'T' : テンポ（T1 ～ T8）
            IF_CP(#'T', SystemUpdateSoundMmlS) //;'
                ld      a, (hl)
                inc     hl
                sub     #'1  ;'
                ld      ix, #_soundT
                add     ix, bc
                ld      0(ix), a
                jr      SystemUpdateSoundMml
        SystemUpdateSoundMmlS: // 'S' : エンベロープ波形（S0 ～ S15）
            IF_CP(#'S', SystemUpdateSoundMmlM) //;'
                ld      a, (hl)
                inc     hl
                sub     #'0 ;'
                ld      (_soundS), a
                ld      a, (hl)
                cp      #'0 ;'
                jr      c, SystemUpdateSoundMml
                    cp      #('9+0x01) ;'
                    jr      nc, SystemUpdateSoundMml
                        sub     #'0 ;'
                        add     a, #0x0a
                        ld      (_soundS), a
                        inc     hl
                        jp      SystemUpdateSoundMml
        SystemUpdateSoundMmlM: // 'M' : エンベロープ周期（M0 ～ M15）
            IF_CP(#'M', SystemUpdateSoundMmlV) //;'
                ld      a, (hl)
                inc     hl
                sub     #'0 ;'
                ld      (_soundM), a
                ld      a, (hl)
                cp      #'0 ;'
                jp      c, SystemUpdateSoundMml
                    cp      #('9+0x01) ;'
                    jp      nc, SystemUpdateSoundMml
                        sub     #'0 ;'
                        add     a, #0x0a
                        ld      (_soundM), a
                        inc     hl
                        jp      SystemUpdateSoundMml
        SystemUpdateSoundMmlV: // 'V' : 音量（V0 ～ V16）
            IF_CP(#'V', SystemUpdateSoundMmlO) //;'
                ld      a, (hl)
                inc     hl
                ld      ix, #_soundV
                add     ix, bc
                sub     #'0 ;'
                ld      0(ix), a
                ld      a, (hl)
                cp      #'0 ;'
                jp      c, SystemUpdateSoundMml
                    cp      #('9+0x01) ;'
                    jp      nc, SystemUpdateSoundMml
                        sub     #'0 ;'
                        add     #0x0a
                        ld      0(ix), a
                        inc     hl
                        jp      SystemUpdateSoundMml
        SystemUpdateSoundMmlO: // 'O' : オクターブ（O1 ～ O8）
            IF_CP(#'O', SystemUpdateSoundMmlL) //;'
                ld      a, (hl)
                inc     hl
                sub     #'1  ;'
                ld      ix, #_soundO
                add     ix, bc
                ld      0(ix), a
                jp      SystemUpdateSoundMml
        SystemUpdateSoundMmlL: // 'L' : 音の長さ（L0 ～ L9）
            IF_CP(#'L', SystemUpdateSoundMmlR) //;'
                ld      a, (hl)
                inc     hl
                sub     #'0 ;'
                ld      ix, #_soundL
                add     ix, bc
                ld      0(ix), a
                jp      SystemUpdateSoundMml
        SystemUpdateSoundMmlR: // 'R' : 休符
            IF_CP(#'R', SystemUpdateSoundMmlA) //;'
                ld      ix, #_soundFrequency
                add     ix, de
                xor     a
                ld      0(ix), a
                ld      1(ix), a
                jr      SystemUpdateSoundRest
        SystemUpdateSoundMmlA: // 'A' : 音符
            push    de
                sub     #'A ;'
                sla     a
                sla     a
                ld      ix, #_soundToneFrequencyTable
                ld      e, a
                ld      d, #0x00
                add     ix, de
                ld      iy, #_soundO
                add     iy, bc
                ld      e, 0(iy)
                sla     e
                sla     e
                sla     e
                sla     e
                sla     e
                add     ix, de
                IF_LD_A_CP((hl), #'#', 10$) //;'
                    ld      de, #0x0002
                    add     ix, de
                    inc     hl
                10$:
            pop     de
            ld      iy, #_soundFrequency
            add     iy, de
            LDA(0(iy), 0(ix))
            LDA(1(iy), 1(ix))
            // jr      SystemUpdateSoundRest
        SystemUpdateSoundRest: // 音の長さの設定
            ld      a, (hl)
            cp      #'0    ;'
            IFNC(20$)
                cp      #('9+0x01) ;'
                IFC(20$)
                    inc     hl
                    sub     #'0 ;'
                    jr      21$
            20$: // else
                ld      ix, #_soundL
                add     ix, bc
                ld      a, 0(ix)
            21$: // endif
            push    de
                ld      ix, #_soundLengthTable
                ld      e, a
                ld      d, #0x00
                add     ix, de
                ld      iy, #_soundT
                add     iy, bc
                ld      e, 0(iy)
                sla     e
                sla     e
                sla     e
                sla     e
                add     ix, de
            pop     de
            ld      iy, #_soundRest
            add     iy, bc
            LDA(0(iy), 0(ix))
            ld      ix, #_soundUpdate
            add     ix, bc
            ld      0(ix), #0x01
            // 再生ポインタの保存
                ld      ix, #_soundPlay
                add     ix, de
                ld      0(ix), l
                ld      1(ix), h
                // jr      SystemUpdateSoundNext
            // チャンネルの走査の完了
    SystemUpdateSoundNext: // 次のチャンネルへ
        inc     bc
        inc     de
        inc     de
        ld      a, c
        cp      #0x04
        jp      nz, SystemUpdateSoundChannel
    SystemUpdateSoundChannelA:
        // チャンネルＡの設定
        IF_LD_A_IS_NZERO((_soundUpdate+0), SystemUpdateSoundChannelB)
            ld      hl, #(_soundRegister+0x07)
            ld      ix, #_soundFrequency
            // set or reset
                ld      a, 0(ix)
                IF_NOTEQ(1(ix), 30$)
                    res     #0x00, (hl)
                    jr      31$
                30$: // else
                    set     #0x00, (hl)
                31$: // endif
            // write psg
                LDA(e, (_soundV+0))
                ld      a, #0x08
                call    WRTPSG
                ld      e, 0(ix)
                ld      a, #0x00
                call    WRTPSG
                ld      e, 1(ix)
                ld      a, #0x01
                call    WRTPSG
    SystemUpdateSoundChannelB: // チャンネルＢの設定
        IF_LD_A_IS_NZERO((_soundUpdate+1), SystemUpdateSoundChannelD)
            ld      hl, #(_soundRegister+0x07)
            ld      ix, #_soundFrequency
            // set or reset
                ld      a, 2(ix)
                or      3(ix)
                IFNZERO(40$)
                    res     #0x01, (hl)
                    jr      41$
                40$:// else
                    set     #0x01, (hl)
                41$:// endif
            // write psg
                LDA(e, (_soundV+1))
                ld      a, #0x09
                call    WRTPSG
                ld      e, 2(ix)
                ld      a, #0x02
                call    WRTPSG
                ld      e, 3(ix)
                ld      a, #0x03
                call    WRTPSG
    SystemUpdateSoundChannelD: // チャンネルＤの設定
        IF_LD_A_IS_NZERO((_soundUpdate+3), SystemUpdateSoundChannelC)
            ld      hl, #(_soundRegister+0x07)
            ld      ix, #_soundFrequency
            // set or reset
                ld      a, 6(ix)
                or      7(ix)
                IFNZERO(50$)
                    res     #0x02, (hl)
                    jr      51$
                50$:// else
                    set     #0x02, (hl)
                51$:// endif
            // write psg
                LDA(e, (_soundV+3))
                ld      a, #0x0a
                call    WRTPSG
                ld      e, 6(ix)
                ld      a, #0x04
                call    WRTPSG
                ld      e, 7(ix)
                ld      a, #0x05
                call    WRTPSG
            jr      SystemUpdateSoundChannelCommon
    SystemUpdateSoundChannelC: // チャンネルＣの設定
        IF_LD_A_IS_NZERO((_soundUpdate+2), SystemUpdateSoundChannelCommon)
            ld      ix, #_soundPlay
            ld      a, 6(ix)
            or      7(ix)
            IFZERO(SystemUpdateSoundChannelCommon)
                ld      hl, #(_soundRegister+0x07)
                ld      ix, #_soundFrequency
                // set or reset
                    ld      a, 4(ix)
                    or      5(ix)
                    IFNZERO(60$)
                        res     #0x02, (hl)
                        jr      61$
                    60$:// else
                        set     #0x02, (hl)
                    61$:// endif
                // write psg
                    LDA(e, (_soundV+2))
                    ld      a, #0x0a
                    call    WRTPSG
                    ld      e, 4(ix)
                    ld      a, #0x04
                    call    WRTPSG
                    ld      e, 5(ix)
                    ld      a, #0x05
                    call    WRTPSG
    SystemUpdateSoundChannelCommon: // チャンネル共通の更新
        ld      ix, #_soundUpdate
        ld      a, 0(ix)
        or      1(ix)
        or      2(ix)
        or      3(ix)
        IFNZERO(SystemUpdateSoundEnd)
            xor     a
            ld      0(ix), a
            ld      1(ix), a
            ld      2(ix), a
            ld      3(ix), a
            LDA(e, (_soundS))
            ld      a, #0x0d
            call    WRTPSG
            ld      a, (_soundM)
            sla     a
            ld      c, a
            ld      b, #0x00
            ld      ix, #_soundEnvelopeTable
            add     ix, bc
            push    ix
                ld      e, 0(ix)
                ld      a, #0x0b
                call    WRTPSG
            pop     ix
            ld      e, 1(ix)
            ld      a, #0x0c
            call    WRTPSG
            LDA(e, (_soundRegister+0x07))
            ld      a, #0x07
            call    WRTPSG
        // 更新の終了
    SystemUpdateSoundEnd:
        // レジスタの復帰
            pop     iy
            pop     ix
            pop     de
            pop     bc
            pop     hl
        // 終了
            ret
    __endasm;
}
void SystemSuspendSound(void) __naked { // サウンドを一時停止する
    __asm;
    // レジスタの保存
        push    hl
    // スリープの設定
        ld      hl, #_flag
        set     #FLAG_SOUND_SLEEP, (hl)
    // サウンドの停止
        ld      e, #0b10111111
        ld      a, #0x07
        call    WRTPSG
    // レジスタの復帰
        pop     hl
    // 終了
        ret
    __endasm;
}
void SystemResumeSound(void) __naked { // サウンドを再開する
    __asm;
    // レジスタの保存
        push        hl
    // スリープの解除
        ld      hl, #_flag
        res     #FLAG_SOUND_SLEEP, (hl)
    // サウンドの更新
        LDA((_soundUpdate+0), #0x01)
        ld      (_soundUpdate+1), a
        ld      (_soundUpdate+2), a
        ld      (_soundUpdate+3), a
    // レジスタの復帰
        pop     hl
    // 終了
        ret
    __endasm;
}
static void SystemClearSoundChannel(u8 a) __naked { // サウンドのチャンネルをクリアする
    a;
    __asm;
    // レジスタの保存
        push    af
        push    hl
        push    bc
    // チャンネルのクリア
        ld      c, a
        ld      b, #0x00
        ld      hl, #_soundT
        add     hl, bc
        ld      (hl), #0x00

        ld      hl, #_soundV
        add     hl, bc
        ld      (hl), #0x0f

        ld      hl, #_soundO
        add     hl, bc
        ld      (hl), #0x03

        ld      hl, #_soundL
        add     hl, bc
        ld      (hl), #0x05

        ld      hl, #_soundFrequency
        add     hl, bc
        add     hl, bc
        ld      (hl), #0x00

        inc     hl
        ld      (hl), #0x00

        ld      hl, #_soundRest
        add     hl, bc
        ld      (hl), #0x01

        ld      hl, #_soundUpdate
        add     hl, bc
        ld      (hl), #0x00
    // レジスタの復帰
        pop     bc
        pop     hl
        pop     af
    // 終了
        ret
    __endasm;
}

// MML データ
const u8 const mmlNull[] = "";
const u8 const mmlTitleStartChannel0[] =
    "T2S0M12V16L1"
    "O5DEADEAB2"
    "R4";
const u8 const mmlTitleStartChannel1[] =
    "T2V16L1"
    "O4AO5DEO4AO5DEF#2"
    "R4";
const u8 const mmlTitleStartChannel2[] =
    "T2V16L1"
    "O4EAO5DO4EAO5DE2"
    "R4";
const u8 const mmlStartChannel0[] =
    "T1S0M12V16"
    "L0O6CO5BAGFEDCO4BAG"
    "L3O3GO4CO3GO4CECEGEGO5C5R5"
    "L3O3GO4CO3GO4CECEGEGO5C5R5"
    "L1O4E3DD#DD#DD#DD#DD#DD#DD#DD#"
    "L0O4CDEFGABO5C"
    "L0O4CDEFGABO5C"
    "L0O4CDEFGABO5C"
    "R5";
const u8 const mmlStartChannel1[] =
    "T1V16"
    "L0RRRRRRRRRRR"
    "L5RRRRRRR"
    "L5RRRRRRR"
    "L3RRRRRRRRR"
    "L0RRRRRRRR"
    "L0RRRRRRRR"
    "L0RRRRRRRR"
    "R5";
const u8 const mmlStartChannel2[] =
    "T1V16"
    "L0RRRRRRRRRRR"
    "L5RRRRRRR"
    "L5RRRRRRR"
    "L3RRRRRRRRR"
    "L0RRRRRRRR"
    "L0RRRRRRRR"
    "L0RRRRRRRR"
    "R5";
const u8 const mmlHiScoreChannel0[] =
    "T2S0M12V16L3"
    "O5D5RDD#FG6"
    "R9";
const u8 const mmlHiScoreChannel1[] =
    "T2V16L3"
    "O4A5RAA#O5CD6"
    "R9";
const u8 const mmlHiScoreChannel2[] =
    "T2V16L3"
    "O4F#5RF#GAB6"
    "R9";
const u8 const mmlShotChannel0[] = // MML データ
    "T1V15L0"
    "O5C#CC#RCO4BA#AG#GFD#C#O3BG#";
const u8 const mmlBombChannel0[] = // MML データ
    "T1V15L0"
    "O4CO3BAGABO4CDEFGG#R1GFEDCDFGABO5CC#R1CO4BAGAO5CDEGG#R1GFEDCDFGABO6CDO5A#";
const u8 const mmlEnemyTurnChannel1[] = // MML データ
    "T1V15L1"
    "O6CO5BA#AG#GF#FED#DDC#CO4BA#AAGF#FED#DC#CO3BA#AG#GF#FED#DC#CO2BA#A";
const u8 const mmlEnemyBombChannel2[] =
    "T1V15L0"
    "O4AGFEDCAGFEDCCDEFGABO5CDEFGABO6CO5AFEDEF";
