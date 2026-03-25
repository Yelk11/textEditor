#ifndef UI_H
#define UI_H

#include "rope.h"

typedef struct {
    ropeNode* content;
    int cursor_row;
    int cursor_col;
    int view_offset_row;
    int view_offset_col;
    int max_rows;
    int max_cols;
    int modified;
} Editor;

Editor* editor_create(void);
void editor_free(Editor* editor);
void editor_render(Editor* editor);
void editor_handle_input(Editor* editor, int ch);
void editor_insert_char(Editor* editor, char ch);
void editor_delete_char(Editor* editor);
void editor_newline(Editor* editor);
void editor_backspace(Editor* editor);
void editor_move_cursor(Editor* editor, int row_delta, int col_delta);
int editor_run(Editor* editor);

#endif
