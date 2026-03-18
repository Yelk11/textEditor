

typedef struct ropeNode{
    struct ropeNode* left;
    struct ropeNode* right;
    int weight; // length of left node only
    int length; // weight of right and left nodes (total length of string)
    char* str;
}ropeNode;

ropeNode* create_leaf(char* str);

ropeNode* create_node(ropeNode* left, ropeNode* right);

int rope_length(ropeNode* root);

ropeNode* rope_concat(ropeNode* left, ropeNode* right);

ropeNode* rope_insert(int index, char* str);

ropeNode* rope_balance(ropeNode* root);

ropeNode* rope_substring(ropeNode* root, int start, int end);



char* rope_to_string(ropeNode* root);

void rope_print(ropeNode* root);

void rope_print_tree(ropeNode* root);

void rope_free(ropeNode* root);



