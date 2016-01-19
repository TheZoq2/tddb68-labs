#include <stdio.h>
#include <stdlib.h>

struct list_item 
{
    int value;
    struct list_item * next;
};

void append(struct list_item *first, int x) /* puts x at the end of the list */
{
    list_item* current = first;
    
    while(current->next != NULL)
    {
        current = current->next;
    }

    current->next = (list_item*) malloc(sizeof(list_item));

    current->next->value = x;
}

void prepend(struct list_item *first, int x) /* puts x at the beginning of the list */
{
}

void print(struct list_item *first) /* prints all elements in the list */
{
}

/* input_sorted: find the first element in the list larger than x
   and input x right before that element */
void input_sorted(struct list_item *first, int x)
{
}

void destroy(struct list_item *first) /* free everything dynamically allocated */
{
    
    
}

int main()
{
    struct list_item root;
    root.value = -1; /* This value is always ignored */
    root.next = NULL;

    return 0;
}
