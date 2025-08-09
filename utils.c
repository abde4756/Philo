/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:24:24 by aait-laf          #+#    #+#             */
/*   Updated: 2025/04/04 15:31:37 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int		signe;
	long	res;
	int		i;

	i = 0;
	signe = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		if (str[i] == '-')
			signe = -1;
		i++;
	}
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			res = res * 10 + (str[i] - '0');
		if (res > 2147483647 || !(str[i] >= '0' && str[i] <= '9'))
			return (-1);
		i++;
	}
	return (res * signe);
}

// Fonction thread-safe pour vérifier si la simulation doit s'arrêter
int check_death(t_philo *p)
{
    int result;
    pthread_mutex_lock(&p->flag_mutex);
    result = (p->flag_eat == 2);
    pthread_mutex_unlock(&p->flag_mutex);
    return result;
}

// Fonction thread-safe pour signaler la mort
void set_death_flag(t_philo *p)
{
    pthread_mutex_lock(&p->flag_mutex);
    p->flag_eat = 2;
    pthread_mutex_unlock(&p->flag_mutex);
}
