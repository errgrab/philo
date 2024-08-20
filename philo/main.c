/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 17:18:43 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/20 18:57:03 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int			usage(char *name);
int			main(int ac, char **av);
void		check_watchphilos(t_state *state);
static void	check_isalive(t_state *state, t_philo *philo);
static void	check_end(t_state *state);

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
	auto size_t i = (size_t)-1;
	while (++i < state.num_philo)
		pthread_create(&state.philos[i].thread, NULL, philo_routine,
			&state.philos[i]);
	check_watchphilos(&state);
	return (0);
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

void	check_isalive(t_state *state, t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex);
	if (get_time_now() - philo->last_meal > state->time_die)
	{
		printf("%zu\t%zu %s\n", get_time(), philo->id, "died");
		philo->dead++;
		state->err++;
	}
	pthread_mutex_unlock(&philo->mutex);
	if (state->argc != 5)
		return ;
	auto size_t i = (size_t)-1;
	pthread_mutex_lock(&state->philos[0].mutex);
	auto size_t min = state->philos[0].meals_eaten;
	pthread_mutex_unlock(&state->philos[0].mutex);
	while (++i < state->num_philo)
	{
		pthread_mutex_lock(&state->philos[i].mutex);
		if (min > state->philos[i].meals_eaten)
			min = state->philos[i].meals_eaten;
		pthread_mutex_unlock(&state->philos[i].mutex);
	}
	if (min >= state->eat_limit)
		state->err++;
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
