

typedef struct ropeNode{
    struct ropeNode* left;
    struct ropeNode* right;
    int weight;
    char* str;
    int length;
}ropeNode;

ropeNode* create_leaf(char* str);
ropeNode* rope_concat(ropeNode* left, ropeNode* right);
int rope_length(ropeNode* leaf);
char rope_get_char_at(ropeNode* node, int index);
void rope_split_leaf(ropeNode *node, int i, ropeNode **left, ropeNode **right);
ropeNode* rope_insert(ropeNode *node, int pos, const char *text);
ropeNode* rope_delete(ropeNode *node, int pos, const char *text);
void rope_print(ropeNode* node);
