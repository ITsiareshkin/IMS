#include "../headers/tasks.hpp"

tasks::tasks()
{
}

tasks::~tasks()
{
}

int tasks::size()
{
    return task_list.size();
}

bool tasks::empty()
{
    return task_list.empty();
}

task tasks::get(int pid, task_names name)
{
    if (pid == -1 || name == NONE) return this->task_list[0];
    std::vector<task>::iterator iter = this->task_list.begin();
    while(iter != this->task_list.end())
    {
        if (name == iter->task_name && pid == iter->pid)
        {
            return *iter;
        }
        iter++;
    }
    return {NONE, -1, -1};

}
task tasks::pop()
{
    task to_return = this->task_list[0];
    this->task_list.erase(this->task_list.begin());
    return to_return;
}
void tasks::push(task push_task)
{
    if (this->empty())
    {
        this->task_list.insert(task_list.begin(), push_task);
    }
    else
    {
        std::vector<task>::iterator iter = this->task_list.begin();
        while(iter != this->task_list.end())
        {
            if (push_task.a_time <= iter->a_time)
            {
                this->task_list.insert(iter, push_task);
                return;
            }
            iter++;
        }
        this->task_list.insert(task_list.end(), push_task);
    }
}

bool tasks::is_in(task f_task)
{
    std::vector<task>::iterator iter = this->task_list.begin();
    while(iter != this->task_list.end())
    {
        if (f_task.task_name == iter->task_name && f_task.pid == iter->pid)
        {
            return true;
        }
        iter++;
    }
    return false;
}

void tasks::remove(int pid, task_names name)
{
    std::vector<task>::iterator iter = this->task_list.begin();
    if (name == NONE)
    {
        while(iter != this->task_list.end())
        {
            if (pid == iter->pid)
            {
                this->task_list.erase(iter);
                continue;
            }
            iter++;
        }
    }
    else
    {
         while(iter != this->task_list.end())
        {
            if (pid == iter->pid && name == iter->task_name)
            {
                this->task_list.erase(iter);
                continue;
            }
            iter++;
        }
    }
}