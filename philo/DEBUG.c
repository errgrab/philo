#include "philo.h"

void	debug_show_input(t_state *state)
{
	printf("argc = %zu\n", state->argc);
	printf("num_philo = %zu\n", state->num_philo);
	printf("time_die = %zu\n", state->time_die);
	printf("time_eat = %zu\n", state->time_eat);
	printf("time_sleep = %zu\n", state->time_sleep);
	printf("eat_limit = %zu\n", state->eat_limit);
}

