# DAMAXIAN MSX SDCC

DAMAXIAN MSX SDCC はMSX上で動作するシューティングゲームです。
オリジナルはcobineeさんのオールアセンブラで作られているものでそれをC言語化しました。
固定小数点数を用いたレトロなゲーム機でも動作するプログラムの経験を積むための学習用コードとして再編集しました。

プログラムの履歴を美しく整え順を追ってみることが出来ます。
写経をしながらプログラムすることで構造を理解し自信を持って固定小数点数を用いたシューティングゲームを作れるようになることを目指します。

## 各ステップで行うこと

DAMAXIAN MSX SDCC は以下のステップに分けて作られています:

1. [自機を動かす](https://github.com/hsk/damaxian_msx/compare/0..1)
2. [敵機を出現させる](https://github.com/hsk/damaxian_msx/compare/1..2)
3. [弾をうち当たり判定をつける](https://github.com/hsk/damaxian_msx/compare/2..3)
4. [敵が襲ってくるようにする。sin,cosを用いる](https://github.com/hsk/damaxian_msx/compare/3..4)
5. [敵を倒すと撃ち返ししてくる。atan2を用いる](https://github.com/hsk/damaxian_msx/compare/4..5)
6. [撃ち返しを5wayにする。](https://github.com/hsk/damaxian_msx/compare/5..6)
7. [自機に当たり判定をつける。](https://github.com/hsk/damaxian_msx/compare/6..7)
8. [BG を読み込む](https://github.com/hsk/damaxian_msx/compare/7..8)
9. [タイマーをつける](https://github.com/hsk/damaxian_msx/compare/8..9)
10. [タイトル画面を出す](https://github.com/hsk/damaxian_msx/compare/9..10)
11. [タイトル画面をアニメーションさせる](https://github.com/hsk/damaxian_msx/compare/10..11)
12. [スコアをつける](https://github.com/hsk/damaxian_msx/compare/11..12)
13. [スコアにレートを加える](https://github.com/hsk/damaxian_msx/compare/12..13)
14. [ハイスコアを記録する](https://github.com/hsk/damaxian_msx/compare/13..14)
15. [音をつける](https://github.com/hsk/damaxian_msx/compare/14..15)

1日１ステップで作ると半月で作ることができる内容になっています。
