// Back.c : 背景
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Back.h"
// 定数の定義

void BackLoad(void) { // 背景をロードする
    SetVram(VIDEO_GRAPHIC1_PATTERN_NAME_TABLE,0,0x300);// パターンネームテーブルのクリア
    static const u8 const colorTable[] = { // カラーテーブル
        0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1,
        0x01, 0x01, 0x51, 0x51, 0x91, 0x91, 0xb1, 0xb1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1,
    };
    LoadVram(VIDEO_GRAPHIC1_COLOR_TABLE,colorTable,0x20);// カラーテーブルを読み込む
}
