#include "solver.h"

Solver::Solver(QObject *parent):QObject(parent){

}

bool Solver::solve(struct level Level, float t, struct solution &sol)
{
    initialize_allocator();

    struct level* levels = new level[1];
    levels[0] = Level;
	struct level* last = levels;
	int count = 1;
    timeout = t;
    qDebug() << "Solving";
	
    while (last != NULL && !threadStop)
	{
        if(DOUT)printf("Solve \n");
		printf("Level %d:\n", count);
		memcpy(&lvl, last, sizeof(struct level));
        solvable = 1;
		lvl_sol.length = 0;
        preprocess_level();
        calculate_static_deadlocks();
        print_level(&lvl);

		if (!solvable)
			printf("NO SOLUTION!\n");
		else
		{
            try_solve_level();
			if (solvable)
                if (!check_solution(&lvl_sol, last,0)){
					solvable = 0;
                }
                else{
                    sol = lvl_sol;
                    return 1;
                }
		}

        if(DOUT)printf("Next Level: %p\n", last->next_level);
		last = last->next_level;
		count++;
	}
}

void print_binary(unsigned int x)
{
	unsigned int div = 1 << 31;
	for (int i = 0; i < 32; i++)
	{
		printf("%d", x / div);
		x %= div;
		div /= 2;
	}
}
