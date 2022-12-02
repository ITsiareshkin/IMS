#include <vector>

enum task_names
{
    RELOAD_ISK,
    FIRST_PART,
    SECOND_PART,
    DETECT_TARGET,
    LAUNCH_ISK,
};

struct task
{
    task_names task_name;
    float a_time;
    int pid;
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
    task get();
    task pop();
    void push(task);
};

