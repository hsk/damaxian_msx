// vdp.h : VDP(TMS9918A)

// レジスタ

// mode register #0
#define VDP_R0          0
// screen mode
#define VDP_R0_M2       1
// 1:enable external video input, 0:disable external video input
#define VDP_R0_EXTVID   0
// mode register #1
#define VDP_R1          1
// 1:16KB RAM, 0: 4KB RAM
#define VDP_R1_4_16K    7
// 1:screen on, 0:screen off
#define VDP_R1_BL       6
// generate interrupt
#define VDP_R1_GINT     5
// screen mode
#define VDP_R1_M1       4
// screen mode
#define VDP_R1_M3       3
// 1:16x16 sprite, 0:8x8 sprite
#define VDP_R1_SI       1
// 1:2x2 sprite pixels, 0:1x1 sprite pixels
#define VDP_R1_MAG      0
// pattern name table
#define VDP_R2          2
// pattern name table bit 13
#define VDP_R2_PN13     3
// pattern name table bit 12
#define VDP_R2_PN12     2
// pattern name table bit 11
#define VDP_R2_PN11     1
// pattern name table bit 10
#define VDP_R2_PN10     0
// color table
#define VDP_R3          3
// color table bit 13
#define VDP_R3_CT13     7
// color table bit 12
#define VDP_R3_CT12     6
// color table bit 11
#define VDP_R3_CT11     5
// color table bit 10
#define VDP_R3_CT10     4
// color table bit 9
#define VDP_R3_CT9      3
// color table bit 8
#define VDP_R3_CT8      2
// color table bit 7
#define VDP_R3_CT7      1
// color table bit 6
#define VDP_R3_CT6      0
// pattern generator table
#define VDP_R4          4
// pattern generator table bit 13
#define VDP_R4_PG13     2
// pattern generator table bit 12
#define VDP_R4_PG12     1
// pattern generator table bit 11
#define VDP_R4_PG11     0
// sprite attribute table
#define VDP_R5          5
// sprite attribute table bit 13
#define VDP_R5_SA13     6
// sprite attribute table bit 12
#define VDP_R5_SA12     5
// sprite attribute table bit 11
#define VDP_R5_SA11     4
// sprite attribute table bit 10
#define VDP_R5_SA10     3
// sprite attribute table bit 9
#define VDP_R5_SA9      2
// sprite attribute table bit 8
#define VDP_R5_SA8      1
// sprite attribute table bit 7
#define VDP_R5_SA7      0
// sprite generator table
#define VDP_R6          6
// sprite generator table bit 13
#define VDP_R6_SG13     2
// sprite generator table bit 12
#define VDP_R6_SG12     1
// sprite generator table bit 11
#define VDP_R6_SG11     0
// text color / back drop color
#define VDP_R7          7
// text color bit 3
#define VDP_R7_TC3      7
// text color bit 2
#define VDP_R7_TC2      6
// text color bit 1
#define VDP_R7_TC1      5
// text color bit 0
#define VDP_R7_TC0      4
// text color bit 3
#define VDP_R7_BD3      3
// text color bit 2
#define VDP_R7_BD2      2
// text color bit 1
#define VDP_R7_BD1      1
// text color bit 0
#define VDP_R7_BD0      0

// 色
#define VDP_COLOR_TRANSPARENT   0x00
#define VDP_COLOR_BLACK         0x01
#define VDP_COLOR_MEDIUM_GREEN  0x02
#define VDP_COLOR_LIGHT_GREEN   0x03
#define VDP_COLOR_DARK_BLUE     0x04
#define VDP_COLOR_LIGHT_BLUE    0x05
#define VDP_COLOR_DARK_RED      0x06
#define VDP_COLOR_CYAN          0x07
#define VDP_COLOR_MEDIUM_RED    0x08
#define VDP_COLOR_LIGHT_RED     0x09
#define VDP_COLOR_DARK_YELLOW   0x0a
#define VDP_COLOR_LIGHT_YELLOW  0x0b
#define VDP_COLOR_DARK_GREEN    0x0c
#define VDP_COLOR_MAGENTA       0x0d
#define VDP_COLOR_GRAY          0x0e
#define VDP_COLOR_WHITE         0x0f
