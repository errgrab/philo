/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:35:55 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/19 03:21:06 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	_atoi(char *num, size_t *err);
size_t	get_time_now(void);
size_t	get_time(void);
void	get_input(int ac, char **av, t_state *state);

size_t	_atoi(char *num, size_t *err)
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
	state->argc = ac;
	state->num_philo = _atoi(av[0], &state->err);
	state->time_die = _atoi(av[1], &state->err);
	state->time_eat = _atoi(av[2], &state->err);
	state->time_sleep = _atoi(av[3], &state->err);
	if (ac == 5)
		state->eat_limit = _atoi(av[4], &state->err);
	get_time();
}
