/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 03:01:10 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/16 18:15:29 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


/* philo.c
void	*routine(void *ptr)
t_philo	*philo_init(t_state *state)
*/

/* main.c */
int	usage(char *name);
int	main(int ac, char **av);

void	print_log(char *str, t_philo *philo)
{
	printf("%zu\t%zu %s\n", get_time(philo->state->start_time), philo->id, str);
}

void	get_fork(t_philo	*philo)
{
	while (1) {
		pthread_mutex_lock(&philo->mutex);
		if (!philo->fork && !philo->dead)
		{
			pthread_mutex_unlock(&philo->mutex);
			usleep(50);
			continue ;
		}
		philo->fork--;
		philo->fork_in_hand++;
		pthread_mutex_unlock(&philo->mutex);
		break ;
	}
}

void	get_next_fork(t_philo *philo)
{
	while (1) {
		if (philo->dead)
			break ;
		pthread_mutex_lock(&philo->next->mutex);
		if (!philo->next->fork)
		{
			pthread_mutex_unlock(&philo->mutex);
			usleep(50);
			continue ;
		}
		philo->next->fork--;
		pthread_mutex_unlock(&philo->next->mutex);
		pthread_mutex_lock(&philo->mutex);
		philo->fork_in_hand++;
		pthread_mutex_unlock(&philo->mutex);
		break ;
	}
}

void	eat_routine(t_philo *philo)
{
	get_fork(philo);
	get_next_fork(philo);
	pthread_mutex_lock(&philo->mutex);
	philo->eating++;
	philo->last_meal = get_time(0);
	pthread_mutex_unlock(&philo->mutex);
	usleep(philo->time_eat * 1000);
	pthread_mutex_lock(&philo->mutex);
	philo->fork++;
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_lock(&philo->next->mutex);
	philo->next->fork++;
	pthread_mutex_unlock(&philo->next->mutex);
}

void	*routine(void *ptr)
{
	t_philo	*philo;
	t_state	*state;

	philo = (t_philo *)ptr;
	state = philo->state;
	while (1)
	{
		eat_routine(philo);
		pthread_mutex_lock(&philo->mutex);
		if (philo->dead)
			break ;
		philo->meals_count++;
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

t_philo	*philo_init(t_state *state)
{
	t_philo	*res;

	auto int i = state->num_philo;
	res = malloc(sizeof(t_philo) * i);
	if (!res)
		return (state->err++, NULL);
	memset(res, 0, sizeof(t_philo) * i);
	while (i--)
	{
		res[i].id = i + 1;
		res[i].last_meal = get_time(0);
		res[i].time_eat = state->time_eat;
		res[i].time_sleep = state->time_sleep;
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
		return (write(2, "Err: Unable to allocate philosophers\n", 37), 1);
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
