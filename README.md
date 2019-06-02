
# first_follow_set

文法の First 集合と Follow 集合を標準出力に出力します。C 言語で書かれていて、標準 C ライブラリに依存しています。  
入力する文法は C 言語で記述します。自作コンパイラでエラーからの回復時に文法の First 集合と Follow 集合が必要だったので、作ってみました。

## ファイルのダウンロード時の注意事項

コマンドプロンプトで以下のコマンドを投入し、ゾーン情報を削除します。

```
echo.>first_follow_set.zip:Zone.Identifier
```

## C 言語による文法の記述例(grammer_1 ～ 3.c の tp_make_grammer_1 ～ 3 関数に例があります。テストは tp_test_grammer_1 ～ 3 関数です)

```
// E' -> TE' | ε
TP_GRAMMER* E_TMP = MAKE_GRAMMER_ELEMENT(
    symbol_table, TP_PARSE_TREE_GRAMMER_E_TMP,
    TP_NONTERM_SYMBOL_REFERENCE(TP_PARSE_TREE_GRAMMER_T),
    TP_NONTERM_SYMBOL_REFERENCE(TP_PARSE_TREE_GRAMMER_E_TMP),
    TP_TERM_PIPE,
    TP_TERM_SYMBOL_EMPTY,
    TP_TERM_NULL
);
```

## bin フォルダのコマンドの実行方法

コマンドプロンプトで以下のコマンドを投入すると、テキストファイル(first_follow_set.txt)に実行結果が出力されます。  
出力済みの first_follow_set.txt を同梱しています。

```
first_follow_set.exe > first_follow_set.txt
```

セキュリティ警告が表示されて実行できない場合は、コマンドプロンプトで以下のコマンドを投入し、ゾーン情報を削除します。

```
echo.>first_follow_set.exe:Zone.Identifier
```

コマンドライン引数は、以下の通りです。

```
usage: first_follow_set [-/][hn]
  -h : disp usage.
  -n : disable test.
```

## 開発環境

* Visual Studio Community 2019  
https://docs.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk
* 64 ビット版 Windows 10

## ビルド方法

* first_follow_set.sln ファイルをダブルクリックします。
* ゾーン情報を削除していない場合は、Visual Studio のセキュリティ警告を回避してプロジェクトを開きます。  
![warning](https://raw.githubusercontent.com/tenpoku1000/first_follow_set/master/images/MSVC.PNG)
* F7 キーを押下します。

## ライセンス

[MIT license](https://raw.githubusercontent.com/tenpoku1000/first_follow_set/master/LICENSE)

## 作者

市川 真一 <suigun1000@gmail.com>

## 参考文献

コンパイラ (情報系教科書シリーズ)  
湯浅 太一  
昭晃堂 2010/03 初版 第11刷

上記の書籍は、現在では出版社が変更になっています。

情報系教科書シリーズ コンパイラ  
湯淺 太一  
オーム社 2014/09

岩波講座 ソフトウェア科学〈〔環境〕5〉プログラミング言語処理系  
佐々 政孝  
岩波書店 1997/03 初版 第7刷

コンパイラの構成と最適化  
中田 育男  
朝倉書店 2011/03/30 第2版 第2刷

新世代工学シリーズ システムソフトウェア  
黒住 祥祐, 林 恒俊  
オーム社 2004/03 初版 第2刷

