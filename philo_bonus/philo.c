/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 03:08:13 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/24 16:03:37 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void		*patrol(void *ptr);
void		*end_patrol(void *ptr);
void		philo_routine(t_state *state);
static void	philo_end_routine(t_state *state);
static void	_log(t_state *state, char *str);

void	*patrol(void *ptr)
{
	auto t_state * state = (t_state *)ptr;
	while (1)
	{
		sem_wait(state->philo.sem);
		if (get_time_now() - state->philo.last_meal > state->time_die
			|| !state->philo.alive)
			break ;
		if (state->philo.meals_eaten == state->eat_limit)
		{
			sem_post(state->main);
			state->philo.meals_eaten++;
		}
		sem_post(state->philo.sem);
		usleep(100);
	}
	sem_post(state->philo.sem);
	_log(state, "died");
	sem_wait(state->philo.sem);
	state->philo.alive = 0;
	sem_post(state->philo.sem);
	auto size_t i = 0;
	while (i++ < state->num_philo)
		sem_post(state->end);
	sem_post(state->philo.forks);
	return (NULL);
}

void	*end_patrol(void *ptr)
{
	t_state	*state;

	state = (t_state *)ptr;
	sem_wait(state->end);
	sem_wait(state->philo.sem);
	state->philo.alive = 0;
	sem_post(state->philo.sem);
	sem_post(state->end);
	auto size_t i = (size_t)-1;
	while (++i < state->num_philo)
		sem_post(state->main);
	return (NULL);
}

void	philo_routine(t_state *state)
{
	while (1)
	{
		sem_wait(state->philo.forks);
		_log(state, "has taken a fork");
		sem_wait(state->philo.forks);
		_log(state, "has taken a fork");
		sem_wait(state->philo.sem);
		state->philo.last_meal = get_time_now();
		if (!state->philo.alive)
			break ;
		sem_post(state->philo.sem);
		_log(state, "is eating");
		usleep(state->philo.time_eat * 1000);
		sem_post(state->philo.forks);
		sem_post(state->philo.forks);
		sem_wait(state->philo.sem);
		state->philo.meals_eaten++;
		sem_post(state->philo.sem);
		_log(state, "is sleeping");
		usleep(state->philo.time_sleep * 1000);
		_log(state, "is thinking");
		usleep(100);
	}
	philo_end_routine(state);
}

void	philo_end_routine(t_state *state)
{
	sem_post(state->philo.sem);
	sem_post(state->philo.forks);
	sem_post(state->philo.forks);
	pthread_join(state->philo.thread, NULL);
	pthread_join(state->philo.end, NULL);
	sem_close(state->print);
	sem_close(state->end);
	sem_close(state->main);
	sem_close(state->philo.forks);
	sem_close(state->philo.sem);
	sem_unlink(state->philo.name);
	free(state->philo.name);
}

void	_log(t_state *state, char *str)
{
	sem_wait(state->philo.sem);
	if (!state->philo.alive)
		str = NULL;
	if (str)
	{
		sem_wait(state->print);
		printf("%zu\t%zu %s\n", get_time(), state->philo.id, str);
		sem_post(state->print);
	}
	sem_post(state->philo.sem);
}
