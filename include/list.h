#ifndef LIST_HEADER
#define LIST_HEADER


#include <stddef.h>


#include <kernel.h>





/*
 * Simple Circular doubly linked lists implementation.
 */





struct list_head
{
    struct list_head *next, *prev;
};





#define INIT_LIST_HEAD(name) { &(name), &(name) }

#define DECLARE_LIST_HEAD(name) \
    struct list_head name = INIT_LIST_HEAD(name)



static inline void list_init_head(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}



/*
 * list_empty - tests whether a list is empty
 * head: the list to test.
 */
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}



/*
 * list_is_first - tests whether node is the first node in list head
 * node: the node to test
 * head: the head of the list
 *
 *  ret: 1   // if [...] <-> [head] <-> [node] <-> [...]
 *  ret: 0   // else
 */
static inline int list_is_first(const struct list_head *node,
                                const struct list_head *head)
{
    return node->prev == head;
}



/*
 * list_is_last - tests whether node is the last node in list head
 * node: the node to test
 * head: the head of the list
 *
 *  ret: 1   // if [...] <-> [node] <-> [head] <-> [...]
 *  ret: 0   // else
 */
static inline int list_is_last(const struct list_head *node,
                               const struct list_head *head)
{
    return node->next == head;
}



/*
 * list_is_singular - tests whether a list has just one node.
 * head: the list to test.
 */
static inline int list_is_singular(const struct list_head *head)
{
    return !list_empty(head) && (head->next == head->prev);
}



/*
 * list_size - Returns the number of elements in the list container.
 * head: the list to test.
 */
static inline size_t list_size(const struct list_head *head)
{
    size_t size = 0;
    const struct list_head *it;
    for(it = (head)->next; it != (head); it = it->next)
        size++;

    return size;
}



/*
 * Insert a new node between two known consecutive nodes.
 *
 * This is only for internal list manipulation where we know
 * the prev/next nodes already!
 *
 * before:  [prev] <-> [next]
 * after:   [prev] <-> [node] <-> [next]
 *
 */
static inline void sys_list_add(struct list_head *node,
                                struct list_head *prev,
                                struct list_head *next)
{
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}



/*
 * list_push_front - add a new node
 * node: new node to be added
 * head: list head to add it after
 *
 * Inserts a new node at the beginning of the list,
 * before its current first element.
 *
 * before:  [prev] <-> [head] <-> [next]
 * after:   [prev] <-> [head] <-> [node] <-> [next]
 *
 */
static inline void list_push_front(struct list_head *node, struct list_head *head)
{
    sys_list_add(node, head, head->next);
}



/*
 * list_push_back - add a new node
 * node: new node to be added
 * head: list head to add it before
 *
 * Insert a new node at the end of the list,
 * after its current last element.
 *
 * before:  [prev] <-> [head] <-> [next]
 * after:   [prev] <-> [node] <-> [head] <-> [next]
 */
static inline void list_push_back(struct list_head *node, struct list_head *head)
{
    sys_list_add(node, head->prev, head);
}



/*
 * Delete a list node by making the prev/next nodes
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next nodes already!
 */
static inline void sys_list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}



/*
 * sys_list_del_node - deletes node from list.
 * node: the element to delete from the list.
 * Note: list_empty() on node does not return true after this,
 * the node is in an undefined state.
 *
 * before:  [prev] <-> [node] <-> [next]
 * after:   [prev] <-> [next];              old_val <- [node] -> old_val
 */
static inline void sys_list_del_node(struct list_head *node)
{
    sys_list_del(node->prev, node->next);
}



/*
 * list_del - deletes node from list.
 * node: the element to delete from the list.
 * Note: list_empty() on node does return true after this
 *
 * before:  [prev] <-> [node] <-> [next]
 * after:   [prev] <-> [next];              self <- [node] -> self
 */
static inline void list_del(struct list_head *node)
{
    sys_list_del(node->prev, node->next);
    list_init_head(node);
}



/*
 * list_replace - replace old node by new node
 * old_node: the element to be replaced
 * new_node: the new element to insert
 *
 * If old_node was empty, it will be overwritten.
 */
static inline void list_replace(struct list_head *old_node,
                                struct list_head *new_node)
{
    new_node->next       = old_node->next;
    new_node->next->prev = new_node;
    new_node->prev       = old_node->prev;
    new_node->prev->next = new_node;
}

