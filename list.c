#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/**
 * read_line から 1行ずつ読み込んだ各文字列を要素に持つリストを構築します。
 * このリストの要素は文字列について文字コードで昇順にソートされています。
 *
 * read_line が 0 を返した場合、入力の終端に到達したとみなして
 * 文字列の読み込みを終了します。
 *
 * @retval NULL メモリ領域の確保失敗や文字列の読み込み失敗などの理由で
 *              リストの構築に失敗した。
 * @retval NULL以外 構築したリストのアドレス。
 */
List *
list_new_from_reader(Reader read_line, void *arg)
{
	List *list = malloc(sizeof(List));
	if(list == NULL) {
		fprintf(stderr, "List 用のメモリ領域の確保に失敗しました。");
		return NULL;
	}

	for(;;) {
		char out[STR_SIZE];
		int len = read_line(out, sizeof(out), arg);
		if(len < 0) {
			fprintf(stderr, "文字列の読み込みに失敗しました。");
			list_delete(list);
			return NULL;
		} else if(len == 0) {
			// 読み込む文字列が存在しない場合、終了
			break;
		}

		int result = list_add(list, out);
		if(result != 0) {
			list_delete(list);
			return NULL;
		}
	}
	return list;
}

/**
 * list の要素が文字列について文字コードで
 * 昇順にソートされた状態になる位置へ s を 追加します。
 *
 * @retval 0 list への s の追加に成功した。
 * @retval 0以外 メモリ領域の確保失敗により list への s の追加に失敗した。
 */
int
list_add(List *list, const char *s)
{
	ListEntry *new_entry = malloc(sizeof(ListEntry));
	if(new_entry == NULL) {
		fprintf(stderr, "ListEntry 用のメモリ領域の確保に失敗しました。");
		return 1;
	}
	strncpy(new_entry->str, s, sizeof(new_entry->str));

	// 文字列について文字コードで昇順にソートする
	ListEntry **entry = &(list->head);
	while(*entry != NULL) {
		if(strcmp(new_entry->str, (*entry)->str) <= 0) {
			break;
		}
		entry = &((*entry)->next);
	}
	new_entry->next = *entry;
	*entry = new_entry;

	list->size++;

	return 0;
}

/**
 * list の先頭の文字列から順に apply を実行します。
 * apply が 0 以外の値を返した場合、その時点で処理を中断し
 * apply の戻り値を返します。
 *
 * @retval 0 正常に終了した。
 * @retval 0以外 最後に実行した apply の戻り値。
 */
int
list_foreach(List *list, Function apply, void *arg)
{
	ListEntry *entry = list->head;
	while(entry != NULL) {
		int result = apply(entry->str, arg);
		if(result != 0) {
			return result;
		}
		entry = entry->next;
	}
	return 0;
}

/**
 * list および list の全要素のメモリ領域を解放します。
 * list が NULL の場合は何もしません。
 */
void
list_delete(List *list)
{
	if(list == NULL) {
		return;
	}

	ListEntry *entry = list->head;
	while(entry != NULL) {
		ListEntry *next = entry->next;
		free(entry);
		entry = next;
	}
	free(list);
}

