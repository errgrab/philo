/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anon <anon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 20:59:58 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/07/16 21:48:24 by anon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>    /* printf */
# include <stdlib.h>   /* malloc, free */
# include <string.h>   /* memset */
# include <unistd.h>   /* write, usleep */
# include <sys/time.h> /* gettimeofday */
# include <pthread.h>  /* pthread_{ create, detach, join,
						 mutex_{ init, destroy, lock, unlock } } */

enum e_input
{
	IN_NUM_PHILO,
	IN_TIME_DIE,
	IN_TIME_EAT,
	IN_TIME_SLEEP,
	IN_NUM_EAT,
	IN_LEN
};

typedef struct s_philo	t_philo;
struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	write;
	pthread_mutex_t	fork;
	size_t			time_last_meal;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_eaten;
	int				status; // -1: dead, 0: thinking, 1: eating, 2: sleeping
	int				id;
	t_philo	*next;
};

#endif
