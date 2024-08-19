/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 17:19:07 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/19 17:27:00 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo			*philo_init(t_state *state);
void			*philo_routine(void *ptr);
static size_t	philo_eat_routine(t_philo *philo);
static void		philo_get_fork(t_philo *philo, t_philo *philo_fork);

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

static size_t	philo_eat_routine(t_philo *philo)
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

static void	philo_get_fork(t_philo *philo, t_philo *philo_fork)
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
