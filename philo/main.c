/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 03:01:10 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/12 18:45:30 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* DEBUG.c
debug_show_input(int input[IN_LEN]);
*/

/* util.c
int	_atoi(char *num);
get_time(void);
*/

/* main.c
usage(char *name);
get_input(int ac, char **av, int input[IN_LEN]);
init(int input[IN_LEN]);
de_init(void);
main(int ac, char **av);
*/

size_t	_atoi(char *num, size_t *err)
{
	size_t	res;

	res = 0;
	if (!(*num >= '0' && *num <= '9'))
		return ((*err)++);
	while (*num >= '0' && *num <= '9')
		res = (res * 10) + (*num++ - '0');
	if ((int)res < 0 || res > INT_MAX)
		return ((*err)++);
	return (res);
}

int	usage(char *name)
{
	printf("Usage: %s number_of_philosophers time_to_die time_to_eat "
		"time_to_sleep [number_of_times_each_philosopher_must_eat]\n", name);
	return (1);
}

void	debug_show_input(size_t input[IN_LEN])
{
	int		i;
	char	**input_text;

	i = -1;
	input_text = (char *[]){
		"IN_ARGC", "IN_NUM_PHILO", "IN_TIME_DIE", "IN_TIME_EAT",
		"IN_TIME_SLEEP", "IN_NUM_EAT", "IN_ERROR", "IN_START_TIME"};
	while (++i < IN_LEN)
		printf("input[%d](%s) = %lu\n", i, input_text[i], input[i]);
	printf("\n");
}

size_t	get_time(size_t start)
{
	struct timeval	t;

	if (gettimeofday(&t, NULL))
		write(2, "Error: get_time!\n", 17);
	return ((t.tv_sec * 1000 + t.tv_usec / 1000) - start);
}

void	get_input(int ac, char **av, size_t input[IN_LEN])
{
	int	i;

	memset(input, 0, IN_LEN * sizeof(size_t));
	i = 0;
	input[i] = ac;
	while (++i <= ac)
		input[i] = _atoi(*av++, &input[IN_ERROR]);
	input[IN_START_TIME] = get_time(0);
}

void	*philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->status >= 0)
	{
		philo->status = 0;
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&philo->next->fork);
		philo->status = 1;
		if (get_time(philo->input[IN_START_TIME]) > philo->input[IN_TIME_DIE])
			philo->status = -1;
		usleep(philo->input[IN_TIME_EAT] * 1000);
		philo->meals_eaten++;
		philo->time_last_meal = get_time(philo->input[IN_START_TIME]);
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->next->fork);
		philo->status = 2;
		usleep(philo->input[IN_TIME_SLEEP] * 1000);
		printf("%zu %d %d %d\n", get_time(philo->input[IN_START_TIME]), philo->id, philo->status, philo->meals_eaten);
	}
	return (NULL);
}

t_philo *init(size_t input[IN_LEN])
{
	int			i;
	t_philo		*res;

	i = input[IN_NUM_PHILO];
	res = malloc(sizeof(t_philo) * i);
	memset(res, 0, input[IN_NUM_PHILO] * sizeof(t_philo));
	while (i--)
	{
		res[i].id = i + 1;
		pthread_mutex_init(&res[i].fork, NULL);
		res[i].next = &res[(i + 1) % input[IN_NUM_PHILO]];
		res[i].input = input;
	}
	return (res);
}

int	main(int ac, char **av)
{
	size_t	input[IN_LEN];
	t_philo	*philos;

	if (ac < 5 || ac > 6)
		return (usage(*av));
	get_input(--ac, ++av, input);
	if (input[IN_ERROR])
		return (write(2, "Invalid input!\n", 15), 1);
	philos = init(input);
	debug_show_input(input);
	auto size_t i = -1;
	while (++i < input[IN_NUM_PHILO])
		pthread_create(&philos[i].thread, NULL, philo, &philos[i]);
	i = -1;
	while (++i < input[IN_NUM_PHILO])
		pthread_join(philos[i].thread, NULL);
	free(philos);
	return (0);
}
