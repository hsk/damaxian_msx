// Title.c : タイトル
#include "bios.h"
#include "System.h"
#include "Math.h"
#include "App.h"
#include "Back.h"
#include "Title.h"
// 定数の定義
static const u8 const colorAnimationTable[] = { // カラーアニメーションテーブル
    0xf1, 0x41, 0x61, 0xa1
};
// 変数の定義
static u8 count; // カウント
static u8 colorTable[8];
static void TitleInit(void);
static void TitleLoad(void);
static void TitleLoop(void);
static void TitleUnload(void);
static void TitleEnd(void);
void TitleUpdate(void) {// タイトルを更新する
    if (appState == TITLE_STATE_INIT)        TitleInit();  // 初期化
    else if (appState == TITLE_STATE_LOAD)   TitleLoad();  // ロード
    else if (appState == TITLE_STATE_LOOP)   TitleLoop();  // 待機
    else if (appState == TITLE_STATE_UNLOAD) TitleUnload();// アンロード
    else                                     TitleEnd();   // 終了
    BackUpdate();// 背景の更新
}
static void TitleInit(void) {// タイトルを初期化する
    SystemClearSprite();// スプライトのクリア
    appState = TITLE_STATE_LOAD;// 状態の更新
}
static void TitleLoad(void) {// タイトルをロードする
    BackStoreLogo();// ロゴのロード
    appState = TITLE_STATE_LOOP;// 状態の更新
    appPhase = APP_PHASE_NULL;
}
static void TitleLoop(void) {// タイトルを待機する
    if (appPhase==0) { // 初期化
        count = 0x01;// ロゴの初期化
        appPhase++;
    }
    do {
        if (appPhase == 0x01) {// 待機の処理
            if (input[INPUT_BUTTON_SPACE]!=1) break;// SPACE キー
            appPhase++;// 状態の更新
        }
        // 待機の完了待ち
        if(appPhase++!=30)break;
        // ゲームスタート
        appState = TITLE_STATE_UNLOAD;// 状態の更新
    } while(0);
    if (--count) return;// ロゴの更新
    count = 4;
    // カラーの転送の設定
    u8* hl = colorTable, b = 0x08;
    do { *hl++ = 0xf1; } while(--b);
    videoTransfer.vram0_src = (i16)colorTable;
    colorTable[(SystemGetRandom()>>3) & 0b00000111] =
        colorAnimationTable[(SystemGetRandom()>>3) & 0b00000011];
    videoTransfer.vram0_dst = VIDEO_GRAPHIC1_COLOR_TABLE+0x08;
    videoTransfer.vram0_bytes = 0x08;
    request |= (1<<REQUEST_VRAM); // V-Blank 中の転送の開始
}
static void TitleUnload(void) {// タイトルをアンロードする
    BackRestoreLogo();// ロゴのアンロード
    appState = TITLE_STATE_END;// 状態の更新
}
static void TitleEnd(void) {// タイトルを終了する
    appMode = APP_MODE_GAME;// モードの更新
    appState = 0;// 状態の更新
}