static inline void list_replace_init(struct list_head *old_node,
                                     struct list_head *new_node)
{
    list_replace(old_node, new_node);
    list_init_head(old_node);
}



/*
 * list_move_to_front - delete from one list and add as another's head
 * node: the node to move
 * head: the head that will precede our node
 */
static inline void list_move_to_front(struct list_head *node,
                                      struct list_head *head)
{
    sys_list_del_node(node);
    list_push_front(node, head);
}



/*
 * list_move_to_back - delete from one list and add as another's tail
 * node: the node to move
 * head: the head that will follow our node
 */
static inline void list_move_to_back(struct list_head *node,
                                     struct list_head *head)
{
    sys_list_del_node(node);
    list_push_back(node, head);
}



/*
 * list_rotate_left - rotate the list to the left
 * head: the head of the list
 *
 * before:  [...] <-> [nodeN] <-> [head]  <-> [node1] <-> [node2]  <-> [...]
 * after:   [...] <-> [nodeN] <-> [node1] <-> [head]  <-> [node2]  <-> [...]
 */
static inline void list_rotate_left(struct list_head *head)
{
    struct list_head *first;

    if( !list_empty(head) )
    {
        first = head->next;
        list_move_to_back(first, head);
    }
}



/*
 * list_rotate_right - rotate the list to the right
 * head: the head of the list
 *
 * before:  [...] <-> [nodeN] <-> [head]  <-> [node1] <-> [node2] <-> [...]
 * after:   [...] <-> [head]  <-> [nodeN] <-> [node1] <-> [node2] <-> [...]
 */
static inline void list_rotate_right(struct list_head *head)
{
    struct list_head *last;

    if( !list_empty(head) )
    {
        last = head->prev;
        list_move_to_front(last, head);
    }
}





//---------------- Get Data from node ----------------





/*
 * list_data - get the struct (data) for this node
 * node:   the node.
 * type:   the type of the struct of data this is embedded in.
 * member: the name of the node(list_head) within the struct of data.
 */
#define list_data(node, type, member) \
    container_of(node, type, member)



/*
 * list_data_or_null - get the struct (data) for this node
 * node:   the node.
 * type:   the type of the struct of data this is embedded in.
 * member: the name of the node(list_head) within the struct of data.
 *
 * Note it returns NULL if the list(node) is empty.
 */
#define list_data_or_null(node, type, member) \
    (!list_empty(node) ? list_data(node, type, member) : NULL)



/*
 * list_first_data - get the first struct (data) from a list
 * head:   the list head to take the element from.
 * type:   the type of the struct of data this is embedded in.
 * member: the name of the node(list_head) within the struct of data.
 *
 * Note - list don't have to  be empty.
 */
#define list_first_data(head, type, member) \
    list_data((head)->next, type, member)



/*
 * list_last_data - get the last struct (data) from a list
 * head:   the list head to take the element from.
 * type:   the type of the struct of data this is embedded in.
 * member: the name of the node(list_head) within the struct of data.
 *
 * Note - list don't have to  be empty.
 */
#define list_last_data(head, type, member) \
    list_data((head)->prev, type, member)



/*
 * list_first_data_or_null - get the first struct (data) from a list
 * head:   the list head to take the element from.
 * type:   the type of the struct of data this is embedded in.
 * member: the name of the node(list_head) within the struct of data.
 *
 * Note it returns NULL if the list(node) is empty.
 */
#define list_first_data_or_null(head, type, member) \
    (!list_empty(head) ? list_first_data(head, type, member) : NULL)



/*
 * list_last_data_or_null - get the last struct (data) from a list
 * head:   the list head to take the element from.
 * type:   the type of the struct of data this is embedded in.
 * member: the name of the node(list_head) within the struct of data.
 *
 * Note it returns NULL if the list(node) is empty.
 */
#define list_last_data_or_null(head, type, member) \
    (!list_empty(head) ? list_last_data(head, type, member) : NULL)





//---------------- Iterator ----------------





/*
 * list_citer - constant iterate over a list
 * it:    the &struct list_head to use as a loop cursor(iterator).
 * head:  the head for your list.
 *
 * Note You do not have to change list in this cycle.
 *
 */
