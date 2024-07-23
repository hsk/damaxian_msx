// bdos.h : MSX-DOS システムコール
// マクロの定義

// システムコールアドレス
#define BDOS        0x0005
// TPA アドレス
#define TPA         0x0006

// システムコール

// システムリセット
#define INIDOS      0x00
// コンソール入力
#define GETCON      0x01
// コンソール出力
#define PUTCON      0x02
// 外部入力
#define GETAUX      0x03
// 外部出力
#define PUTAUX      0x04
// プリンタ出力
#define PUTLPT      0x05
// 直接コンソール入出力
#define IODCN       0x06
// 直接コンソール入力その１
#define GETDCN1     0x07
// 直接コンソール入力その２
#define GETDCN2     0x08
// 文字列出力
#define PUTSTR      0x09
// 文字列入力
#define GETSTR      0x0a
// コンソールの状態チェック
#define CHKCON      0x0b
// バージョン番号の取得
#define GETVER      0x0c
// ディスクリセット
#define RSTDSK      0x0d
// デフォルト・ドライブの変更
#define CHGDRV      0x0e
// ファイルのオープン
#define OPNFCB      0x0f
// ファイルのクローズ
#define CLSFCB      0x10
// ファイルの検索その１
#define FNDFCB1     0x11
// ファイルの検索その２
#define FNDFCB2     0x12
// ファイルの抹消
#define DELFCB      0x13
// シーケンシャルな読み出し
#define SRDFCB      0x14
// シーケンシャルな書き出し
#define SWRFCB      0x15
// ファイルの作成
#define CRTFCB      0x16
// ファイル名の変更
#define RENFCB      0x17
// ログイン・ベクトルの獲得
#define GETLGV      0x18
// デフォルト・ドライブの獲得
#define GETDRV      0x19
// 転送先アドレスの設定
#define SETDMA      0x1a
// ディスク情報の獲得
#define GETDSK      0x1b
// ランダムな読み出し
#define RRDFCB      0x21
// ランダムな書き出し
#define RWRFCB      0x22
// ファイルサイズの獲得
#define GETLEN      0x23
// ランダムレコード・フィールドの設定
#define SETREC      0x24
// ランダムな書き込みその２
#define RWRREC      0x26
// ランダムな読み出しその２
#define RRDREC      0x27
// ランダムな書き込みその３
#define FILREC      0x28
// 日付の獲得
#define GETDAT      0x2a
// 日付の設定
#define SETDAT      0x2b
// 時刻の獲得
#define GETTIM      0x2c
// 時刻の設定
#define SETTIM      0x2d
// ベリファイ・フラグの設定
#define SETVFY      0x2e
// 論理セクタを用いた読み出し
#define ARDSEC      0x2f
// 論理セクタを用いた書き込み
#define AWRSEC      0x30
