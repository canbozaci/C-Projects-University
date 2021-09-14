#include <stdio.h>
#include <stdlib.h>

typedef struct id_ll
{
    int value;
    struct id_ll *next;
}id_ll_t;

typedef struct sch_no_stack
{
    int id;
    id_ll_t *head_of_ll;
    struct sch_no_stack *prev;
}sch_no_stack_t;

void init_link_list(id_ll_t *, int);
void insert_to_link_list(id_ll_t *, int);
void init_stack(sch_no_stack_t **, id_ll_t *);
void push_to_stack(sch_no_stack_t **, id_ll_t *);
void pop_from_stack(sch_no_stack_t **, id_ll_t **);
void print_all_to_screen(sch_no_stack_t **, sch_no_stack_t **);
void find_minimum_acc_id(sch_no_stack_t **);
void find_minimum_acc_sch_no(sch_no_stack_t **);

int main(void)
{
    id_ll_t *id_head_node = NULL;
    sch_no_stack_t *sch_last_node = NULL, *backup_stack = NULL;
    long int user_input;

    printf("Please enter -1 to terminate taking input from user!\n");

    while(1)
    {
        scanf("%ld",&user_input);

        if(user_input == -1)
        {
            printf("Taking input is terminated\n");
            break;
        }
        else
        {

            id_head_node = (id_ll_t *)malloc(sizeof(id_ll_t));
            init_link_list(id_head_node,user_input);

            if(sch_last_node == NULL)
            {
                init_stack(&sch_last_node, id_head_node);
            }
            else
            {
                push_to_stack(&sch_last_node, id_head_node);
            }

            id_head_node = NULL;
        }
    }

    print_all_to_screen(&sch_last_node,&backup_stack);

    sch_last_node = backup_stack;

    find_minimum_acc_id(&sch_last_node);

    return 0;
}

void init_link_list(id_ll_t *head_node, int value)
{
    long int divider = 100000000;
    long int result;
    if(head_node == NULL)
    {
        return; 
    }
    else
    {
        head_node->next = NULL;
        head_node->value = value % 1000;
        for(; divider > 0; divider /= 10)
        {
            insert_to_link_list(head_node, value / divider);
            value %= divider;
        }
            
    }
}

void insert_to_link_list(id_ll_t *head_node, int value)
{
    id_ll_t *last_node = NULL;

    if(head_node == NULL)
    {
        return;
    }
    else
    {
        for(last_node = head_node; last_node->next != NULL; last_node = last_node->next);

        last_node->next = (id_ll_t *)malloc(sizeof(id_ll_t));
        last_node->next->value = value;
        last_node->next->next = NULL;
    }
}

void init_stack(sch_no_stack_t **last_node, id_ll_t *id_list)
{
    if(last_node == NULL)
    {
        return;
    }
    else
    {
        (*last_node) = (sch_no_stack_t *)malloc(sizeof(sch_no_stack_t));
        (*last_node)->id = id_list->value;
        (*last_node)->head_of_ll = id_list;
        (*last_node)->prev = NULL;
    }
}

void push_to_stack(sch_no_stack_t **last_node, id_ll_t *id_list)
{
    sch_no_stack_t *new_node = NULL;

    if(last_node == NULL || *last_node == NULL)
    {
        return;
    }
    else
    {
        new_node = (sch_no_stack_t *)malloc(sizeof(sch_no_stack_t));
        new_node->id = id_list->value;
        new_node->head_of_ll = id_list;
        new_node->prev = *last_node;
        *last_node = new_node;
    }
}

void pop_from_stack(sch_no_stack_t **last_node, id_ll_t **id_list)
{

    if(last_node == NULL || *last_node == NULL)
    {
        return;
    }
    else
    {
        *id_list = (*last_node)->head_of_ll;
        (*last_node) = (*last_node)->prev;
    }
}

void print_all_to_screen(sch_no_stack_t **last_node, sch_no_stack_t **stack_backup)
{
    id_ll_t *id_head_node, *next_node;

    while(1)
    {
        id_head_node = NULL;

        pop_from_stack(last_node, &id_head_node);
        
        if(id_head_node == NULL)
        {
            return;
        }
        
        if(*stack_backup == NULL)
        {
            init_stack(stack_backup, id_head_node);
        }
        else
        {
            push_to_stack(stack_backup, id_head_node);
        }
        
        printf("school number of student with ID: %03d = ", id_head_node->value);

        for(next_node = id_head_node->next; next_node != NULL; next_node = next_node->next)
        {
            printf("%d",next_node->value);
        }

        printf("\n");
    }
}

void find_minimum_acc_id(sch_no_stack_t **last_node)
{
    int min_id = -1;
    id_ll_t *id_head_node, *min_id_node = NULL;

    while(1)
    {
        id_head_node = NULL;

        pop_from_stack(last_node, &id_head_node);

        if(id_head_node == NULL)
        {
            break;
        }

        if((min_id == -1) || (min_id > id_head_node->value))
        {
            min_id = id_head_node->value;
            min_id_node = id_head_node;
        }
    }

    printf("school number of student with minimum ID: %03d = ", min_id);

    for(id_head_node = min_id_node->next; id_head_node != NULL; id_head_node = id_head_node->next)
    {
        printf("%d",id_head_node->value);
    }

    printf("\n");
}

void find_minimum_acc_sch_no(sch_no_stack_t **last_node)
{
    
}