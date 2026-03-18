#include "rope.h"

#include <stdio.h>

int main(void)
{
    // Test rope_print
    ropeNode* leaf1 = create_leaf("Hello ");
    ropeNode* leaf2 = create_leaf("World!");
    ropeNode* leaf3 = create_leaf("This ");
    ropeNode* leaf4 = create_leaf("is ");
    ropeNode* leaf5 = create_leaf("me");
    ropeNode* leaf6 = create_leaf(".");
    ropeNode* node1 = rope_concat(leaf1, leaf2);
    ropeNode* node2 = rope_concat(leaf3, leaf4);
    ropeNode* node3 = rope_concat(leaf5, leaf6);
    ropeNode* node4 = rope_concat(node1, node2);
    ropeNode* node5 = rope_concat(node4, node3);

    
    printf("Rope content: ");
    rope_print(node5);
    printf("\n\n");
    
    printf("Rope tree structure:\n");
    rope_print_tree(node5);
    
    rope_free(node5);
    
    return 0;
}
