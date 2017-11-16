# stdin2list

## これはなに

C言語の研修課題(こんなプログラム作れ)に取り組んだ結果だよ。

標準入力から取得した文字列をソートして指定されたファイルに出力するやつだよ。

## なにすればいいの

```
$ git clone https://github.com/ossan-pg/stdin2list.git
$ cd stdin2list/
$ make
```

## つかいかた

```
stdin2list <file>
```

詳細については下記を読んでね。

## 説明

`stdin2list` は標準入力から複数の文字列を読み込み、`file` に追記する。
このとき `file` が存在しなければ作成する。

`file` が指定されていない場合、標準エラーに下記のメッセージを出力して処理を終了する。

```
出力用のファイルが指定されていません。プログラムを終了します。
```

`file` のオープンに失敗した場合、標準エラーに下記のメッセージを出力して処理を終了する。

```
ファイル 'file' のオープンに失敗しました。プログラムを終了します。
```

標準入力から文字列を読み込む際はユーザに対し下記のようにプロンプトを表示する。
```
1個目の文字列を入力して下さい。：xxx
2個目の文字列を入力して下さい。：aaa
3個目の文字列を入力して下さい。：
```

長さが 0 の文字列を読み込んだ(ユーザが何も入力せずエンターキーを押した)場合、
文字列の読み込みを終了する。

その後、これまでに読み込んだ文字列を文字コードで昇順にソートし `file` に追記する。

文字列の追記後は空行をファイルに出力してプログラムを終了する。

```
$ cat output.txt
zzz

$ ./stdin2list output.txt
1個目の文字列を入力して下さい。：xxx
2個目の文字列を入力して下さい。：aaa
3個目の文字列を入力して下さい。：bbb
4個目の文字列を入力して下さい。：
文字列のソート結果をファイルに出力しました。
$ cat output.txt
zzz

aaa
bbb
xxx

```

### 有効な文字列

文字列は半角英数字のみを有効なものとし、それ以外の文字が含まれていた場合は
標準エラーに下記のメッセージを出力して処理を終了する。

```
半角英数字以外が入力されたため、プログラムを終了します。
```

また、1つの文字列につき最大20バイトまでを読み込む対象とし、それを超える分は読み捨てる。

### 名前の由来

`stdin2list` の `list` の部分はプログラム内部のデータ構造に由来する。

課題の内容からとりあえず名前を決めたらユーザにとって分かりにくい名前になってしまった。

---

## メモ：課題で指定された仕様との差異

課題で指定があったが、変更した、または実装していない仕様等は下記。

### 入力時のプロンプト文字列

指定された仕様では入力時のプロンプト文字列の `：` の直後に半角スペースが存在するが、
入力文字列の区切りが分かりにくいため出力しないようにした。

### ソースコードのインデント

ソースコードのインデントは水平タブ文字(＋幅は半角スペース4文字分)と指定されていたが、
GitHub では水平タブが半角スペース8文字分で表示されて見にくいため
半角スペース4文字に変更した。
