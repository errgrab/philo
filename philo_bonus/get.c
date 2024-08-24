/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 17:18:55 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/24 04:12:05 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static size_t	_atoi(char *num, size_t *err);
size_t			get_time_now(void);
size_t			get_time(void);
void			get_input(int ac, char **av, t_state *state);
char			*get_semaphore_name(char *start, size_t id);

static size_t	_atoi(char *num, size_t *err)
{
	size_t	res;

	if (!(*num >= '0' && *num <= '9'))
		return ((*err)++);
	res = 0;
	while (*num && (*num >= '0' && *num <= '9'))
		res = (res * 10) + (*num++ - '0');
	if (res > INT_MAX || res == 0)
		return ((*err)++);
	return (res);
}

size_t	get_time_now(void)
{
	struct timeval	t;

	if (gettimeofday(&t, NULL))
		return (write(2, "Error: get_time!\n", 17), 0);
	return ((t.tv_sec * 1000 + t.tv_usec / 1000));
}

size_t	get_time(void)
{
	static size_t	start;
	size_t			now;

	now = get_time_now();
	if (!start)
		start = now;
	return (now - start);
}

void	get_input(int ac, char **av, t_state *state)
{
	memset(state, 0, sizeof(t_state));
	get_time();
	state->argc = ac;
	state->num_philo = _atoi(av[0], &state->err);
	state->time_die = _atoi(av[1], &state->err);
	state->time_eat = _atoi(av[2], &state->err);
	state->time_sleep = _atoi(av[3], &state->err);
	if (ac == 5)
		state->eat_limit = _atoi(av[4], &state->err);
	state->philo.id = 0;
	state->philo.alive = 1;
	state->philo.time_eat = state->time_eat;
	state->philo.time_sleep = state->time_sleep;
	sem_unlink("/print");
	sem_unlink("/forks");
	sem_unlink("/end");
	sem_unlink("/main");
	state->print = sem_open("/print", O_CREAT, O_RDWR, 1);
	state->philo.forks = sem_open("/forks", O_CREAT, O_RDWR,
			state->num_philo);
	state->end = sem_open("/end", O_CREAT, O_RDWR, 0);
	state->main = sem_open("/main", O_CREAT, O_RDWR, 0);
}

char	*get_semaphore_name(char *start, size_t id)
{
	size_t	len;
	size_t	tmp;
	char	*res;

	len = 0;
	while (start[len])
		len++;
	tmp = id;
	len++;
	while (tmp >= 10)
	{
		tmp /= 10;
		len++;
	}
	res = (char *)malloc(len + 1);
	memset(res, 0, len + 1);
	tmp = 0;
	while (*start && len--)
		res[tmp++] = *start++;
	while (len--)
	{
		res[tmp + len] = (id % 10) + '0';
		id /= 10;
	}
	return (res);
}
