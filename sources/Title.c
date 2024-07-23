// Title.c : タイトル
#include "bios.h"
#include "System.h"
#include "App.h"
#include "Back.h"
#include "Title.h"
// 変数の定義
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
    if (appPhase==0)appPhase++;// 初期化
    do {
        if (appPhase == 0x01) {// 待機の処理
            if (input[INPUT_BUTTON_SPACE]!=1) break;// SPACE キー
        }
        // 待機の完了待ち
        if(appPhase++!=30)break;
        // ゲームスタート
        appState = TITLE_STATE_UNLOAD;// 状態の更新
    } while(0);
}
static void TitleUnload(void) {// タイトルをアンロードする
    BackRestoreLogo();// ロゴのアンロード
    appState = TITLE_STATE_END;// 状態の更新
}
static void TitleEnd(void) {// タイトルを終了する
    appMode = APP_MODE_GAME;// モードの更新
    appState = 0;// 状態の更新
}
