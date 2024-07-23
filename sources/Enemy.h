// Enemy.h : ゲーム画面／敵
// 敵
    #define ENEMIES_SIZE           9
// 状態
    #define ENEMY_STATE_NULL       0
    #define ENEMY_STATE_IN         1
    #define ENEMY_STATE_STAY       2
    #define ENEMY_STATE_BOMB       3
typedef struct {
    u8 state;
    u8 phase;
    union {
        struct{
            u8 xd;
            u8 xi;
        };
        i16 x;
    };
    union {
        struct{
            u8 yd;
            u8 yi;
        };
        i16 y;
    };
    union {
        struct {
            u8 xs;
            u8 ys;
        };
        i16 ss;
    };
    u8 angle;
    u8 turn;
    u8 nodamage;
    u8 count0;
    u8 count1;
    u8 animation;
    union {
        struct{
        u8 sprite_src_l;
        u8 sprite_src_h;
        };
        i16 sprite_src;
    };
    u8 sprite_offset;
} ENEMY;
// 外部関数宣言
    void EnemyInitialize(void);
    void EnemyUpdate(void);
// 外部変数宣言
    extern ENEMY enemies[ENEMIES_SIZE];
