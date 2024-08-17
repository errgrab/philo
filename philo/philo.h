/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 20:59:58 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/16 18:11:41 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>    /* printf */
# include <stdlib.h>   /* malloc, free */
# include <string.h>   /* memset */
# include <unistd.h>   /* write, usleep */
# include <sys/time.h> /* gettimeofday */

# include <pthread.h>
/* pthread_{create,detach,join,mutex_{init,destroy,lock,unlock}} */

# define INT_MAX 2147483647

typedef struct s_state	t_state;
typedef struct s_philo	t_philo;
typedef pthread_mutex_t	t_mutex;
typedef pthread_t		t_hread;

struct s_philo
{
	t_hread	thread;
	t_mutex	mutex;

	size_t	last_meal;
	size_t	meals_eaten;

	size_t	id;

	size_t	time_eat;
	size_t	time_sleep;

	size_t	thinking;
	size_t	eating;
	size_t	sleeping;
	size_t	dead;

	size_t	fork;
	size_t	fork_in_hand;

	t_philo	*next;
	t_state	*state;
};

struct s_state
{
	size_t	argc;

	size_t	num_philo;
	size_t	time_die;
	size_t	time_eat;
	size_t	time_sleep;
	size_t	eat_limit;

	size_t	err;
	size_t	start_time;

	t_philo	*philos;
};

/* utils.c */
size_t	_atoi(char *num, size_t *err);
size_t	get_time(size_t start);
void	get_input(int ac, char **av, t_state *state);

/* DEBUG.c */
void	debug_show_input(t_state *state);

/* main.c */
int		usage(char *name);
int		main(int ac, char **av);

#endif
