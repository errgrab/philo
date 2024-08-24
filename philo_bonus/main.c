/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 17:18:43 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/24 04:15:24 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int			usage(char *name, int err);
int			main(int ac, char **av);

int	usage(char *name, int err)
{
	if (err == 0)
	{
		write(2, "Usage: ", 7);
		while (*name)
			write(2, name++, 1);
		write(2, " number_of_philosophers time_to_die time_to_eat", 47);
		write(2,
			" time_to_sleep [number_of_times_each_philosophers_must_eat].\n",
			61);
	}
	else if (err == 1)
		write(2, "Err: Use values between 1 and INT_MAX!\n", 39);
	else
		write(2, "Err: Unknown error!\n", 20);
	return (1);
}

static void	*limit_eat(void *ptr)
{
	t_state	*state;

	state = (t_state *)ptr;
	auto size_t i = -1;
	while (++i < state->num_philo)
		sem_wait(state->main);
	i = -1;
	while (++i < state->num_philo)
	{
		sem_post(state->end);
		sem_post(state->philo.forks);
	}
	return (NULL);
}

static void	end(t_state *state)
{
	if (state->argc == 5)
		pthread_create(&state->thread, 0, limit_eat, state);
	while (1)
		if (waitpid(-1, NULL, 0) <= 0)
			break ;
	if (state->argc == 5)
		pthread_join(state->thread, NULL);
	sem_close(state->print);
	sem_close(state->main);
	sem_close(state->end);
	sem_close(state->philo.forks);
	sem_unlink("/print");
	sem_unlink("/forks");
	sem_unlink("/end");
	sem_unlink("/main");
}

static void	start(t_state *state)
{
	auto size_t i = -1;
	while (++i < state->num_philo)
	{
		if (!fork())
		{
			state->philo.id = i + 1;
			state->philo.name = get_semaphore_name("/philo", state->philo.id);
			sem_unlink(state->philo.name);
			state->philo.sem = sem_open(state->philo.name, O_CREAT, O_RDWR, 1);
			state->philo.last_meal = get_time_now();
			pthread_create(&state->philo.thread, 0, patrol, state);
			pthread_create(&state->philo.end, 0, end_patrol, state);
			philo_routine(state);
			break ;
		}
		usleep(100);
	}
	if (state->philo.id == 0)
		end(state);
}

int	main(int ac, char **av)
{
	t_state	state;

	memset(&state, 0, sizeof(state));
	if (ac < 5 || ac > 6)
		return (usage(*av, 0));
	get_input(--ac, ++av, &state);
	if (state.err)
		return (usage(*av, 1));
	start(&state);
	return (0);
}
