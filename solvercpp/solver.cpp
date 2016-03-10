#include "solver.h"

Solver::Solver(QObject *parent):QObject(parent){

}

bool Solver::solve(struct level Level, std::chrono::steady_clock::time_point t)
{
    //srand(time(0));
    initialize_allocator();

    //struct level* levels = load_xsb_levels(Level);
    struct level* levels = new level[1];
    levels[0] = Level;
	struct level* last = levels;
	int count = 1;
	
	while (last != NULL)
	{
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        millisecs_t duration( std::chrono::duration_cast<millisecs_t>(currentTime - t));
        updateTimer(duration.count());

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
				if (!check_solution(&lvl_sol, last,0))
					solvable = 0;
                else
                    return 1;
		}


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
