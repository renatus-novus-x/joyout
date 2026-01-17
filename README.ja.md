[English version is here](./README.md)

[![ubuntu](https://github.com/renatus-novus-x/joyout/workflows/ubuntu/badge.svg)](https://github.com/renatus-novus-x/joyout/actions?query=workflow%3Aubuntu)

<img src="https://raw.githubusercontent.com/renatus-novus-x/joyout/main/images/tether.gif" title="tether" />

# joyout

X68000 用 Atari 9ピン・ジョイポート出力テスター：キーボードの 1〜4 を 4本のデジタル出力ライン（ポート1の pin 6/7/8 ＋ ポート2の pin 8）に割り当て、USB ジョイスティックアダプタ等を駆動するために使います。

## 使い方

1. `joyout.x` をビルドします（`src/` 内の Makefile を参照）。
2. X68000（または互換環境）で `joyout.x` を実行します。
3. キー `1`, `2`, `3`, `4` を押している間、対応する4つの出力ラインが切り替わります（押している間だけ有効）：
   - `1` -> ジョイポート1 pin 6
   - `2` -> ジョイポート1 pin 7
   - `3` -> ジョイポート1 pin 8
   - `4` -> ジョイポート2 pin 8
4. `ESC` キーで終了します。

## 注意

- このツールは、システムI/O（8255 の Port C）経由で Atari 9ピン・ジョイポートの出力ラインを駆動します。
- 外部機器（例：USB ジョイスティックアダプタ）と接続する際は、GND（グランド）を共通にしてください。

## ダウンロード
- [joyout.x](https://raw.githubusercontent.com/renatus-novus-x/joyout/main/bin/joyout.x)
