#include "ui.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STATUS_HEIGHT 2
#define TAB_WIDTH 4

Editor* editor_create(void)
{
    Editor* editor = calloc(1, sizeof(Editor));
    if (!editor) return NULL;
    
    editor->content = create_leaf("");
    editor->cursor_row = 0;
    editor->cursor_col = 0;
    editor->view_offset_row = 0;
    editor->view_offset_col = 0;
    editor->modified = 0;
    
    return editor;
}

void editor_free(Editor* editor)
{
    if (!editor) return;
    if (editor->content) rope_free(editor->content);
    free(editor);
}

// Get character at position in rope
char editor_get_char_at(Editor* editor, int pos)
{
    if (pos < 0 || pos >= rope_length(editor->content)) return '\0';
    
    char* str = rope_to_string(editor->content);
    if (!str) return '\0';
    
    char ch = str[pos];
    free(str);
    return ch;
}

// Convert 2D cursor position to linear index in rope
int editor_cursor_to_index(Editor* editor)
{
    char* str = rope_to_string(editor->content);
    if (!str) return 0;
    
    int index = 0;
    int row = 0;
    int col = 0;
    
    while (str[index] && (row < editor->cursor_row || (row == editor->cursor_row && col < editor->cursor_col))) {
        if (str[index] == '\n') {
            row++;
            col = 0;
        } else {
            col++;
        }
        index++;
    }
    
    free(str);
    return index;
}

// Convert linear index to 2D cursor position
void editor_index_to_cursor(Editor* editor, int index)
{
    char* str = rope_to_string(editor->content);
    if (!str) return;
    
    int row = 0;
    int col = 0;
    
    for (int i = 0; i < index && str[i]; i++) {
        if (str[i] == '\n') {
            row++;
            col = 0;
        } else {
            col++;
        }
    }
    
    editor->cursor_row = row;
    editor->cursor_col = col;
    free(str);
}

void editor_insert_char(Editor* editor, char ch)
{
    int index = editor_cursor_to_index(editor);
    
    if (ch == '\n') {
        editor->content = rope_insert(editor->content, index, "\n");
        editor->cursor_row++;
        editor->cursor_col = 0;
    } else {
        char buf[2] = {ch, '\0'};
        editor->content = rope_insert(editor->content, index, buf);
        editor->cursor_col++;
    }
    
    editor->modified = 1;
}

void editor_backspace(Editor* editor)
{
    if (editor->cursor_col == 0 && editor->cursor_row == 0) return;
    
    int index = editor_cursor_to_index(editor);
    if (index <= 0) return;
    
    // Delete character before cursor
    ropeNode* left = rope_substring(editor->content, 0, index - 1);
    ropeNode* right = rope_substring(editor->content, index, rope_length(editor->content));
    
    rope_free(editor->content);
    if (left && right) {
        editor->content = rope_concat(left, right);
    } else if (left) {
        editor->content = left;
    } else if (right) {
        editor->content = right;
    } else {
        editor->content = create_leaf("");
    }
    
    // Move cursor back
    if (editor->cursor_col > 0) {
        editor->cursor_col--;
    } else if (editor->cursor_row > 0) {
        editor->cursor_row--;
        // Find column at end of previous line
        char* str = rope_to_string(editor->content);
        if (str) {
            int idx = editor_cursor_to_index(editor);
            int col = 0;
            while (idx > 0 && str[idx - 1] != '\n') {
                col++;
                idx--;
            }
            editor->cursor_col = col;
            free(str);
        }
    }
    
    editor->modified = 1;
}

void editor_delete_char(Editor* editor)
{
    int index = editor_cursor_to_index(editor);
    int len = rope_length(editor->content);
    
    if (index >= len) return;
    
    ropeNode* left = rope_substring(editor->content, 0, index);
    ropeNode* right = rope_substring(editor->content, index + 1, len);
    
    rope_free(editor->content);
    if (left && right) {
        editor->content = rope_concat(left, right);
    } else if (left) {
        editor->content = left;
    } else if (right) {
        editor->content = right;
    } else {
        editor->content = create_leaf("");
    }
    
    editor->modified = 1;
}

