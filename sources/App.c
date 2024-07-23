// App.c : アプリケーション
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "Math.h"
#include "App.h"
#include "Back.h"
#include "Title.h"
#include "Game.h"
// 外部変数宣言
extern const u8 const pattern[];
extern const u8 const spritePattern[];
// 変数の定義
u8 appMode;         // モード
u8 appState;        // 状態
u8 appPhase;
u8 appScore[6];     // 現在のスコア
u8 appRate[4];      // スコアの倍率
u8 appTimer[4];     // タイマ

void AppInitialize(void) __naked { // アプリケーションを初期化する
    // スクリーンモードの設定
    SystemSetScreenMode(VIDEO_GRAPHIC1);
    DI();// 割り込みの禁止
    // パターンジェネレータの転送
    LoadVram(VIDEO_GRAPHIC1_PATTERN_GENERATOR_TABLE,pattern,0x800);
    // スプライトジェネレータの転送
    LoadVram(VIDEO_GRAPHIC1_SPRITE_GENERATOR_TABLE,spritePattern,0x800);
    EI();// 割り込み禁止の解除
    // アプリケーションの初期化
        // 現在のスコアの初期化
        for(u8 i=0;i<6;i++) appScore[i]=0;
        // スコアの倍率の初期化
        for(u8 i=0;i<4;i++) appRate[i]=0;
        appRate[1]=1;
        // タイマの初期化
        for(u8 i=0;i<4;i++) appTimer[i]=0;
    appMode = APP_MODE_LOAD;// モードの初期化
}
static void AppLoad(void) { // アプリケーションを読み込む
    BackLoad();// 背景のロード
    videoRegister[VDP_R7] = 0x07;// 背景色の設定
    videoRegister[VDP_R1] |= (1 << VDP_R1_BL)|(1<<VDP_R1_SI);// スプライトの設定と表示の開始
    request |= (1 << REQUEST_VIDEO_REGISTER);// V-Blank 中の転送の開始
    appMode = APP_MODE_TITLE;// モードの更新
    appState = TITLE_STATE_INIT;// 状態の更新
}
void AppUpdate(void) { // アプリケーションを更新する
    if (appMode == APP_MODE_LOAD) AppLoad();          // 読み込み
    else if (appMode == APP_MODE_TITLE) TitleUpdate();// タイトル画面
    else if (appMode == APP_MODE_GAME) GameUpdate();  // ゲーム画面
    SystemGetRandom();
}
