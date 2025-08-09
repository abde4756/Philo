/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:16:14 by aait-laf          #+#    #+#             */
/*   Updated: 2025/04/28 16:51:32 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void    *routine(void *t)
{
    t_elemt *p = (t_elemt *)t;
    int     i;

    if(p->philo->num_eat != -1)
    {
        i = 0;
        while (i < p->philo->num_eat)
        {
            // Utiliser eat_mutex partagé, pas alloué localement
            pthread_mutex_lock(&p->eat_mutex[p->idx_philo - 1]);
            if(check_death(p->philo))
            {
                pthread_mutex_unlock(&p->eat_mutex[p->idx_philo - 1]);
                break;  // Pas de message, le moniteur s'en occupe
            }
            pthread_mutex_unlock(&p->eat_mutex[p->idx_philo - 1]);
            
            if(thinking_ph(p, p->philo) == -1)
                break;
            if(eting_ph(p, p->philo) == -1)
                break;
            if(sleeping_ph(p, p->philo) == -1)
                break;
            i++;
        }
    }
    else
    {
        while (1)
        {
            // Utiliser eat_mutex partagé, pas alloué localement
            pthread_mutex_lock(&p->eat_mutex[p->idx_philo - 1]);
            if(check_death(p->philo))
            {
                pthread_mutex_unlock(&p->eat_mutex[p->idx_philo - 1]);
                break;  // Pas de message, le moniteur s'en occupe
            }
            pthread_mutex_unlock(&p->eat_mutex[p->idx_philo - 1]);
            
            if(thinking_ph(p, p->philo) == -1)
                break;
            if(eting_ph(p, p->philo) == -1)
                break;
            if(sleeping_ph(p, p->philo) == -1)
                break;
        } 
    }
    return(NULL);
}

unsigned long   get_time(void)
{
    struct timeval t;
    
    gettimeofday(&t, NULL);
    return((t.tv_sec) * 1000 + (t.tv_usec) / 1000);
}

// FONCTION MONITEUR DÉDIÉE
void    *monitor_routine(void *arg)
{
    t_elemt *philos = (t_elemt *)arg;
    int nbr_philo = philos[0].philo->nbr_philo;
    
    while (1)
    {
        int i = 0;
        int philos_finished = 0;  // Compteur des philosophes qui ont fini
        
        while (i < nbr_philo)
        {
            // Utiliser le tableau partagé eat_mutex (tous pointent vers philo[0].eat_mutex)
            pthread_mutex_lock(&philos[0].eat_mutex[i]);
            
            // Vérifier si simulation doit s'arrêter
            if (check_death(philos[0].philo))
            {
                pthread_mutex_unlock(&philos[0].eat_mutex[i]);
                return NULL;  // Quelqu'un est déjà mort
            }
            
            // NOUVEAU: Vérifier si ce philosophe a fini de manger
            if (philos[0].philo->num_eat != -1 && 
                philos[i].meals_eaten >= philos[0].philo->num_eat)
            {
                philos_finished++;
            }
            
            // Calculer le temps depuis le dernier repas
            unsigned long temps_actuel = get_time();
            unsigned long temps_depuis_repas = temps_actuel - philos[i].last_eat;
            
            // MORT DÉTECTÉE ?
            if (temps_depuis_repas > (unsigned long)philos[0].philo->time_to_die)
            {
                // Signal global de mort
                set_death_flag(philos[0].philo);
                
                // Message de mort (format standard avec timestamp relatif)
                unsigned long timestamp = temps_actuel - philos[0].philo->current;
                printf("%lu %d died\n", timestamp, philos[i].idx_philo);
                
                pthread_mutex_unlock(&philos[0].eat_mutex[i]);
                return NULL;  // Arrêter le moniteur
            }
            
            pthread_mutex_unlock(&philos[0].eat_mutex[i]);
            i++;
        }
        
        // NOUVEAU: Si tous les philosophes ont fini de manger, arrêter la simulation
        if (philos[0].philo->num_eat != -1 && philos_finished == nbr_philo)
        {
            return NULL;  // Simulation terminée avec succès
        }
        
        // Vérifier chaque milliseconde
        usleep(1000);
    }
    
    return NULL;
}

void    cleanup_philosophers(t_philo *p, t_elemt *philo)
{
    int i;
    
    // Détruire les mutex des fourchettes
    i = 0;
    while (i < p->nbr_philo)
    {
        pthread_mutex_destroy(&philo->forks[i]);
        pthread_mutex_destroy(&philo[i].last_et);
        i++;
    }
    
    // Détruire les mutex eat_mutex
    i = 0;
    while (i < p->nbr_philo)
    {
        pthread_mutex_destroy(&philo->eat_mutex[i]);
        i++;
    }
    
    // Libérer la mémoire
    free(philo->forks);
    free(philo->eat_mutex);
    free(philo);
    free(p);
}

int    creat_philo(t_philo *p, t_elemt *philo)
{
    int i = 0;
    int k = 0; 
    
    // Vérifier les allocations mémoire
    philo->forks = malloc(sizeof(pthread_mutex_t) * p->nbr_philo);
    if (!philo->forks)
    {
        perror("Failed to allocate forks");
        return (-1);
    }
    
    philo->eat_mutex = malloc(sizeof(pthread_mutex_t) * p->nbr_philo);
    if (!philo->eat_mutex)
    {
        free(philo->forks);
        perror("Failed to allocate eat_mutex");
        return (-1);
    }
    
    int j = 0;
    while (j < p->nbr_philo)
    {
        pthread_mutex_init(&philo->forks[j], NULL);
        pthread_mutex_init(&philo[j].last_et, NULL);
        j++;
    }
    j = 0;
    while (j < p->nbr_philo)
    {
        philo[j].left_fork = &philo->forks[j];
        philo[j].right_fork = &(philo->forks[(j + 1) % p->nbr_philo]);
        j++;
    }

    while (k < p->nbr_philo)
    {
        if(pthread_mutex_init(&philo->eat_mutex[k], NULL) != 0)
        {
            perror("Failed to initialize mutex");
            // Nettoyer les mutex déjà créés
            while (--k >= 0)
                pthread_mutex_destroy(&philo->eat_mutex[k]);
            free(philo->forks);
            free(philo->eat_mutex);
            return (-1);
        }
        k++;
    }
    i = 0;
    
    // Initialiser last_eat avant de créer les threads
    unsigned long start_time = get_time();
    p->current = start_time;  // Temps de référence global
    p->flag_current = 1;      // Marquer comme initialisé
    
    while (i < p->nbr_philo)
    {
        (philo[i]).idx_philo = i + 1;
        philo[i].philo = p;
        philo[i].last_eat = start_time;  // Temps de début
        philo[i].meals_eaten = 0;        // ← NOUVEAU: Initialiser compteur
        
        // IMPORTANT: Tous pointent vers le même tableau eat_mutex
        philo[i].eat_mutex = philo[0].eat_mutex;
        
        i++;
    }
    
    // Créer les threads des philosophes
    i = 0;
    while (i < p->nbr_philo)
    {
        pthread_create(&philo[i].tid_philo, NULL, &routine, &philo[i]);
        i++;
    }
    
    // Créer le thread moniteur
    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, &monitor_routine, philo);
    

    // Attendre que le moniteur détecte une mort
    pthread_join(monitor_thread, NULL);
    
    // Attendre que tous les philosophes se terminent
    i = 0;
    while (i < p->nbr_philo)
    {
        pthread_join(philo[i].tid_philo, NULL);
        i++;
    }
    
    // Nettoyer TOUTE la mémoire à la fin
    cleanup_philosophers(p, philo);
    return (0);
}
