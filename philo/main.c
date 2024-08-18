/* ************************************************************************** */
/* main.c */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 03:01:10 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/18 22:43:04 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	usage(char *name);
int	main(int ac, char **av);

void	print_log(char *str, t_philo *philo)
{
	printf("%zu\t%zu %s\n", get_time(), philo->id, str);
}

void	philo_next_action(t_philo *philo)
{
	if (philo->action < 3)
		philo->action = (philo->action + 1) % 3 ;
}

size_t	philo_eat_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	pthread_mutex_lock(&philo->next->fork);
	pthread_mutex_lock(&philo->mutex);
	philo->meals_eaten++;
	philo->last_meal = get_time_now();
	philo_next_action(philo);
	if (philo->action == 3)
		return (pthread_mutex_unlock(&philo->fork),
			pthread_mutex_unlock(&philo->next->fork), 1);
	pthread_mutex_unlock(&philo->mutex);
	usleep(philo->time_eat * 1000);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->next->fork);
	return (0);
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
		philo_next_action(philo);
		if (philo->action == 3)
			break ;
		pthread_mutex_unlock(&philo->mutex);
		usleep(philo->time_sleep * 1000);
		pthread_mutex_lock(&philo->mutex);
		philo_next_action(philo);
		if (philo->action == 3)
			break ;
		pthread_mutex_unlock(&philo->mutex);
	}
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

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
		res[i].time_eat = state->time_eat;
		res[i].time_sleep = state->time_sleep;
		res[i].last_action = 42;
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
		return (write(2, "Err: Use values between 1 and INT_MAX!\n", 39), 1);
//	debug_show_input(&state);
	state.philos = philo_init(&state);
	if (state.err)
		return (write(2, "Err: Unable to allocate philosophers!\n", 37), 1);
	auto size_t i = (size_t)-1;
	while (++i < state.num_philo)
		pthread_create(&state.philos[i].thread, NULL, philo_routine,
			&state.philos[i]);
	i = (size_t)-1;
	while (++i < state.num_philo)
		pthread_join(state.philos[i].thread, NULL);
	free(state.philos);
	return (0);
}
