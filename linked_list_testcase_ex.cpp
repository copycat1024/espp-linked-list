#include <gtest/gtest.h>
#include "linked_list.h"

// help macro
#define ASSERT_NO_ERROR(statement) ASSERT_EXIT((statement, exit(0)), ::testing::ExitedWithCode(0), ".*");
#define EXPECT_OUTPUT(statement, value)        \
    {                                          \
        char buf[100] = "", *output;           \
        int out_pipe[2];                       \
        int saved_stdout = dup(STDOUT_FILENO); \
        if (pipe(out_pipe) != 0)               \
        {                                      \
            FAIL() << "Can't create pipe";     \
        }                                      \
        dup2(out_pipe[1], STDOUT_FILENO);      \
        close(out_pipe[1]);                    \
        printf(" ");                           \
        statement;                             \
        fflush(stdout);                        \
        read(out_pipe[0], buf, 100);           \
        dup2(saved_stdout, STDOUT_FILENO);     \
        output = buf + 1;                      \
        EXPECT_STREQ(output, value);           \
    }

// global variable
char text[] = "Some text";
char *null_text = (char *)0;
linked_list *null_ll = (linked_list *)0;

// add_to_list
TEST(LinkedList, AddNullList)
{
    ASSERT_NO_ERROR(add_to_list(null_ll, text))

    int err = add_to_list(null_ll, text);

    EXPECT_EQ(err, -1);
}

TEST(LinkedList, AddOne)
{
    linked_list ll = {0};

    int err = add_to_list(&ll, text);

    linked_list *next = ll.next;

    EXPECT_EQ(err, 1);

    EXPECT_EQ(ll.data, null_text);
    EXPECT_EQ(ll.index, 0);
    ASSERT_NE(ll.next, null_ll);

    EXPECT_EQ(next->data, text);
    EXPECT_EQ(next->index, 1);
    EXPECT_EQ(next->next, null_ll);

    free(next);
}

TEST(LinkedList, AddTwo)
{
    linked_list ll = {0};
    linked_list ll_1 = {0};

    ll.next = &ll_1;
    ll_1.index = 1;

    int err = add_to_list(&ll, text);

    linked_list *next = ll_1.next;

    EXPECT_EQ(err, 2);

    EXPECT_EQ(ll.data, null_text);
    EXPECT_EQ(ll.index, 0);
    EXPECT_EQ(ll.next, &ll_1);

    EXPECT_EQ(ll_1.data, null_text);
    EXPECT_EQ(ll_1.index, 1);
    ASSERT_NE(ll_1.next, null_ll);

    EXPECT_EQ(next->data, text);
    EXPECT_EQ(next->index, 2);
    EXPECT_EQ(next->next, null_ll);

    free(next);
}

// display_item
TEST(LinkedList, DisplayItemNullItem)
{
    ASSERT_NO_ERROR(display_item(null_ll))

    int err = display_item(null_ll);

    EXPECT_EQ(err, -1);
}

TEST(LinkedList, DisplayItemNullString)
{
    char buffer[100];

    linked_list ll = {0};

    int err = display_item(&ll);

    EXPECT_EQ(err, 0);

    sprintf(buffer, "%s\n", NULL_REPLACE_STRING);
    EXPECT_OUTPUT(display_item(&ll), buffer);
}

TEST(LinkedList, DisplayItemOk)
{
    char buffer[100];

    linked_list ll = {0};
    ll.data = text;

    int err = display_item(&ll);

    EXPECT_EQ(err, 0);

    sprintf(buffer, "%s\n", text);
    EXPECT_OUTPUT(display_item(&ll), buffer);
}

// display_list
TEST(LinkedList, DisplayListNullList)
{
    ASSERT_NO_ERROR(display_list(null_ll))

    int err = display_list(null_ll);

    EXPECT_EQ(err, -1);
}

TEST(LinkedList, DisplayListOne)
{
    char buffer[100];

    linked_list ll = {0};
    ll.data = text;

    int err = display_list(&ll);

    EXPECT_EQ(err, 0);

    sprintf(buffer, "%s\n", text);
    EXPECT_OUTPUT(display_list(&ll), buffer);
}

TEST(LinkedList, DisplayListVarious)
{
    char buffer[100];

    linked_list ll = {0};
    linked_list ll_1 = {0};
    ll.data = text;
    ll.next = &ll_1;
    ll_1.index = 1;

    int err = display_list(&ll);

    EXPECT_EQ(err, 0);

    sprintf(buffer, "%s\n%s\n", text, NULL_REPLACE_STRING);
    EXPECT_OUTPUT(display_list(&ll), buffer);
}

// search_from_list
TEST(LinkedList, SearchNullList)
{
    ASSERT_NO_ERROR(search_from_list(null_ll, text));

    linked_list *ll = search_from_list(null_ll, text);

    EXPECT_EQ(ll, null_ll);
}

TEST(LinkedList, SearchNotFound)
{
    linked_list ll = {0};

    linked_list *node = search_from_list(&ll, text);

    EXPECT_EQ(ll.data, null_text);
    EXPECT_EQ(ll.index, 0);
    EXPECT_EQ(ll.next, null_ll);

    EXPECT_EQ(node, null_ll);
}

TEST(LinkedList, SearchFound)
{
    linked_list ll = {0};
    linked_list ll_1 = {0};
    ll.next = &ll_1;
    ll_1.data = text;
    ll_1.index = 1;

    linked_list *node = search_from_list(&ll, text);

    EXPECT_EQ(ll.data, null_text);
    EXPECT_EQ(ll.index, 0);
    EXPECT_EQ(ll.next, &ll_1);

    EXPECT_EQ(ll_1.data, text);
    EXPECT_EQ(ll_1.index, 1);
    EXPECT_EQ(ll_1.next, null_ll);

    EXPECT_EQ(node, &ll_1);
}

// delete_from_list
TEST(LinkedList, DeleteNullList)
{
    ASSERT_NO_ERROR(delete_from_list(null_ll, 0));

    int err = delete_from_list(null_ll, 0);

    EXPECT_EQ(err, -1);
}

TEST(LinkedList, DeleteOutOfIndex)
{
    linked_list ll = {0};

    int err = delete_from_list(&ll, 1);

    EXPECT_EQ(err, -1);

    EXPECT_EQ(ll.data, null_text);
    EXPECT_EQ(ll.index, 0);
    EXPECT_EQ(ll.next, null_ll);
}

TEST(LinkedList, DeleteOk)
{
    linked_list ll = {0};
    linked_list *ll_1 = (linked_list *)malloc(sizeof(linked_list));
    ll.next = ll_1;
    ll_1->index = 1;

    int err = delete_from_list(&ll, 1);

    EXPECT_EQ(err, 0);

    EXPECT_EQ(ll.data, null_text);
    EXPECT_EQ(ll.index, 0);
    EXPECT_EQ(ll.next, null_ll);
}

TEST(LinkedList, DeleteMany)
{
    linked_list ll = {0};
    linked_list *ll_1 = (linked_list *)malloc(sizeof(linked_list));
    ll.next = ll_1;
    ll_1->index = 1;

    int err = delete_from_list(&ll, 1);

    EXPECT_EQ(err, 0);

    EXPECT_EQ(ll.data, null_text);
    EXPECT_EQ(ll.index, 0);
    EXPECT_EQ(ll.next, null_ll);
}
