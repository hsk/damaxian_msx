// System.c : システムライブラリ
#include "bios.h"
#include "vdp.h"
#include "System.h"
// 定数の定義
static const u8 const inputStickTable[] = { // キー入力
    0x00,
    (1 << INPUT_KEY_UP),
    (1 << INPUT_KEY_UP)    | (1 << INPUT_KEY_RIGHT),
    (1 << INPUT_KEY_RIGHT),
    (1 << INPUT_KEY_DOWN)  | (1 << INPUT_KEY_RIGHT),
    (1 << INPUT_KEY_DOWN),
    (1 << INPUT_KEY_DOWN)  | (1 << INPUT_KEY_LEFT),
    (1 << INPUT_KEY_LEFT),
    (1 << INPUT_KEY_UP)    | (1 << INPUT_KEY_LEFT),
};
static const u8 const videoScreenMode[] = { // ビデオ
    0b00000000,          // text 1
    0b10110000,
    (0x0000 >> 10),
    (0x0000 >> 6),
    (0x0800 >> 11),
    (0x1b00 >> 7),
    (0x3800 >> 11),
    0b11110100,
    0b00000000,          // graphic 1
    0b10100000,
    (0x1800 >> 10),
    (0x2000 >> 6),
    (0x0000 >> 11),
    (0x1b00 >> 7),
    (0x3800 >> 11),
    0b00000111,
    0b00000010,          // graphic 2
    0b10100000,
    (0x1800 >> 10),
    (0x2000 >> 6),
    (0x0000 >> 11),
    (0x1b00 >> 7),
    (0x3800 >> 11),
    0b00000111,
    0b00000000,          // multi color
    0b10101000,
    (0x0800 >> 10),
    (0x2000 >> 6),
    (0x0000 >> 11),
    (0x1b00 >> 7),
    (0x3800 >> 11),
    0b00000111,
};
// 変数の定義
u8 flag;               // フラグ
u8 request;            // リクエスト
u8 input[INPUT_SIZE];  // キー入力
u8 inputBuffer;
u8 videoPort[2];       // ビデオ
u8 videoRegister[8];
VIDEO_TRANSFER videoTransfer;
u8 sprite[128];        // スプライト
void SystemInitialize(void) { // システムを初期化する
    flag = 0; // フラグの初期化
    SystemInitializeInput();// キー入力の初期化
    SystemInitializeVideo();// ビデオの初期化
}
void SystemInitializeInput(void) { // キー入力を初期化する
    for (u8 i=0;i<INPUT_SIZE;i++) input[i]=0;
}
void SystemUpdateInput(void) __naked { // キーの入力を更新する
    __asm;
    // レジスタの保存
        push    hl
        push    bc
        push    de
        push    ix
        push    iy
    // キーの取得
        // stick 1 と stick 2 の or を取ったものをbに入れる
            // stick 1 入力
                ld      a, #0x00
                call    GTSTCK
                ld      c, a
                ld      b, #0x00
                ld      hl, #_inputStickTable
                add     hl, bc
                LDA((_inputBuffer+0), (hl))
            // stick 2 入力
                ld      a, #0x01
                call    GTSTCK
                ld      c, a
                ld      b, #0x00
                ld      hl, #_inputStickTable
                add     hl, bc
                ld      a, (_inputBuffer+0)
                or      (hl)
                ld      b, a
        ld      hl, #(_input+INPUT_KEY_UP)
        ld      a, (hl)
        inc     a
        IFZERO(00$)
            ld      a, #0xff
        00$:
        bit     #INPUT_KEY_UP, b
        IFZERO(01$)
            xor     a
        01$:
        ld      (hl), a
        ld      hl, #(_input+INPUT_KEY_DOWN)
        ld      a, (hl)
        inc     a
        IFZERO(02$)
            ld      a, #0xff
        02$:
        bit     #INPUT_KEY_DOWN, b
        IFZERO(03$)
            xor     a
        03$:
        ld      (hl), a
        ld      hl, #(_input+INPUT_KEY_LEFT)
        ld      a, (hl)
        inc     a
        IFZERO(04$)
            ld      a, #0xff
        04$:
        bit     #INPUT_KEY_LEFT, b
        IFZERO(05$)
            xor     a
        05$:
        ld      (hl), a
        ld      hl, #(_input+INPUT_KEY_RIGHT)
        ld      a, (hl)
        inc     a
        IFZERO(06$)
            ld      a, #0xff
        06$:
        bit     #INPUT_KEY_RIGHT, b
        IFZERO(07$)
            xor     a
        07$:
        ld      (hl), a
    // ボタンの取得
        ld      a, #0x00
        call    GTTRIG
        ld      (_inputBuffer+0), a
        ld      a, #0x01
        call    GTTRIG
        ld      hl, #(_inputBuffer+0)
        or      (hl)
        ld      hl, #(_input+INPUT_BUTTON_SPACE)
        IFNZERO(10$)
            ld      a, (hl)
            inc     a
            IFZERO(10$)
                ld      a, #0xff
        10$:
        ld      (hl), a
        ld      a, #0x07
        call    SNSMAT
        ld      b, a
        ld      hl, #(_input+INPUT_BUTTON_RETURN)
        ld      a, (hl)
        inc     a
        IFZERO(11$)
            ld      a, #0xff
        11$:
        bit     #0x07, b
        IFNZERO(12$)
            xor     a
        12$:
        ld      (hl), a
        ld      hl, #(_input+INPUT_BUTTON_ESC)
        ld      a, (hl)
        inc     a
        IFZERO(13$)
            ld      a, #0xff
        13$:
        bit     #0x02, b
        IFNZERO(14$)
            xor     a
        14$:
        ld      (hl), a
        ld      hl, #(_input+INPUT_BUTTON_STOP)
        ld      a, (hl)
        inc     a
        IFZERO(15$)
            ld      a, #0xff
        15$:
        bit     #0x04, b
        IFNZERO(16$)
            xor     a
        16$:
        ld      (hl), a
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
void SystemInitializeVideo(void) { // ビデオを初期化する
    // ポートの取得
    videoPort[0]= *(u8*)0x0006;
    videoPort[1]= *(u8*)0x0007;
    // レジスタの取得
    for (u8 i=0;i<8;i++)videoRegister[i]=((u8*)RG0SAV)[i];
    // VRAM の転送の初期化
    for (u8 i=0,*v=(u8*)videoTransfer;i<sizeof(VIDEO_TRANSFER);i++)
        *v++=0;
}
void SystemSetScreenMode(u8 mode) { // スクリーンモードを設定する
    // レジスタの設定
    inline const u8* a = videoScreenMode;
    u8* hl = (u8*)&a[mode << 3];
    u8* de = videoRegister;
    u8 b = 8;
    for(;b;b--) *de++ = *hl++;
    SystemTransferVideoRegister();// レジスタの転送
}
void SystemTransferVideoRegister(void) __naked { // レジスタを転送する
    __asm;
    // レジスタの保存
        push    hl
        push    bc
    // 割り込みの禁止
        di
    // レジスタの設定
        ld      hl, #_videoRegister
        ld      a, (_videoPort+1)
        inc     a
        ld      c, a
        outi
        ld      a, #0x80
        out     (c), a
        outi
        ld      a, #0x81
        out     (c), a
        outi
        ld      a, #0x82
        out     (c), a
        outi
        ld      a, #0x83
        out     (c), a
        outi
        ld      a, #0x84
        out     (c), a
        outi
        ld      a, #0x85
        out     (c), a
        outi
        ld      a, #0x86
        out     (c), a
        outi
        ld      a, #0x87
        out     (c), a
    // 割り込み禁止の解除
        ei
    // レジスタの復帰
        pop     bc
        pop     hl
    // 終了
        ret
    __endasm;
}
#define $ __endasm; __asm
void SystemTransferVram(void) __naked { // VRAM にデータを転送する
    __asm;
    // レジスタの保存
        push    hl
        push    bc
        push    de
    // 割り込みの禁止
        di
    // ポートの取得
        LDA(d, (_videoPort+1))
        inc     a
        ld      e, a
    // VRAM 0 の転送
        ld      hl, (_videoTransfer+VIDEO_TRANSFER_VRAM_0_SRC)
        ld      a, l
        or      h
        IFNZERO(01$)
            ld      c, e
            ld      a, (_videoTransfer+VIDEO_TRANSFER_VRAM_0_DST+0)
            out     (c), a
            ld      a, (_videoTransfer+VIDEO_TRANSFER_VRAM_0_DST+1)
            or      #0b01000000
            out     (c), a
            ld      c, d
            LDA(b, (_videoTransfer+VIDEO_TRANSFER_VRAM_0_BYTES))
            otir
        01$:
    // VRAM 1 の転送
        ld      hl, (_videoTransfer+VIDEO_TRANSFER_VRAM_1_SRC)
        ld      a, l
        or      h
        IFNZERO(02$)
            ld      c, e
            ld      a, (_videoTransfer+VIDEO_TRANSFER_VRAM_1_DST+0)
            out     (c), a
            ld      a, (_videoTransfer+VIDEO_TRANSFER_VRAM_1_DST+1)
            or      #0b01000000
            out     (c), a
            ld      c, d
            LDA(b, (_videoTransfer+VIDEO_TRANSFER_VRAM_1_BYTES))
            otir
        02$:
    // VRAM 2 の転送
        ld      hl, (_videoTransfer+VIDEO_TRANSFER_VRAM_2_SRC)
        ld      a, l
        or      h
        IFNZERO(03$)
            ld      c, e
            ld      a, (_videoTransfer+VIDEO_TRANSFER_VRAM_2_DST+0)
            out     (c), a
            ld      a, (_videoTransfer+VIDEO_TRANSFER_VRAM_2_DST+1)
            or      #0b01000000
            out     (c), a
            ld      c, d
            LDA(b, (_videoTransfer+VIDEO_TRANSFER_VRAM_2_BYTES))
            otir
        03$:
    // VRAM 3 の転送
        ld      hl, (_videoTransfer+VIDEO_TRANSFER_VRAM_3_SRC)
        ld      a, l
        or      h
        IFNZERO(90$)
            ld      c, e
            ld      a, (_videoTransfer+VIDEO_TRANSFER_VRAM_3_DST+0)
            out     (c), a
            ld      a, (_videoTransfer+VIDEO_TRANSFER_VRAM_3_DST+1)
            or      #0b01000000
            out     (c), a
            ld      c, d
            LDA(b, (_videoTransfer+VIDEO_TRANSFER_VRAM_3_BYTES))
            otir
        90$:
    // 割り込み禁止の解除
        ei
    // 転送の完了
        ld      hl, #_videoTransfer
        xor     a
        ld (hl), a $ inc hl $ ld (hl), a $ inc hl $ inc hl $ inc hl $ inc hl $
        ld (hl), a $ inc hl $ ld (hl), a $ inc hl $ inc hl $ inc hl $ inc hl $
        ld (hl), a $ inc hl $ ld (hl), a $ inc hl $ inc hl $ inc hl $ inc hl $
        ld (hl), a $ inc hl $ ld (hl), a
    // レジスタの復帰
        pop     de
        pop     bc
        pop     hl
    // 終了
        ret
    __endasm;
}
void SystemClearSprite(void) { // スプライトをクリアする
    // スプライトのクリア
    for (u8*hl=sprite,b=0x80;b;b--,hl++) *hl = 0xc0;
}
void SystemTransferSprite(void) __naked { // スプライトを転送する
    __asm;
    // レジスタの保存
        push    hl
        push    bc
    // スプライトアトリビュートテーブルの取得
        LDA(l, (_videoRegister+VDP_R5))
        ld      h, #0x00
        sla     l
        rl      h
        sla     l
        rl      h
        sla     l
        rl      h
        sla     l
        rl      h
        sla     l
        rl      h
        sla     l
        rl      h
        sla     l
        rl      h
    // 割り込みの禁止
        di
    // VRAM アドレスの設定
        ld      a, (_videoPort+1)
        inc     a
        ld      c, a
        //ld      a, #<VIDEO_GRAPHIC1_SPRITE_ATTRIBUTE_TABLE
        //out     (c), a
        //ld      a, #(>VIDEO_GRAPHIC1_SPRITE_ATTRIBUTE_TABLE | 0b01000000)
        out     (c), l
        ld      a, h
        or      #0b01000000
        out     (c), a
        dec     c
    // スプライトアトリビュートテーブルの転送
        ld      hl, #_sprite
        ld      b, #0x80
        otir
    // 割り込み禁止の解除
        ei
    // レジスタの復帰
        pop     bc
        pop     hl
    // 終了
        ret
    __endasm;
}
// de スプライトデータアドレス
// hl スプライトテーブルアドレス
// b x座用
// c y座標
void SystemSetSprite(u8* hl,u8*de,u8 y,u8 x) { // スプライトを設定する
    // スプライトの設定
    *de++ = *hl++ + y;
    x = *hl++ + x;
    u8 b = 0x00;
    if (x >= 0xf0) {
        x += 0x20;
        b = 0x80;
    }
    *de++ = x;
    *de++ = *hl++;
    *de++ = *hl++ | b;
}

static void SetVramAddress(u16 vram) __naked {
    vram;
    __asm;
        // ポート取得
            ld      a, (_videoPort+1)
            inc     a
            ld      c, a
        // 出力アドレス設定
            ld      a, l
            out     (c), a
            ld      a, h
            or      a, #0b01000000
            out     (c), a
        ret
    __endasm;
}
static void LoadVram1(u8*addr,u16 size) __naked {
    addr;// hl
    size;// de
    __asm;
        // 転送設定
            ld      a, (_videoPort+1)
            ld      c, a
        // 転送 256*8 = 2048 = 2kbytes
            xor     a
            cp      d
            jp      z,LoadVram1_loop1_end
            ld      b, #0x00
    LoadVram1_loop1:
            otir
            dec d
            jp nz, LoadVram1_loop1
    LoadVram1_loop1_end:
            xor     a
            cp      e
            ret     z
            ld      b,e
            otir
            ret
    __endasm;
}
void LoadVram(u16 vram,u8*addr,u16 size) {
    SetVramAddress(vram);
    LoadVram1(addr, size);
}
static void SetVram1(u8 data,u16 size) __naked {
    data;// a
    size;// hl
    __asm;
        // 転送設定
            ex      de, hl
            ld      h, a
            ld      a, (_videoPort+1)
            ld      c, a
        // 転送 256*8 = 2048 = 2kbytes
            xor     a
            cp      d
            jp      z,2$
            ld      a, h
            ld      b, #0x00
    1$:
            out     (c),a
            DJNZ 1$
            dec d
            jp nz, 1$
    2$:
            xor     a
            cp      e
            ret     z
            ld      b,e
            ld      a, h
    99$:
            out     (c),a
            DJNZ 99$

            ret
    __endasm;
}
void SetVram(u16 vram,u8 data,u16 size) {
    SetVramAddress(vram);
    SetVram1(data, size);
}
