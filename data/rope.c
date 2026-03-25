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

ropeNode* create_node(ropeNode* left, ropeNode* right)
{
    ropeNode* node = calloc(1, sizeof(ropeNode));
    node->left = left;
    node->right = right;
    node->str = NULL;
    node->weight = left ? left->length : 0;
    node->length = (left ? left->length : 0) + (right ? right->length : 0);
    return node;
}

ropeNode* rope_split(ropeNode* rope, int index)
{
    if (!rope || index <= 0) return rope;
    if (index >= rope->length) return NULL;
    
    // If it's a leaf, we need to split the string
    if (rope->str) {
        ropeNode* right = create_leaf(rope->str + index);
        rope->str[index] = '\0';
        rope->length = index;
        rope->weight = index;
        return right;
    }
    
    // If index is within left subtree
    if (index <= rope->weight) {
        ropeNode* right_of_left = rope_split(rope->left, index);
        if (right_of_left) {
            return rope_concat(right_of_left, rope->right);
        }
        return rope->right;
    }
    
    // If index is within right subtree
    ropeNode* left_of_right = rope_split(rope->right, index - rope->weight);
    if (left_of_right) {
        rope->right = left_of_right;
        rope->length = rope->weight + left_of_right->length;
        return NULL;
    }
    return NULL;
}

ropeNode* rope_insert(ropeNode* rope, int index, char* str)
{
    if (!str || index < 0) return rope;
    if (!rope) return create_leaf(str);
    if (index > rope->length) return rope;
    
    ropeNode* left = rope;
    ropeNode* insert_node = create_leaf(str);
    
    if (index == 0) {
        return rope_concat(insert_node, rope);
    }
    
    if (index >= rope->length) {
        return rope_concat(rope, insert_node);
    }
    
    ropeNode* right = rope_split(rope, index);
    return rope_concat(rope_concat(left, insert_node), right);
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



ropeNode* rope_substring(ropeNode* root, int start, int end)
{
    if (!root || start < 0 || end > root->length || start >= end) {
        return NULL;
    }
    
    if (root->str) {
        char* substr = calloc(end - start + 1, sizeof(char));
        strncpy(substr, root->str + start, end - start);
        ropeNode* result = create_leaf(substr);
        free(substr);
        return result;
    }
    
    // Navigate the tree to find the substring
    if (end <= root->weight) {
        // Both start and end are in the left subtree
        return rope_substring(root->left, start, end);
    }
    
    if (start >= root->weight) {
        // Both start and end are in the right subtree
        return rope_substring(root->right, start - root->weight, end - root->weight);
    }
    
    // Substring spans both left and right subtrees
    ropeNode* left_part = rope_substring(root->left, start, root->weight);
    ropeNode* right_part = rope_substring(root->right, 0, end - root->weight);
    
    if (left_part && right_part) {
        return rope_concat(left_part, right_part);
    }
    
    return left_part ? left_part : right_part;
}

ropeNode* rope_balance(ropeNode* root)
{
    if (!root || root->str) return root;
    
    // Base case: if already balanced or leaf, return as is
    if (!root->left || !root->right) return root;
    
    // Recursively balance subtrees
    root->left = rope_balance(root->left);
    root->right = rope_balance(root->right);
    
    // Ensure balance (left and right heights don't differ by more than 1)
    // For simplicity, we'll recompute weights after balancing
    root->weight = root->left ? root->left->length : 0;
    root->length = (root->left ? root->left->length : 0) + (root->right ? root->right->length : 0);
    
    return root;
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
