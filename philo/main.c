/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 03:01:10 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/07/19 00:33:38 by ecarvalh         ###   ########.fr       */
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

int	_atoi(char *num)
{
	unsigned int	res;
	int				sig;

	res = 0;
	sig = 1;
	while (*num == '-' || *num == '+')
		if (*num++ == '-')
			sig = -sig;
	while (*num >= '0' && *num <= '9')
		res = (res * 10) + (*num++ - '0');
	return (res * sig);
}

int	usage(char *name)
{
	printf("Usage: %s number_of_philosophers time_to_die time_to_eat "
		"time_to_sleep [number_of_times_each_philosopher_must_eat]\n",
		name);
	return (1);
}

void	debug_show_input(int input[IN_LEN])
{
	int		i;
	char	**input_text;

	i = -1;
	input_text = (char *[]){
		"IN_ARGC", "IN_NUM_PHILO", "IN_TIME_DIE", "IN_TIME_EAT",
		"IN_TIME_SLEEP", "IN_NUM_EAT", "IN_ERROR", "IN_LEN"};
	while (++i < IN_LEN)
		printf("input[%d](%s) = %d\n", i, input_text[i], input[i]);
	printf("\n");
}

void	get_input(int ac, char **av, int input[IN_LEN])
{
//	int	i;

	memset(input, 0, IN_LEN);
	(void)ac;
	(void)av;
	/*
	i = 0;
	input[i] = ac - 1;
	while (++i < ac)
		input[i] = _atoi(*av++);
	i = 0;
	while (++i < ac)
		if (input[i] < 0 || input[i] > INT_MAX)
			input[IN_ERROR]++;
			*/
}

size_t	get_time(void)
{
	struct timeval	time;

	if (-1 == gettimeofday(&time, NULL))
		write(2, "Error: get_time!\n", 17);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	*philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->fork);
	pthread_mutex_lock(&philo->next->fork);
	printf("%zu %d TEST\n", get_time(), philo->id);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->next->fork);
	usleep(1000000);
	return (NULL);
}

t_philo *init(int input[IN_LEN])
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
	}
	return (res);
}

int	main(int ac, char **av)
{
	int		input[IN_LEN];
	t_philo	*philos;

	if (ac < 5 || ac > 6)
		return (usage(*av));
	get_input(--ac, ++av, input);
	debug_show_input(input);
	if (input[IN_ERROR])
	{
		printf("input ERROR: %d\n", input[IN_ERROR]);
		return (write(2, "Invalid input!\n", 15), 1);
	}
	philos = init(input);
	/*
	int i = -1;
	while (++i < input[IN_NUM_PHILO])
		pthread_create(&philos[i].thread, NULL, philo, &philos[i]);
	i = -1;
	while (++i < input[IN_NUM_PHILO])
		pthread_join(philos[i].thread, NULL);
	*/
	printf("philo %d, is id %d\n", 0, philos[0].id);
//	debug_show_input(input);
	free(philos);
	return (0);
}
