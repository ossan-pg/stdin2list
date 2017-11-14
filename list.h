#pragma once

/** リストで取り扱う文字列の最大長(終端のヌル文字分含む)。 */
#define STR_SIZE (20+1)

/**
 * 最大で size - 1 個の文字を読み込み、out が指すバッファに格納する関数へのポインタです。
 * バッファの中の最後の文字の後に '\0' が書き込まれます。
 *
 * この型の関数が out と size 以外の情報を必要とする場合は
 * arg に適切なポインタを指定してください。
 * そうではない場合、arg には NULL を指定してください。
 *
 * この型の関数が文字列の読み込みに成功した場合は読み込んだ文字列の長さを返します。
 * 読み込みに失敗した場合は負の値を返します。
 */
typedef int (*Reader)(char *out, int size, void *arg);

/**
 * 文字列 s に対して処理をする関数へのポインタです。
 *
 * この型の関数が s 以外の情報を必要とする場合は
 * arg に適切なポインタを指定してください。
 * そうではない場合、arg には NULL を指定してください。
 *
 * この型の関数の処理が正常に終了した場合は 0 を返します。
 * 処理中にエラーが発生した場合は 0 以外を返します。
 */
typedef int (*Function)(const char *s, void *arg);

/**
 * リストの要素を示す構造体です。
 */
typedef struct ListEntry {
    char str[STR_SIZE];
    struct ListEntry *next;
} ListEntry;

/**
 * リストを示す構造体です。
 */
typedef struct {
    ListEntry *head;
    size_t size;
} List;

// 参考：https://docs.oracle.com/javase/jp/8/docs/api/java/util/List.html

List
list_new(void);

int
list_init_from_reader(List *list, Reader read_line, void *arg);

int
list_add(List *list, const char *s);

int
list_foreach(List *list, Function apply, void *arg);

void
list_clear(List *list);

