#include "unity.h"
#include "rope.h"
#include <stdlib.h>

void test_rope_insert(void)
{

}

void test_rope_length(void)
{
    ropeNode* leaf = create_leaf("abc");
    TEST_ASSERT_EQUAL_INT(3, rope_length(leaf));
    rope_free(leaf);
}

void test_rope_concat_and_to_string(void)
{
    ropeNode* a = create_leaf("A");
    ropeNode* b = create_leaf("B");
    ropeNode* c = rope_concat(a, b);

    TEST_ASSERT_EQUAL_INT(2, rope_length(c));

    char* s = rope_to_string(c);
    TEST_ASSERT_EQUAL_STRING("AB", s);
    free(s);
    rope_free(c);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_rope_insert);
    RUN_TEST(test_rope_length);
    RUN_TEST(test_rope_concat_and_to_string);
    return UNITY_END();
}
