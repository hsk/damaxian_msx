// Bullet.h : ゲーム画面／弾
// マクロの定義
    // 弾
        #define BULLET_SIZE            18
    // 状態
        #define BULLET_STATE_NULL      0
        #define BULLET_STATE_MOVE      1
// 外部関数宣言
    void BulletInitialize(void);
    void BulletUpdate(void);
    void BulletEntry(void);
typedef struct {
    u8 state;
    //u8 phase;
    union {
        struct {
            u8 xd;
            u8 xi;
        };
        i16 x;
    };
    union {
        struct {
            u8 yd;
            u8 yi;
        };
        i16 y;
    };
    union {
        struct {
            u8 spxd;
            u8 spxi;
        };
        i16 spx;
    };
    union {
        struct {
            u8 spyd;
            u8 spyi;
        };
        i16 spy;
    };
    union {
        struct {
            u8 sprite_src_l;
            u8 sprite_src_h;
        };
        i16 sprite_src;
    };
    u8 sprite_offset;
} BULLET;
// 外部変数宣言
    extern BULLET bullets[BULLET_SIZE];
    extern BULLET bulletEntry;
