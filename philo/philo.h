/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 20:59:58 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/08/14 18:39:47 by ecarvalh         ###   ########.fr       */
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
	size_t	time_last_meal;
	size_t	meals_eaten;
	size_t	id;
	size_t	status; /* 3: dead, 0: thinking, 1: eating, 2: sleeping */
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

#endif
