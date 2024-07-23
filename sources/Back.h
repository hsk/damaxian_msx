// Back.h : 背景
// マクロの定義
// パターンネームテーブルオフセットアドレス
#define BACK_PATTERN_NAME_TABLE_SCORE       ( 7 * 0x20 + 25)
#define BACK_PATTERN_NAME_TABLE_RATE        (18 * 0x20 + 27)
#define BACK_PATTERN_NAME_TABLE_TIMER       (20 * 0x20 + 27)
#define BACK_PATTERN_NAME_TABLE_LOGO        (10 * 0x20 +  4)
#define BACK_PATTERN_NAME_TABLE_MESSAGE     (12 * 0x20 +  4)
// メッセージ
#define BACK_MESSAGE_START      0
#define BACK_MESSAGE_TIMEUP     1
#define BACK_MESSAGE_GAMEOVER   2
// 外部関数宣言
void BackLoad(void);
void BackUpdate(void);
void BackTransferStatus(void);
void BackStoreLogo(void);
void BackRestoreLogo(void);
void BackStoreMessage(u8 n);
void BackRestoreMessage(void);
