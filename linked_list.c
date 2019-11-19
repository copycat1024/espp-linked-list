#include "linked_list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const char null_rep[] = NULL_REPLACE_STRING;

int my_cmp(const char *a, const char *b);

int add_to_list(linked_list *ll, char *s)
{
  if (!ll)
    return -1;

  while (ll->next)
    ll = ll->next;

  linked_list *next = (struct linked_list *)malloc(sizeof(linked_list));

  ll->next = next;
  next->index = ll->index + 1;
  next->data = s;
  next->next = (linked_list *)0;

  return next->index;
}

int display_item(linked_list *ll)
{
  if (!ll)
    return -1;

  const char *str = ll->data ? ll->data : null_rep;
  printf("%s\n", str);

  return 0;
}

int display_list(linked_list *ll)
{
  if (!ll)
    return -1;

  do
  {
    display_item(ll);
    ll = ll->next;
  } while (ll);

  return 0;
}

linked_list *search_from_list(linked_list *ll, char *s)
{
  if (!ll)
    return (linked_list *)0;

  do
  {
    if (my_cmp(s, ll->data) == 0)
      break;

    ll = ll->next;
  } while (ll);

  return ll;
}

int delete_from_list(linked_list *ll, int index)
{

  if (!ll)
    return -1;

  do
  {
    if (ll->next && ll->next->index == index)
      break;

    ll = ll->next;
  } while (ll);

  if (!ll) // out of index
    return -1;

  linked_list *node = ll->next;
  ll->next = node->next;
  free(node);

  return 0;
}

int empty_list(linked_list *ll)
{
  return 0;
}

int swap_items(linked_list *f, linked_list *s)
{
  return 0;
}

int sort_list(linked_list *ll)
{
  return 0;
}

int linkedlist_status(linked_list *ll)
{
  return 0;
}

int my_cmp(const char *a, const char *b)
{
  if (a && b)
    return strcmp(a, b);

  if (a)
    return 1;
  else
    return b ? -1 : 0;
}
