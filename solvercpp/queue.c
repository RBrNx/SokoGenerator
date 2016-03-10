#include "queue.h"
#include "allocator.h"

struct cqueue* create_queue_node(void* element)
{
    struct cqueue* tmp = malloc_node();
	tmp->e = element;
	return tmp;
}

void append_queue_node(struct cqueue* node, struct cqueue* after) // adds node next to after
{
	node->next = after->next;
	after->next = node;
}

struct cqueue* remove_queue_node(struct cqueue* before) // removes node next to "before" from queue and returns it
{
    struct cqueue* tmp = before->next;
	before->next = before->next->next;
	return tmp;
}

int is_queue_empty(struct cqueue* head)
{
	return !head->next;
}
