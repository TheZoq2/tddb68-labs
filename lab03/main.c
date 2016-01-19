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
    current->next->next = NULL;
}

void prepend(struct list_item *first, int x) /* puts x at the beginning of the list */
{
    first->next = root->next;
    root->next = first;
}

void print(struct list_item *first) /* prints all elements in the list */
{
}

/* 
 * input_sorted: find the first element in the list larger than x
 * and input x right before that element 
*/
void input_sorted(struct list_item *first, int x)
{
    list_item* current = first;

    while(current->next != NULL)
    {
        if(current->next->value > x)
        {
            list_item* oldNext = current->next;

            current->next = (list_item*) malloc(sizeof(list_item));

            current->next->value = x;

            current->next->next = oldNext;
        }
    }
}

void destroy(struct list_item *first) /* free everything dynamically allocated */
{
    list_item* current = first->next;

    while(current != NULL)
    {
        list_item* next = current->next;
        
        free(current);
        current = next;
    }

    first->next = NULL;
}

int main()
{
    struct list_item root;
    root.value = -1; /* This value is always ignored */
    root.next = NULL;


    append(&root, 5);
    append(&root, 3);
    append(&root, 10);
    append(&root, -1);

    print(&root);

    prepend(&root, -2);
    prepend(&root, -3);
    prepend(&root, -4);

    print(&root);

    destroy(&root);

    input_sorted(&root, 3);
    input_sorted(&root, 3);
    input_sorted(&root, 6);
    input_sorted(&root, 2);
    input_sorted(&root, 0);

    return 0;
}
