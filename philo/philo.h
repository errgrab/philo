/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 20:59:58 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/06/27 22:35:07 by ecarvalh         ###   ########.fr       */
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
	int				meals_eaten;
	int				status; // -1: dead, 0: thinking, 1: eating, 2: sleeping
	int				id;
	t_philo	*next;
};

#endif
