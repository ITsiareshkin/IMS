#include "../headers/tasks.hpp"

tasks::tasks(/* args */)
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

task tasks::get()
{
    return this->task_list[0];
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