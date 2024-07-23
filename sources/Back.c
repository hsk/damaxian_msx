// Back.c : 背景
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "Math.h"
#include "App.h"
#include "Back.h"
// 外部変数宣言
extern const u8 const bg[];
extern const u8 const logoPatternNameTable[]; // ロゴデータ
extern const u8 const messagePatternNameTable[]; // メッセージデータ

// 定数の定義

static const u8 const colorTable[] = { // カラーテーブル
    0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1,
    0x01, 0x01, 0x51, 0x51, 0x91, 0x91, 0xb1, 0xb1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1,
};
static const u8 const colorAnimationTable[] = {
    0x01, 0x01, 0xf1, 0xf1, 0x91, 0x91, 0xb1, 0xb1,
    0x01, 0x01, 0x51, 0x51, 0xf1, 0xf1, 0xb1, 0xb1,
    0x01, 0x01, 0x51, 0x51, 0x91, 0x91, 0xf1, 0xf1,
    0x01, 0x01, 0x51, 0x51, 0x91, 0x91, 0xb1, 0xb1,
    0x01, 0x01, 0xf1, 0xf1, 0xb1, 0xb1, 0x51, 0x51,
    0x01, 0x01, 0x91, 0x91, 0xf1, 0xf1, 0x51, 0x51,
    0x01, 0x01, 0x91, 0x91, 0xb1, 0xb1, 0xf1, 0xf1,
    0x01, 0x01, 0x91, 0x91, 0xb1, 0xb1, 0x51, 0x51,
    0x01, 0x01, 0xf1, 0xf1, 0x51, 0x51, 0x91, 0x91,
    0x01, 0x01, 0xb1, 0xb1, 0xf1, 0xf1, 0x91, 0x91,
    0x01, 0x01, 0xb1, 0xb1, 0x51, 0x51, 0xf1, 0xf1,
    0x01, 0x01, 0xb1, 0xb1, 0x51, 0x51, 0x91, 0x91,
    0x01, 0x01, 0xf1, 0xf1, 0x91, 0x91, 0xb1, 0xb1,
    0x01, 0x01, 0x51, 0x51, 0xf1, 0xf1, 0xb1, 0xb1,
    0x01, 0x01, 0x51, 0x51, 0x91, 0x91, 0xf1, 0xf1,
    0x01, 0x01, 0x51, 0x51, 0x91, 0x91, 0xb1, 0xb1,
};
// 変数の定義
static u8 timerPatternNameTable[4];     // タイマ
static u8 count;                        // カウント

static void BackLoadPatternNameTable(void);
static void BackLoadColorTable(void);
static void BackMakeTimerPatternNameTable(void);

