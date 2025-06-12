#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define ID_LENGTH 16


const char RAND_DICT[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
                          'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                          'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '!'};

const size_t DICT_SIZE = sizeof(RAND_DICT) / sizeof(RAND_DICT[0]);

char *generate_id()
{
    char *s = malloc(ID_LENGTH + 1);
    if (!s)
        return NULL;

    for (int i = 0; i < ID_LENGTH; i++)
    {
        int idx = rand() % DICT_SIZE;
        s[i] = RAND_DICT[idx];
    }
    s[ID_LENGTH] = '\0';
    return s;
}

struct task *task_init(char *cmd, time_t schedule_at)
{
    struct task *t = malloc(sizeof(struct task));
    t->id = generate_id();
    t->cmd = malloc(strlen(cmd) + 1);
    t->execute_at = schedule_at;
    strcpy(t->cmd, cmd);

    return t;
}

void remove_task(struct task_list *tl, char *id)
{
    size_t task_idx = SIZE_MAX;
    for (size_t i = 0; i < tl->length; i++)
    {
        struct task *t = tl->items[i];
        if (!strcmp(t->id, id))
        {
            task_idx = i;
            break;
        }
    }

    if (task_idx == SIZE_MAX)
    {
        return;
    }

    struct task *tmp = tl->items[task_idx];
    tl->items[task_idx] = tl->items[tl->length - 1];
    tl->items[tl->length - 1] = NULL;
    tl->length--;
    destroy_task(tmp);
}

struct task_list *task_list_init()
{
    struct task_list *tl = malloc(sizeof(struct task_list));
    tl->capacity = 4;
    tl->length = 0;
    struct task **tmp = malloc(sizeof(struct task *) * tl->capacity);

    if (tmp == NULL)
    {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    tl->items = tmp;
    return tl;
}

void push_task(struct task_list *list, struct task *t)
{
    if (list->length >= list->capacity)
    {
        list->capacity *= 2;
        struct task **tmp = realloc(list->items, list->capacity * sizeof(struct task *));
        if (!tmp)
        {
            fprintf(stderr, "Out of memory!\n");
            exit(EXIT_FAILURE);
        }
        list->items = tmp;
    }

    list->items[list->length++] = t;
}

struct task **get_tasks_to_execute(struct task_list *tl, size_t *out_len)
{
    time_t now = time(NULL);
    *out_len = 0;

    struct task **res = malloc(sizeof(struct task *) * tl->length);
    if (res == NULL)
    {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < tl->length; i++)
    {
        struct task *t = tl->items[i];
        if (t->execute_at <= now)
        {
            res[(*out_len)++] = t;
        }
    }

    if (*out_len == 0)
    {
        free(res);
        return NULL;
    }

    struct task **tmp = realloc(res, sizeof(struct task *) * (*out_len));
    if (tmp == NULL)
    {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    res = tmp;
    return res;
}

void destroy_task_list(struct task_list *tl)
{
    for (size_t i = 0; i < tl->length; i++)
    {
        destroy_task(tl->items[i]);
    }
    free(tl->items);
    free(tl);
}

void destroy_task(struct task *t)
{
    free(t->id);
    free(t->cmd);
    free(t);
}