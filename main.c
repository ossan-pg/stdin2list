#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "list.h"

/**
 * 文字列読み込み時のエラーを示す列挙型です。
 * Reader 型の関数の戻り値として使用するため、全て負の値です。
 */
enum ReadFailureReason {
    IO_ERROR    = -1, ///< 文字列の読み込みに失敗した。
    INVALID_STR = -2, ///< 読み込んだ文字列に不正な文字が含まれていた。
};

/**
 * 標準入力から最大で size - 1 個の文字を読み込み、out が指すバッファに格納します。
 * バッファの中の最後の文字の後に '\0' が書き込まれます。
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
 * @retval 0以上       読み込んだ文字列の長さ。
 * @retval IO_ERROR    文字列の読み込みに失敗した。
 * @retval INVALID_STR 読み込んだ文字列に半角英数字以外が含まれていた。
 */
static int
read_stdin(char *out, int size, void *arg)
{
    if(arg != NULL) {
        int *count = arg;
        printf("%d個目の文字列を入力して下さい。：", *count);
        (*count)++;
    }

    char *s = fgets(out, size, stdin);
    if(s == NULL) {
        return IO_ERROR;
    }
    char *p = strchr(s, '\n');
    if(p == NULL) {
        // '\n' がない == size 以上の文字列が入力された状態。
        // 入力バッファに文字列が残っているのでそれをクリアする。
        for(;;) {
            int c = fgetc(stdin);
            if(c == '\n' || c == EOF) {
                break;
            }
        }
    } else {
        *p = '\0';
    }

    // 半角英数字のみを有効とする
    size_t len = strlen(s);
    for(size_t i = 0; i < len; i++) {
        if(!isalnum(s[i])) {
            return INVALID_STR;
        }
    }

    // fgets() で指定するバッファサイズ(第2引数) は int 型なので
    // s の長さは INT_MAX を超えない。
    return (int)len;
}

/**
 * arg が示すファイルに対し、s と改行を書き込みます。
 *
 * この関数では arg をFILE * 型の値として参照します。
 * arg にはオープンしたファイルへのポインタを指定してください。
 *
 * @retval 0 この関数は常に 0 を返す。
 */
static int
write_file(const char *s, void *arg)
{
    // 書き込みは失敗しないものとする。問題があればそのとき対応する。
    fprintf(arg, "%s\n", s);

    return 0;
}

/**
 * 標準入力から文字列を読み込み、file へ出力します。
 *
 * @retval EXIT_SUCCESS 処理成功。
 * @retval EXIT_FAILURE 処理失敗。
 */
static int
stdin2file(FILE *file)
{
    /*
     * この関数は「main() の処理の一部を切り出したもの」という位置付けのため
     * プログラムの終了に関するエラーメッセージもこの関数内で出力している。
     *
     * 戻り値についても main() の戻り値で使用する定数を使用している。
     */

    // 標準入力からの読み込み＋ソート済み文字列リスト作成
    List list = list_new();
    int n = 1;
    int result = list_init_from_reader(&list, read_stdin, &n);
    if(result > 0) {
        fprintf(stderr,
                "入力された文字列からのリスト構築に失敗したため、プログラムを終了します。\n");
        return EXIT_FAILURE;
    } else if(result < 0) {
        switch(result) {
        case INVALID_STR:
            fprintf(stderr,
                    "半角英数字以外が入力されたため、プログラムを終了します。\n");
            break;
        default:
            fprintf(stderr,
                    "文字列の読み込みに失敗したため、プログラムを終了します。\n");
            break;
        }
        return EXIT_FAILURE;
    }

    // 読み込んだ文字列をファイルへ出力
    list_foreach(&list, write_file, file);
    // 文字列の後は空行で終了する
    write_file("", file);
    printf("文字列のソート結果をファイルに出力しました。\n");

    list_clear(&list);

    return EXIT_SUCCESS;
}

int
main(int argc, char *argv[])
{
    if(argc <= 1) {
        fprintf(stderr,
                "出力用のファイルが指定されていません。プログラムを終了します。\n");
        return EXIT_FAILURE;
    }

    const char *file_name = argv[1];
    FILE *file = fopen(file_name, "a");
    if(file == NULL) {
        fprintf(stderr,
                "ファイル '%s' のオープンに失敗しました。プログラムを終了します。\n",
                file_name);
        return EXIT_FAILURE;
    }
    int result = stdin2file(file);
    fclose(file);

    return result;
}

