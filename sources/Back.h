// Back.h : 背景
// マクロの定義
// パターンネームテーブルオフセットアドレス
#define BACK_PATTERN_NAME_TABLE_TIMER       (20 * 0x20 + 27)
#define BACK_PATTERN_NAME_TABLE_MESSAGE     (12 * 0x20 +  4)
// メッセージ
#define BACK_MESSAGE_START      0
#define BACK_MESSAGE_TIMEUP     1
#define BACK_MESSAGE_GAMEOVER   2
// 外部関数宣言
void BackLoad(void);
void BackUpdate(void);
void BackTransferStatus(void);
void BackStoreMessage(u8 n);
void BackRestoreMessage(void);
