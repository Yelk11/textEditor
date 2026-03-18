#include "rope.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


ropeNode* create_leaf(char* str)
{
    ropeNode* node = calloc(1, sizeof(ropeNode));
    node->str = strdup(str);
    node->left = NULL;
    node->right = NULL;
    node->weight = strlen(str);
    node->length = node->weight;
    return node;
}

ropeNode* rope_insert(int index, char* str)
{
    return NULL;
}

ropeNode* rope_concat(ropeNode* left, ropeNode* right)
{
    ropeNode* root = calloc(1, sizeof(ropeNode));
    root->left = left;
    root->right = right;
    root->str = NULL;
    root->weight = left->length;
    root->length = left->length + right->length;
    return root;
}

int rope_length(ropeNode* root)
{
    return root ? root->length : 0;
}

char* rope_to_string(ropeNode* root)
{
    if (!root) return NULL;

    char* buffer = calloc(root->length + 1, 1);
    if (!buffer) return NULL;

    // simple in-order write
    if (root->str) {
        memcpy(buffer, root->str, root->length);
        buffer[root->length] = '\0';
        return buffer;
    }

    char* left_str = rope_to_string(root->left);
    char* right_str = rope_to_string(root->right);

    size_t left_len = left_str ? strlen(left_str) : 0;
    size_t right_len = right_str ? strlen(right_str) : 0;

    if (left_str) {
        memcpy(buffer, left_str, left_len);
        free(left_str);
    }
    if (right_str) {
        memcpy(buffer + left_len, right_str, right_len);
        free(right_str);
    }

    buffer[root->length] = '\0';
    return buffer;
}

void rope_print(ropeNode* root)
{
    if (root == NULL) return;
    if (root->str != NULL) {
        printf("%s", root->str);
    } else {
        rope_print(root->left);
        rope_print(root->right);
    }
}

void rope_print_tree_helper(ropeNode* root, int depth)
{
    if (root == NULL) return;
    for(int i = 0; i < depth; i++) printf("  ");
    if (root->str != NULL) {
        printf("Leaf: weight=%d, length=%d, str=\"%s\"\n", root->weight, root->length, root->str);
    } else {
        printf("Node: weight=%d, length=%d\n", root->weight, root->length);
        rope_print_tree_helper(root->left, depth + 1);
        rope_print_tree_helper(root->right, depth + 1);
    }
}

void rope_print_tree(ropeNode* root)
{
    rope_print_tree_helper(root, 0);
}

void rope_free(ropeNode* root)
{
    if (root == NULL)
        return;

    rope_free(root->left);
    rope_free(root->right);
    free(root->str);
    free(root);
}