void editor_move_cursor(Editor* editor, int row_delta, int col_delta)
{
    int new_row = editor->cursor_row + row_delta;
    int new_col = editor->cursor_col + col_delta;
    
    // Clamp to valid bounds
    if (new_row < 0) new_row = 0;
    if (new_col < 0) new_col = 0;
    
    char* str = rope_to_string(editor->content);
    if (!str) return;
    
    // Validate row and find max column for that row
    int current_row = 0;
    int line_start = 0;
    int line_end = 0;
    
    for (int i = 0; str[i]; i++) {
        if (current_row == new_row) {
            if (str[i] == '\n' || str[i] == '\0') {
                line_end = i;
                break;
            }
        }
        if (str[i] == '\n') {
            current_row++;
            if (current_row > new_row) break;
            line_start = i + 1;
        }
    }
    
    int max_col = line_end - line_start;
    if (new_col > max_col) new_col = max_col;
    
    editor->cursor_row = new_row;
    editor->cursor_col = new_col;
    
    free(str);
}

void editor_render(Editor* editor)
{
    clear();
    
    char* text = rope_to_string(editor->content);
    if (!text) text = calloc(1, 1);
    
    // Render text content
    int row = 0;
    int col = 0;
    int text_idx = 0;
    
    for (int screen_row = 0; screen_row < editor->max_rows && text[text_idx]; screen_row++) {
        col = 0;
        
        while (col < editor->max_cols && text[text_idx] && text[text_idx] != '\n') {
            if (row == editor->cursor_row && col == editor->cursor_col) {
                attron(A_REVERSE);
                mvaddch(screen_row, col, text[text_idx] ? text[text_idx] : ' ');
                attroff(A_REVERSE);
            } else {
                mvaddch(screen_row, col, text[text_idx]);
            }
            
            col++;
            text_idx++;
        }
        
        // Render cursor at end of line if needed
        if (row == editor->cursor_row && col == editor->cursor_col) {
            attron(A_REVERSE);
            mvaddch(screen_row, col, ' ');
            attroff(A_REVERSE);
        }
        
        // Skip newline
        if (text[text_idx] == '\n') {
            text_idx++;
            row++;
        }
    }
    
    // Render status bar
    int status_row = editor->max_rows;
    attron(A_REVERSE);
    mvhline(status_row, 0, ' ', editor->max_cols);
    
    char status[256];
    snprintf(status, sizeof(status), " Line: %d | Col: %d | Length: %d | %s",
             editor->cursor_row + 1,
             editor->cursor_col + 1,
             rope_length(editor->content),
             editor->modified ? "[Modified]" : "");
    
    mvprintw(status_row, 0, "%s", status);
    attroff(A_REVERSE);
    
    // Render help line
    mvprintw(status_row + 1, 0, "^X=Exit  ^S=Save");
    
    // Clamp cursor to visible area
    if (editor->cursor_row < editor->view_offset_row) {
        editor->view_offset_row = editor->cursor_row;
    }
    if (editor->cursor_col < editor->view_offset_col) {
        editor->view_offset_col = editor->cursor_col;
    }
    
    refresh();
    
    if (text) free(text);
}

void editor_handle_input(Editor* editor, int ch)
{
    if (ch == ERR) return;
    
    // Handle special keys
    if (ch == KEY_UP) {
        editor_move_cursor(editor, -1, 0);
    } else if (ch == KEY_DOWN) {
        editor_move_cursor(editor, 1, 0);
    } else if (ch == KEY_LEFT) {
        editor_move_cursor(editor, 0, -1);
    } else if (ch == KEY_RIGHT) {
        editor_move_cursor(editor, 0, 1);
    } else if (ch == KEY_BACKSPACE || ch == 127) {
        editor_backspace(editor);
    } else if (ch == KEY_DC) {
        editor_delete_char(editor);
    } else if (ch == '\n') {
        editor_insert_char(editor, '\n');
    } else if (ch == '\t') {
        for (int i = 0; i < TAB_WIDTH; i++) {
            editor_insert_char(editor, ' ');
        }
    } else if (isprint(ch)) {
        editor_insert_char(editor, (char)ch);
    }
}

int editor_run(Editor* editor)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, FALSE);
    
    getmaxyx(stdscr, editor->max_rows, editor->max_cols);
    editor->max_rows -= STATUS_HEIGHT;
    
    int running = 1;
    
    while (running) {
        editor_render(editor);
        
        int ch = getch();
        
        if (ch == 'q') {
            // Exit editor
            running = 0;
        } else {
            editor_handle_input(editor, ch);
        }
    }
    
    endwin();
    return 0;
}
