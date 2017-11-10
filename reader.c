#include <stdio.h>
#include <string.h>

/**
 * 標準入力から最大で size - 1 個の文字を読み込み、out が指すバッファに格納します。
 * バッファの中の最後の文字の後に'\0' が書き込まれます。
 *
 * arg が NULL 以外の場合、標準入力からの文字列読み込み前に
 * 標準出力へプロンプトとして "n個目の文字列を入力して下さい。：" を出力します。
 * この時 n は arg が指すアドレスから取得した int 値です。
 * また、プロンプト出力後は *arg の値を +1 します。
 * 下記の場合、標準出力には "1個目の文字列を入力して下さい。：" と "n = 2" が
 * 出力されます。
 *
 * char buff[10 + 1];
 * int n = 1;
 * int len = read_stdin(buff, sizeof(buff), &n);
 * printf("n = %d\n", n);
 *
 * arg が NULL の場合はプロンプトを出力しません。
 *
 * @retval -1 文字の読み込みに失敗した。
 * @retval 0以上 読み込んだ文字列の長さ。
 */
int
read_stdin(char *out, int size, void *arg)
{
	if(arg != NULL) {
		int *count = arg;
		printf("%d個目の文字列を入力して下さい。：", *count);
		(*count)++;
	}
	char *s = fgets(out, size, stdin);
	if(s == NULL) {
		return -1;
	}
	char *p = strchr(s, '\n');
	if(p != NULL) {
		*p = '\0';
	}

	// 実用上、読み込んだ文字列長は INT_MAX を超えない見込み
	return (int)strlen(s);
}

