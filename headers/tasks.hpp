#ifndef TASKS_HPP
#define TASKS_HPP

#include <vector>

enum task_names
{
    NONE,
    RELOAD_ISK,
    FIRST_PART,
    SECOND_PART,
    DETECT_TARGET,
    LAUNCH_ISK,
    LAUNCH_PVO,
    TARGET_POINT,
    GET_READY_S300,
};

struct task
{
    task_names task_name;
    float a_time;
    int pid;
    int l_id;
};

class tasks
{
private:
    std::vector <task> task_list;
public:
    tasks(/* args */);
    ~tasks();
    int size();
    bool empty();
    task get(int pid = -1, task_names name = NONE);
    task pop();
    bool is_in(task f_task);
    void remove(int pid, task_names name = NONE);
    void push(task);
};

#endif //TASKS_HPP