void BackLoad(void) { // 背景をロードする
    BackLoadPatternNameTable();// パターンネームテーブルのロード
    BackLoadColorTable();// カラーテーブルのロード
    BackMakeTimerPatternNameTable();// タイマの作成
    LoadVram(VIDEO_GRAPHIC1_PATTERN_NAME_TABLE+BACK_PATTERN_NAME_TABLE_TIMER, timerPatternNameTable, 4);// タイマの転送
    count = 0x01;// カウントの初期化
}
void BackUpdate(void) { // 背景を更新する
    // カラーアニメーション
    if (--count) return;// カウンタの更新
    count = 0x18;// カウンタの再設定
    // カラーの設定
    u8 e = SystemGetRandom() & 0b01111000;// ランダム値取得 -> e
    videoTransfer.vram3_src = (i16)&colorAnimationTable[e];// 転送元アドレス設定
    videoTransfer.vram3_dst = VIDEO_GRAPHIC1_COLOR_TABLE+0x10;// 転送先アドレス設定
    videoTransfer.vram3_bytes = 0x08;// 転送バイト数設定
    request |= (1 << REQUEST_VRAM);// V-Blank 中の転送の開始
}
void BackTransferStatus(void) { // ステータスを転送する
    BackMakeTimerPatternNameTable();// タイマの作成
    // タイマの転送の設定
    videoTransfer.vram2_src = (i16)timerPatternNameTable;// 転送元アドレス設定
    videoTransfer.vram2_dst = VIDEO_GRAPHIC1_PATTERN_NAME_TABLE+BACK_PATTERN_NAME_TABLE_TIMER;// 転送先アドレス設定
    videoTransfer.vram2_bytes = 0x04;// 転送バイト数設定
    request |= (1 << REQUEST_VRAM);// V-Blank 中の転送の開始
}
void BackStoreLogo(void) { // ロゴを配置する
    // パターンネームテーブルの転送の設定
    videoTransfer.vram0_src = (i16)logoPatternNameTable+0x00;// 転送元アドレス設定
    videoTransfer.vram0_dst = VIDEO_GRAPHIC1_PATTERN_NAME_TABLE+BACK_PATTERN_NAME_TABLE_LOGO+0x00;// 転送先アドレス設定
    videoTransfer.vram0_bytes = 0x10;// 転送バイト数設定
    videoTransfer.vram1_src = (i16)logoPatternNameTable+0x10;// 転送元アドレス設定
    videoTransfer.vram1_dst = VIDEO_GRAPHIC1_PATTERN_NAME_TABLE+BACK_PATTERN_NAME_TABLE_LOGO+0x20;// 転送先アドレス設定
    videoTransfer.vram1_bytes = 0x10;// 転送バイト数設定
    request |= (1 << REQUEST_VRAM);// V-Blank 中の転送の開始
}
void BackRestoreLogo(void) { // ロゴの背景を復旧する
    // パターンネームテーブルの転送の設定
    videoTransfer.vram0_src = (i16)bg+BACK_PATTERN_NAME_TABLE_LOGO+0x00;// 転送元アドレス設定
    videoTransfer.vram0_dst = VIDEO_GRAPHIC1_PATTERN_NAME_TABLE+BACK_PATTERN_NAME_TABLE_LOGO+0x00;// 転送先アドレス設定
    videoTransfer.vram0_bytes = 0x10;// 転送バイト数設定
    videoTransfer.vram1_src = (i16)bg+BACK_PATTERN_NAME_TABLE_LOGO+0x20;// 転送元アドレス設定
    videoTransfer.vram1_dst = VIDEO_GRAPHIC1_PATTERN_NAME_TABLE+BACK_PATTERN_NAME_TABLE_LOGO+0x20;// 転送先アドレス設定
    videoTransfer.vram1_bytes = 0x10;// 転送バイト数設定
    request |= (1 << REQUEST_VRAM);// V-Blank 中の転送の開始
}
void BackStoreMessage(u8 a) { // メッセージを配置する
    // パターンネームテーブルの転送の設定
    // VRAM0 転送元アドレス設定
    i16 hl = (i16)messagePatternNameTable+(a << 5);
    videoTransfer.vram0_src = hl;// VRAM1 転送元アドレス設定
    videoTransfer.vram1_src = hl+0x10;
    videoTransfer.vram0_dst = VIDEO_GRAPHIC1_PATTERN_NAME_TABLE+BACK_PATTERN_NAME_TABLE_MESSAGE+0x00;// VRAM0 転送先アドレス設定
    videoTransfer.vram0_bytes = 0x10;// VRAM0 転送バイト数設定
    videoTransfer.vram1_dst = VIDEO_GRAPHIC1_PATTERN_NAME_TABLE+BACK_PATTERN_NAME_TABLE_MESSAGE+0x20;// VRAM1 転送先アドレス設定
    videoTransfer.vram1_bytes = 0x10;// VRAM1 転送バイト数設定
    request |= (1 << REQUEST_VRAM);// V-Blank 中の転送の開始
}
void BackRestoreMessage(void) { // メッセージの背景を復旧する
    // パターンネームテーブルの転送の設定
    videoTransfer.vram0_src = (i16)&bg[BACK_PATTERN_NAME_TABLE_MESSAGE+0x00];// 転送元アドレス設定
    videoTransfer.vram0_dst = VIDEO_GRAPHIC1_PATTERN_NAME_TABLE+BACK_PATTERN_NAME_TABLE_MESSAGE+0x00;// 転送先アドレス設定
    videoTransfer.vram0_bytes = 0x10;// 転送バイト数設定
    videoTransfer.vram1_src = (i16)&bg[BACK_PATTERN_NAME_TABLE_MESSAGE+0x20];// 転送元アドレス設定
    videoTransfer.vram1_dst = VIDEO_GRAPHIC1_PATTERN_NAME_TABLE+BACK_PATTERN_NAME_TABLE_MESSAGE+0x20;// 転送先アドレス設定
    videoTransfer.vram1_bytes = 0x10;// 転送バイト数設定
    request |= (1<<REQUEST_VRAM);// V-Blank 中の転送の開始
}
static void BackLoadPatternNameTable(void) { // パターンネームテーブルを読み込む
    LoadVram(VIDEO_GRAPHIC1_PATTERN_NAME_TABLE,bg,0x300);
}
// タイマのパターンネームテーブルを作成する
static void BackMakeTimerPatternNameTable(void) {
    // 最初の3文字転送
    u8* hl = appTimer;
    u8* de = timerPatternNameTable;
    u8 b = 3, c = 0;
    do {
        u8 a = *hl++;
        if (a) c = #0x10; // 0 以外なら0x10を加えるようにする
        a += c;
        *de++ = a;
    } while(--b);
    *de = *hl+0x10;// 4文字目は0x10を加えて転送
}
static void BackLoadColorTable(void) { // カラーテーブルを読み込む
    LoadVram(VIDEO_GRAPHIC1_COLOR_TABLE,colorTable,0x20);
}
