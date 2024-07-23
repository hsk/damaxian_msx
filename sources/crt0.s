; crt0.s : ブートコード
    .module crt0
    .include    "bios.inc"
; 外部関数宣言
    .globl  _main
; HEADER 領域（プログラムのエントリポイント）
    .area   _HEADER (ABS)
    .org    0x8400
_boot: ; ブートコード
    ; 変数の初期化
        LDHL    sp, #_stackfinal
        call    _gsinit
    ; main() 関数の呼び出し
        call    _main
_terminated:: ; 終了コード
    ld      hl, #_terminatedCode ; NEWSTT
    jp      0x4601
; 定数定義
_terminatedCode: .db     0x3a, 0x94, 0x00 ; 終了コード
    .area   _CODE
    .area   _GSINIT
_gsinit: ; 変数の初期化
    ; GSFINAL 領域
    .area   _GSFINAL
_gsfinal: ; 変数の初期化の完了
    ret
    .area   _DATA
_data:; DATA 領域の開始
_stack: .ds     256 ; スタック領域
_stackfinal: ; DATA 領域の末端
    .area   _DATAFINAL
_datafinal: ; DATA 領域の終了
