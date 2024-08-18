/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 20:59:58 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/18 22:44:58 by ecarvalh         ###   ########.fr       */
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
	t_mutex	fork;
	t_mutex	mutex;

	size_t	last_meal;
	size_t	meals_eaten;

	size_t	id;

	size_t	time_eat;
	size_t	time_sleep;

	size_t	action; // 0: thinking, 1: eating, 2: sleeping, 3: dead
	size_t	last_action;

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

	t_philo	*philos;
};

/* utils.c */
size_t	_atoi(char *num, size_t *err);
size_t	get_time_now(void);
size_t	get_time(void);
void	get_input(int ac, char **av, t_state *state);

/* DEBUG.c */
void	debug_show_input(t_state *state);

/* ??? */
t_philo	*philo_init(t_state *state);
void	*philo_routine(void *ptr);
size_t	philo_eat_routine(t_philo *philo);
void	philo_next_action(t_philo *philo);

void	print_log(char *str, t_philo *philo);

/* main.c */
int		usage(char *name);
int		main(int ac, char **av);

#endif
