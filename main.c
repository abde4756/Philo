/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:18:28 by aait-laf          #+#    #+#             */
/*   Updated: 2025/04/28 16:22:42 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void    initial_sct(t_philo *p)
{
    p->nbr_philo = 0;
    p->num_eat = -1;
    p->time_eat = 0;
    p->flag_current = 0;
    p->time_sleep = 0;
    p->time_to_die = 0;
    p->current = 0;
    // p->last_eat = 0;
    p->flag_eat = 0;
}

void    initai_sct2(t_elemt *philo)
{
    philo->idx_philo = 0;
    philo->tid_philo = 0;
    philo->last_eat = 0;
}

int main(int ac, char **av)
{
    t_philo     *p;
    t_elemt     *philo;
    
    if(ac != 5 && ac != 6)
        return(1);
    p = malloc(sizeof(t_philo));
    initial_sct(p);
    
    p->nbr_philo = ft_atoi(av[1]);
    p->time_to_die = ft_atoi(av[2]);
    p->time_eat = ft_atoi(av[3]);
    p->time_sleep = ft_atoi(av[4]);
   
    philo =  malloc(sizeof(t_elemt) * p->nbr_philo);
    initai_sct2(philo);
   
    philo->philo = p;
    if (av[5])
        p->num_eat = ft_atoi(av[5]);
    creat_philo(p, philo);

    // printf("p->nbr_philo == %d\n", p->nbr_philo);
    // printf("p->time_to_die == %d\n", p->time_to_die);
    // printf("p->time_eat == %d\n", p->time_eat);
    // printf(" p->time_sleep == %d\n",  p->time_sleep);
    // printf("p->num_eat == %d\n", p->num_eat);

}
