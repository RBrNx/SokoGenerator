#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "queue.h"
#include "move.h"

void initialize_allocator();

struct cqueue* malloc_node();
void free_node(struct cqueue* q);

struct move* malloc_move();
void free_move(struct move* mov);

#endif // ALLOCATOR_H
