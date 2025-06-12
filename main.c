#include <stdio.h>
#include "task.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define TASK_RETRIEVE_INTERVAL 1

int main(void)
{
    printf("Scheduler running...\nPID: %d\n", getpid());
    struct task_list *tl = task_list_init();
    time_t now = time(NULL);

    for (int i = 0; i < 10000; i++)
    {
        struct task *t = task_init("ls > task_result.txt", now + 5);
        push_task(tl, t);
    }

    while (1)
    {
        size_t tasks_len;
        struct task **tasks_to_execute = get_tasks_to_execute(tl, &tasks_len);

        for (int i = 0; i < tasks_len; i++)
        {
            if (tasks_to_execute == NULL)
            {
                continue;
            }
            struct task *t = tasks_to_execute[i];
            int cmd_result = system(t->cmd);

            if (cmd_result != 0)
            {
                printf("Task execution returned non-zero value.\nCommand: %s\nTask ID:%s\n", t->cmd, t->id);
            }
            remove_task(tl, t->id);
        }

        sleep(TASK_RETRIEVE_INTERVAL);
    }

    return 0;
}
