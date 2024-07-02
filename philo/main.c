/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 03:01:10 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/07/02 23:20:40 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* DEBUG */
void	debug_show_input(int input[IN_LEN]);

/* util.c */
int		_atoi(char *num);

/* main.c */
int		usage(char *name);
void	get_input(int ac, char **av, int input[IN_LEN]);
void	init(int input[IN_LEN]);
int		main(int ac, char **av);

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
	input_text = (char *[]){"IN_NUM_PHILO", "IN_TIME_DIE", "IN_TIME_EAT",
		"IN_TIME_SLEEP", "IN_NUM_EAT"};
	while (++i < IN_LEN)
		printf("input[%d](%s) = %d\n", i, input_text[i], input[i]);
	printf("\n");
}

void	get_input(int ac, char **av, int input[IN_LEN])
{
	int	i;

	i = -1;
	memset(input, 0, IN_LEN);
	while (++i < ac)
		input[i] = _atoi(*av++);
	if (i == IN_NUM_EAT)
		input[i] = -1;
}

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

void	init(int input[IN_LEN])
{
	pthread_t	philo;
	int			id;


	(void)philo;
	(void)id;
	(void)input;
}

// void	de_init(void);

int	main(int ac, char **av)
{
	int	input[IN_LEN];

	if (ac < 5 || ac > 6)
		return (usage(*av));
	get_input(--ac, ++av, input);
	debug_show_input(input);
	return (0);
}