#define list_citer(it, head) \
    for( it = (head)->next; it != (head); it = it->next )



/*
 * list_criter - constant revers iterate over a list
 * it:    the &struct list_head to use as a loop cursor(iterator).
 * head:  the head for your list.
 *
 * Note You do not have to change list in this cycle.
 */
#define list_criter(it, head) \
    for( it = (head)->prev; it != (head); it = it->prev )



/*
 * list_iter - iterate over a list safe against removal of list node
 * it:     the &struct list_head to use as a loop cursor(iterator).
 * tmp_it: another &struct list_head to use as temporary cursor(iterator)
 * head:   the head for your list.
 */
#define list_iter(it, tmp_it, head)                          \
    for( it = (head)->next, tmp_it = it->next; it != (head); \
         it = tmp_it, tmp_it = it->next)



/*
 * list_riter - reverse iterate over a list safe against removal of list node
 * it:     the &struct list_head to use as a loop cursor(iterator).
 * tmp_it: another &struct list_head to use as temporary cursor(iterator)
 * head:   the head for your list.
 */
#define list_riter(it, tmp_it, head)           \
    for(it  = (head)->prev, tmp_it = it->prev; \
        it != (head);                          \
        it  = tmp_it, tmp_it = it->prev)



/*
 * list_data_citer - constant iterate over list of given type (data)
 * it:     the &struct data to use as a loop cursor(iterator).
 * head:   the &list_head to take the element from.
 * type:   the type of the struct of data this is embedded in.
 * member: the name of the node(list_head) within the struct of data.
 *
 * Note You do not have to change list in this cycle.
 */
#define list_data_citer(it, head, type, member)         \
    for (it = list_data((head)->next, type, member);    \
         &it->member != (head);                         \
         it = list_data(it->member.next, type, member))



/*
 * list_data_criter - constant reverse iterate over list of given type (data)
 * it:     the &struct data to use as a loop cursor(iterator).
 * head:   the &list_head to take the element from.
 * type:   the type of the struct of data this is embedded in.
 * member: the name of the node(list_head) within the struct of data.
 *
 * Note You do not have to change list in this cycle.
 */
#define list_data_criter(it, head, type, member)        \
    for (it = list_data((head)->prev, type, member);    \
         &it->member != (head);                         \
         it = list_data(it->member.prev, type, member))



/*
 * list_data_iter - iterate over list of given type
 * it:     the &struct data to use as a loop cursor(iterator).
 * head:   the &list_head to take the element from.
 * tmp_it: another &struct list_head to use as temporary cursor(iterator)
 * type:   the type of the struct of data this is embedded in.
 * member: the name of the node(list_head) within the struct of data.
 */
#define list_data_iter(it, head, tmp_it, type, member)                         \
    for (it = list_data((head)->next, type, member), tmp_it = it->member.next; \
         &it->member != (head);                                                \
         it = list_data(tmp_it, type, member), tmp_it = it->member.next)



/*
 * list_data_riter - reverse iterate over list of given type
 *
 * it:     the &struct data to use as a loop cursor(iterator).
 * head:   the &list_head to take the element from.
 * tmp_it: another &struct list_head to use as temporary cursor(iterator)
 * type:   the type of the struct of data this is embedded in.
 * member: the name of the node(list_head) within the struct of data.
 */
#define list_data_riter(it, head, tmp_it, type, member)                        \
    for (it = list_data((head)->prev, type, member), tmp_it = it->member.prev; \
         &it->member != (head);                                                \
         it = list_data(tmp_it, type, member), tmp_it = it->member.prev)





//---------------- Algorithm ----------------





/*
 * list_for_each - Applies function fn to each of the elements in the range [first,last)
 *
 * first, last is Input iterators to the initial and final positions in a sequence.
 * The range used is [first,last), which contains all the elements
 * between first and last, including the element pointed by first
 * but not the element pointed by last.
 *
 * first:  the &list_head to use as a loop cursor(iterator).
 * last:   the &list_head to use as a last element.
 * fn:     Unary function that accepts an element in the range as argument.
 */
#define list_for_each(first, last, fn)  \
    for(; first != last; fn(first), first = first->next);




#endif  //LIST_HEADER
