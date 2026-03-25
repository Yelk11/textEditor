#include "ui.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    Editor* editor = editor_create();
    if (!editor) {
        fprintf(stderr, "Failed to create editor\n");
        return 1;
    }
    
    int result = editor_run(editor);
    
    editor_free(editor);
    
    return result;
}
