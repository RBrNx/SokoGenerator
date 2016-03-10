#ifndef SOKOBAN_SOLVER_QUEUE
#define SOKOBAN_SOLVER_QUEUE

struct cqueue
{
    struct cqueue* next;
	void* e;
};

struct cqueue* create_queue_node(void* element);
void append_queue_node(struct cqueue* node, struct cqueue* after); // adds node next to after
struct cqueue* remove_queue_node(struct cqueue* before); // removes node next to "before" from queue and returns it
int is_queue_empty(struct cqueue* head);

#endif // SOKOBAN_SOLVER_QUEUE
