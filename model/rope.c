#include "rope.h"

#include <string.h>
#include <stdlib.h>

ropeNode* create_leaf(char* str)
{
    ropeNode* node = calloc(1, sizeof(ropeNode));
    node->str = str;
    node->length = strlen(str);
    node->left = NULL;
    node->right = NULL;
    node->weight = node->length;
    return node;
}

ropeNode* rope_concat(ropeNode* left, ropeNode* right)
{

}
int rope_length(ropeNode* leaf)
{

}
char rope_get_char_at(ropeNode* node, int index)
{

}
void rope_split_leaf(ropeNode *node, int i, ropeNode **left, ropeNode **right)
{

}