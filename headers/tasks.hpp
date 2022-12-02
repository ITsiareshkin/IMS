#include <vector>

enum task_names
{
    RELOAD_ISK,
    FIRST_PART,
    SECOND_PART,
    DETECT_TARGET,
};

struct task
{
    task_names task_name;
    float a_time;
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

