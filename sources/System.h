// System.h : システムライブラリ

// マクロの定義
    // フラグ
        #define FLAG_H_TIMI             0
        #define FLAG_SOUND_SLEEP        1
        #define FLAG_CANCEL             7
    // リクエスト
        #define REQUEST_NULL            0
        #define REQUEST_VIDEO_REGISTER  1
        #define REQUEST_VRAM            2
    // キー入力
        #define INPUT_KEY_UP            0
        #define INPUT_KEY_DOWN          1
        #define INPUT_KEY_LEFT          2
        #define INPUT_KEY_RIGHT         3
        #define INPUT_BUTTON_SPACE      4
        #define INPUT_BUTTON_RETURN     5
        #define INPUT_BUTTON_ESC        6
        #define INPUT_BUTTON_STOP       7
        #define INPUT_SIZE              8
    // ビデオ
        #define VIDEO_TEXT1                                 0
        #define VIDEO_TEXT1_PATTERN_NAME_TABLE              0x0000
        #define VIDEO_TEXT1_PATTERN_GENERATOR_TABLE         0x0800
        #define VIDEO_TEXT1_COLOR_TABLE                     0x0000
        #define VIDEO_TEXT1_SPRITE_ATTRIBUTE_TABLE          0x1b00
        #define VIDEO_TEXT1_SPRITE_GENERATOR_TABLE          0x3800
        #define VIDEO_GRAPHIC1                              1
        #define VIDEO_GRAPHIC1_PATTERN_NAME_TABLE           0x1800
        #define VIDEO_GRAPHIC1_PATTERN_GENERATOR_TABLE      0x0000
        #define VIDEO_GRAPHIC1_COLOR_TABLE                  0x2000
        #define VIDEO_GRAPHIC1_SPRITE_ATTRIBUTE_TABLE       0x1b00
        #define VIDEO_GRAPHIC1_SPRITE_GENERATOR_TABLE       0x3800
        #define VIDEO_GRAPHIC2                              2
        #define VIDEO_GRAPHIC2_PATTERN_NAME_TABLE           0x1800
        #define VIDEO_GRAPHIC2_PATTERN_GENERATOR_TABLE      0x0000
        #define VIDEO_GRAPHIC2_COLOR_TABLE                  0x2000
        #define VIDEO_GRAPHIC2_SPRITE_ATTRIBUTE_TABLE       0x1b00
        #define VIDEO_GRAPHIC2_SPRITE_GENERATOR_TABLE       0x3800
        #define VIDEO_MULTICOLOR                            3
        #define VIDEO_MULTICOLOR_PATTERN_NAME_TABLE         0x0800
        #define VIDEO_MULTICOLOR_PATTERN_GENERATOR_TABLE    0x0000
        #define VIDEO_MULTICOLOR_COLOR_TABLE                0x2000
        #define VIDEO_MULTICOLOR_SPRITE_ATTRIBUTE_TABLE     0x1b00
        #define VIDEO_MULTICOLOR_SPRITE_GENERATOR_TABLE     0x3800
        //#define VIDEO_TRANSFER_VRAM_0                       0
        //#define VIDEO_TRANSFER_VRAM_1                       1
        //#define VIDEO_TRANSFER_VRAM_2                       2
        //#define VIDEO_TRANSFER_VRAM_3                       3
        #define VIDEO_TRANSFER_VRAM_SIZE                    4
        #define VIDEO_TRANSFER_SRC                          0
        #define VIDEO_TRANSFER_DST                          2
        #define VIDEO_TRANSFER_BYTES                        4
        #define VIDEO_TRANSFER_SIZE                         5

        //#define VIDEO_TRANSFER_VRAM_0                       0
        #define VIDEO_TRANSFER_VRAM_0_SRC                   0
        #define VIDEO_TRANSFER_VRAM_0_DST                   2
        #define VIDEO_TRANSFER_VRAM_0_BYTES                 4

        //#define VIDEO_TRANSFER_VRAM_1                       5
        #define VIDEO_TRANSFER_VRAM_1_SRC                   5
        #define VIDEO_TRANSFER_VRAM_1_DST                   7
        #define VIDEO_TRANSFER_VRAM_1_BYTES                 9

        #define VIDEO_TRANSFER_VRAM_2                       10
        #define VIDEO_TRANSFER_VRAM_2_SRC                   10
        #define VIDEO_TRANSFER_VRAM_2_DST                   12
        #define VIDEO_TRANSFER_VRAM_2_BYTES                 14

        //#define VIDEO_TRANSFER_VRAM_3                       15
        #define VIDEO_TRANSFER_VRAM_3_SRC                   15
        #define VIDEO_TRANSFER_VRAM_3_DST                   17
        #define VIDEO_TRANSFER_VRAM_3_BYTES                 19

typedef struct {
        u16 vram0_src;
        u16 vram0_dst;
        u8  vram0_bytes;
        u16 vram1_src;
        u16 vram1_dst;
        u8  vram1_bytes;
        u16 vram2_src;
        u16 vram2_dst;
        u8  vram2_bytes;
        u16 vram3_src;
        u16 vram3_dst;
        u8  vram3_bytes;
} VIDEO_TRANSFER;

// 外部関数宣言
    // システム
        void SystemInitialize(void);
    // キー入力
        void SystemInitializeInput(void);
        void SystemUpdateInput(void);
    // ビデオ
        void SystemInitializeVideo(void);
        void SystemSetScreenMode(u8 mode);
        void SystemTransferVideoRegister(void);
        void SystemTransferVram(void);
        void LoadVram(u16 vram, u8*addr, u16 size);
        void SetVram(u16 vram, u8 data, u16 size);
    // スプライト
        void SystemClearSprite(void);
        void SystemTransferSprite(void);
        void SystemSetSprite(u8* hl,u8*de,u8 x, u8 y);

// 外部変数宣言
    // フラグ
        extern u8 flag;
    // リクエスト
        extern u8 request;
    // キー入力
        extern u8 input[INPUT_SIZE];
    // ビデオ
        extern u8 videoPort[2];
        extern u8 videoRegister[8];
        extern VIDEO_TRANSFER videoTransfer;
    // スプライト
        extern u8 sprite[128];
