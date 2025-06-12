#ifndef TASK_H
#define TASK_H
#include <time.h>

struct task {
    char *id;
    char *cmd;
    time_t execute_at;    
};

struct task_list {
    struct task **items;
    size_t length;
    size_t capacity;
};

void push_task(struct task_list *l, struct task *t);
void remove_task(struct task_list *tl, char* id);
struct task *task_init(char *cmd, time_t schedule_at);
struct task_list *task_list_init();
struct task **get_tasks_to_execute(struct task_list *tl, size_t *out_len);
void destroy_task_list(struct task_list *tl);
void destroy_task(struct task *t);
char *generate_id();
#endif