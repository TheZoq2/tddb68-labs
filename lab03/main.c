#include <stdio.h>
#include <stdlib.h>

struct list_item 
{
    int value;
    struct list_item * next;
};

void append(struct list_item *first, int x) /* puts x at the end of the list */
{
    struct list_item* current = first;
    
    while(current->next != NULL)
    {
        current = current->next;
    }

    current->next = (struct list_item*) malloc(sizeof(struct list_item));

    current->next->value = x;
}

void prepend(struct list_item *first, int x) /* puts x at the beginning of the list */
{
    struct list_item* new_elem = malloc(sizeof(struct list_item));
    new_elem->next = first->next;
    first->next = new_elem;
}

void print(struct list_item *first) /* prints all elements in the list */
{
    struct list_item* current = first->next;
    while (current != NULL) {
        printf("%d ", current->value);
    }
    printf("\n");
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
