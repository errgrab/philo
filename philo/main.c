/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:57:41 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/19 03:46:51 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	usage(char *name);
int	main(int ac, char **av);

t_philo	*philo_init(t_state *state)
{
	t_philo	*res;
	size_t	now;

	auto int i = state->num_philo;
	res = malloc(sizeof(t_philo) * i);
	if (!res)
		return (state->err++, NULL);
	memset(res, 0, sizeof(t_philo) * i);
	now = get_time_now();
	while (i--)
	{
		res[i].id = i + 1;
		res[i].last_meal = now;
		res[i].fork = 1;
		res[i].time_eat = state->time_eat;
		res[i].time_sleep = state->time_sleep;
		pthread_mutex_init(&res[i].mutex, NULL);
		res[i].next = &res[(i + 1) % state->num_philo];
		res[i].state = state;
	}
	return (res);
}

void	*philo_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (1)
	{
		if (philo_eat_routine(philo))
			break ;
		pthread_mutex_lock(&philo->mutex);
		if (philo->dead)
			break ;
		philo->sleeping++;
		pthread_mutex_unlock(&philo->mutex);
		usleep(philo->time_sleep * 1000);
		pthread_mutex_lock(&philo->mutex);
		if (philo->dead)
			break ;
		philo->thinking++;
		pthread_mutex_unlock(&philo->mutex);
	}
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

size_t	philo_eat_routine(t_philo *philo)
{
	philo_get_fork(philo, philo);
	philo_get_fork(philo, philo->next);
	pthread_mutex_lock(&philo->mutex);
	if (philo->dead)
		return (1);
	philo->eating++;
	philo->meals_eaten++;
	philo->last_meal = get_time_now();
	pthread_mutex_unlock(&philo->mutex);
	usleep(philo->time_eat * 1000);
	pthread_mutex_lock(&philo->mutex);
	philo->meals_eaten++;
	philo->fork++;
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_lock(&philo->next->mutex);
	philo->next->fork++;
	pthread_mutex_unlock(&philo->next->mutex);
	return (0);
}

void	philo_get_fork(t_philo *philo, t_philo *philo_fork)
{
	while (1)
	{
		pthread_mutex_lock(&philo->mutex);
		if (philo->dead)
		{
			pthread_mutex_unlock(&philo->mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->mutex);
		pthread_mutex_lock(&philo_fork->mutex);
		if (!philo_fork->fork)
		{
			pthread_mutex_unlock(&philo_fork->mutex);
			usleep(1000);
			continue ;
		}
		philo_fork->fork--;
		pthread_mutex_unlock(&philo_fork->mutex);
		pthread_mutex_lock(&philo->mutex);
		philo->in_hand++;
		pthread_mutex_unlock(&philo->mutex);
		break ;
	}
}

void	check_isalive(t_state *state, t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex);
	if (get_time_now() - philo->last_meal > state->time_die)
	{
		printf("%zu\t%zu %s\n", get_time(), philo->id, "is DEAD");
		philo->dead++;
		state->err++;
	}
	pthread_mutex_unlock(&philo->mutex);
	if (state->argc == 5)
	{
		auto size_t i = state->num_philo;
		auto size_t min = state->philos[i - 1].meals_eaten;
		while (i-- > 0)
			if (min > state->philos[i].meals_eaten)
				min = state->philos[i].meals_eaten;
		if (min >= state->eat_limit)
			state->err++;
	}
}

void	check_end(t_state *state)
{
	size_t	i;

	i = state->num_philo;
	while (i-- > 0)
	{
		pthread_mutex_lock(&state->philos[i].mutex);
		state->philos[i].dead++;
		pthread_mutex_unlock(&state->philos[i].mutex);
		pthread_join(state->philos[i].thread, NULL);
	}
	free(state->philos);
}

void	check_watchphilos(t_state *state)
{
	t_philo	*philo;

	philo = state->philos;
	while (!state->err)
	{
		pthread_mutex_lock(&philo->mutex);
		while (philo->in_hand > 0 && philo->in_hand--)
			printf("%zu\t%zu %s\n", get_time(), philo->id, "has taken a fork");
		if (philo->thinking > 0 && philo->thinking--)
			printf("%zu\t%zu %s\n", get_time(), philo->id, "is thinking");
		if (philo->eating > 0 && philo->eating--)
			printf("%zu\t%zu %s\n", get_time(), philo->id, "is eating");
		if (philo->sleeping > 0 && philo->sleeping--)
			printf("%zu\t%zu %s\n", get_time(), philo->id, "is sleeping");
		pthread_mutex_unlock(&philo->mutex);
		philo = philo->next;
		check_isalive(state, philo);
	}
	check_end(state);
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
		return (write(2, "Err: Use values between 1 and INT_MAX!\n", 39), 1);
	state.philos = philo_init(&state);
	if (state.err)
		return (write(2, "Err: Unable to allocate philosophers!\n", 37), 1);
	auto size_t i = state.num_philo;
	while (i-- > 0)
		pthread_create(&state.philos[i].thread, NULL, philo_routine,
			&state.philos[i]);
	check_watchphilos(&state);
	return (0);
}
