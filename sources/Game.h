// Game.h : ゲーム画面
// マクロの定義
// 状態
#define GAME_STATE_INITIALIZE   0
#define GAME_STATE_LOAD         1
#define GAME_STATE_START        2
#define GAME_STATE_PLAY         3
#define GAME_STATE_TIMEUP       4
#define GAME_STATE_OVER         5
#define GAME_STATE_HISCORE      6
#define GAME_STATE_UNLOAD       7
#define GAME_STATE_END          8
// スプライト
#define GAME_SPRITE_BULLET          0x00
#define GAME_SPRITE_BULLET_OFFSET   0x24
#define GAME_SPRITE_ENEMY           0x48
#define GAME_SPRITE_ENEMY_OFFSET    0x48
#define GAME_SPRITE_SHIP            0x6c
#define GAME_SPRITE_SHOT            0x70
// フラグ
#define GAME_FLAG_PLAYABLE      0
#define GAME_FLAG_PAUSE         1
#define GAME_FLAG_STATUS        2
// 外部関数宣言
void GameUpdate(void);
// 外部変数宣言
extern u8 gameFlag;
extern u8 gameShootDown;
