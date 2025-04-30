#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

// #include <unisdt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_philo
{
    char    **array;
    int     nbr_philo;
    int     time_think;
    int     time_eat;
    int     time_sleep;
    int     num_eat;
    int     time_to_die;
    unsigned long current;
    int     flag_current;
    int     flag_eat;
}   t_philo;

typedef enum s_state
{
    THINKING,
    HUNGRY,
    EATING,
    SLEEPING,
}  t_state;

typedef struct s_elemt
{
    t_philo *philo;
    t_state state;
    int    idx_philo;
    pthread_t   tid_philo;
    pthread_mutex_t *forks;
    pthread_mutex_t *eat_mutex;
    pthread_mutex_t last_et;
    unsigned long last_eat;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
} t_elemt;

int	ft_atoi(const char *str);
void    initial_sct(t_philo *p);
//-----------------------


int    sleeping_ph(t_elemt *philo, t_philo *p);
int    eting_ph(t_elemt *philo, t_philo *p);
int    thinking_ph(t_elemt *philo, t_philo *p);

void    creat_philo(t_philo *p, t_elemt *philo);
void    *routine(void *p);
void    initai_sct2(t_elemt *philo);
unsigned long   get_time(void);

#endif
