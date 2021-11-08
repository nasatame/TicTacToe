# TicTacToe
電気通信大学MMAの2021年秋に部誌に投稿した、”〇×ゲームの全探索をやってみよう”のコードです。MITにしておきますので、各自勉強や研究にご自由にお使いください。よければ部誌もリンク先から読むことが出来ますのでよろしくお願いいたします。　  
"2021年 秋号　〇×ゲームの全探索をやってみよう"をご参照ください。  
https://wiki.mma.club.uec.ac.jp/Booklet

## HOW TO COMPILE
開発環境は、"Visual Studio 2019 Community" + "C++20"です。  
C++20がコンパイルできる環境なら動くと思われます。  
STLを使用している一部(contain)を書き換えれば、C++のバージョンを下げても動くと思います。  

## HOW TO USE
人間同士で対戦するなら、580行目付近をアンコメントしてください。  
自動対戦が不要なら、584行目付近から628行目付近をコメントアウトしてください。  
細かい仕様については、コードか部誌を読んでください。  

## RESULT
実行結果については、Result.mdにまとめてあります。大きく分けて以下の3つについて述べてあります。

1. 〇×ゲームの全探索結果から分かるゲームの性質について
2. ハッシュテーブルを用いることによる高速化効果
3. この手法の一般化について
