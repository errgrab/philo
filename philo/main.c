/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 03:01:10 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/14 18:36:08 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* DEBUG.c
void	debug_show_input(t_state *state);
*/

/* philo.c
void	*routine(void *ptr)
t_philo	*philo_init(t_state *state)
*/

/* main.c */
int	usage(char *name);
int	main(int ac, char **av);

void	debug_show_input(t_state *state)
{
	printf("argc = %zu\n", state->argc);
	printf("num_philo = %zu\n", state->num_philo);
	printf("time_die = %zu\n", state->time_die);
	printf("time_eat = %zu\n", state->time_eat);
	printf("time_sleep = %zu\n", state->time_sleep);
	printf("eat_limit = %zu\n", state->eat_limit);
}

/*
size_t	new_status(size_t old, size_t new)
{
	if (old < 3)
		return (new);
	return (old);
}

void	*philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->status >= 0)
	{
		philo->status = new_status(philo->status, 0);
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&philo->next->fork);
		philo->status = new_status(philo->status, 1);
		if (get_time(philo->input[IN_START_TIME]) > philo->input[IN_TIME_DIE])
			philo->status = new_status(philo->status, 3);
		usleep(philo->input[IN_TIME_EAT] * 1000);
		philo->meals_eaten++;
		philo->time_last_meal = get_time(philo->input[IN_START_TIME]);
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->next->fork);
		philo->status = new_status(philo->status, 2);
		usleep(philo->input[IN_TIME_SLEEP] * 1000);
		auto char *text[] =
		{"is thinking", "is eating", "is sleeping", "is DEAD"};
		printf("%zu\t%zu %s %zu\n", get_time(philo->input[IN_START_TIME]),
				philo->id, text[philo->status], philo->meals_eaten);
	}
	return (NULL);
}
*/

void	*routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (philo->status < 3)
	{
		printf("%zu\t%zu\n", get_time(philo->state->start_time), philo->id);
	}
	return (NULL);
}

t_philo	*philo_init(t_state *state)
{
	t_philo	*res;

	auto int i = state->num_philo;
	res = malloc(sizeof(t_philo) * i);
	memset(res, 0, sizeof(t_philo) * i);
	while (i--)
	{
		res[i].id = i + 1;
		pthread_mutex_init(&res[i].fork, NULL);
		res[i].next = &res[(i + 1) % state->num_philo];
		res[i].state = state;
	}
	return (res);
}

int	usage(char *name)
{
	printf("Usage: %s number_of_philosophers time_to_die time_to_eat "
		"time_to_sleep [number_of_times_each_philosopher_must_eat]\n", name);
	return (1);
}

int	main(int ac, char **av)
{
	t_state	state;

	memset(&state, 0, sizeof(state));
	if (ac < 5 || ac > 6)
		return (usage(*av));
	get_input(--ac, ++av, &state);
	if (state.err)
		return (write(2, "Invalid input!\n", 15), 1);
	debug_show_input(&state);
	state.philos = philo_init(&state);
	auto size_t i = -1;
	while (++i < state.num_philo)
		pthread_create(&state.philos[i].thread, NULL, routine,
			&state.philos[i]);
	i = -1;
	while (++i < state.num_philo)
		pthread_join(state.philos[i].thread, NULL);
	free(state.philos);
	return (0);
}
