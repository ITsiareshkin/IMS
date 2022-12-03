#include <getopt.h>
#include <iostream>
#include "../headers/tasks.hpp"
#include "../headers/s300.hpp"
#include <stdlib.h>
#include <string.h>
#include <ctime>
using namespace std;
int pid=12;
int iskanders, ended, iskander_amm, iskander_ready, tracked_targets;
float dist_to_target, sim_time, detect_time;
tasks tasklist;
int s300_n;
s300 syst[4];
int target_hits=0;
int downed_missiles=0;
int missed=0;
const float main_speed=2.1;
const float max_speed=2.6;
const float s300_speed=2.0;



void help()
{
	cerr << "Usage:\n"
		<< "\t-i or --iskander to specify the number of iskanders\n"
		<< "\t-s or --s300 to specify the number of s300\n"
		<< "\t-d or --distance to specify the distance to target in km\n";
}

void execute(task todo_task)
{
    tasklist.pop();
    switch(todo_task.task_name) {
        
        case RELOAD_ISK:
        {
            iskander_ready++;
            if (iskander_amm != 0 && iskander_ready != 0) tasklist.push({LAUNCH_ISK, sim_time, ++pid, 0});
            break;
        }
        case LAUNCH_ISK:
        {
            iskander_amm--;
            iskander_ready--;
            float reloadtime = float(rand() % 150)/10.0+45;
            tasklist.push({RELOAD_ISK, sim_time+reloadtime, todo_task.pid, 0});
            tasklist.push({FIRST_PART, sim_time+(dist_to_target-5)/main_speed, todo_task.pid, 0});
            tasklist.push({DETECT_TARGET, sim_time+detect_time, todo_task.pid, 0});
            cout << "Time: "<< sim_time << ". Launched iskander. Left: "<< iskander_amm <<"\n";
            break;
        }
        case FIRST_PART:
        {
            cout << "Time: "<< sim_time << ". Missile near the target.\n";
            tasklist.remove(todo_task.pid, LAUNCH_PVO);
            tasklist.push({SECOND_PART, sim_time + 5/max_speed, todo_task.pid, 0});
            break;
        }
        case SECOND_PART:
        {
            cout << "Time: "<< sim_time << ". The missile hit the target.\n";
            ended--;
            tasklist.remove(todo_task.pid, DETECT_TARGET);
            target_hits++;
            break;
        }
        case DETECT_TARGET:
        {
            int lid=s300_ready();
            if (lid!=-1)
            {
                tasklist.push({LAUNCH_PVO, sim_time + 1, todo_task.pid, lid});
                s300_taketarget(lid);
            }
            else tasklist.push({DETECT_TARGET, sim_time + 0.001, todo_task.pid, 0}); //FIX TIME TO READY
            break;
        }
        case LAUNCH_PVO:
        {
            cout << "Time: "<< sim_time << ". Air defense missle launched.\n";
            s300_launch(todo_task.l_id);
            float reloadtime = float(rand() % 200)/100.0+3;
            tasklist.push({GET_READY_S300, sim_time+reloadtime, todo_task.l_id, 0});
            task first_part = tasklist.get(todo_task.pid, FIRST_PART);
            float curr_dist = dist_to_target - (sim_time - first_part.a_time + (dist_to_target -5)/main_speed)*main_speed;
            if (curr_dist <= 75)
            {
                tasklist.push({TARGET_POINT, sim_time + curr_dist/(main_speed+s300_speed), todo_task.pid, todo_task.l_id});
            }
            else
            {
                tasklist.push({TARGET_POINT, sim_time+(curr_dist-75)/(main_speed+s300_speed), todo_task.pid, todo_task.l_id});
            }
            break;
        }
        case TARGET_POINT:
        {
            task tmp_task = tasklist.get(todo_task.pid, FIRST_PART);
            if (tmp_task.a_time >= todo_task.a_time)
            {
                int rand_n = rand() % 100;
                if (rand_n <= 85)
                {
                    tasklist.remove(todo_task.pid, FIRST_PART);
                    cout << "Time: "<< sim_time << ". Air defense hit the target.\n";
                    downed_missiles++;
                }
                else 
                {
                    cout << "Time: "<< sim_time << ". Air defense missile failed to hit the target.\n";
                    missed++;
                }
            }
            else cout << "Time: "<< sim_time << ". Air defense missile self-destructed.\n";
            s300_freetarget(todo_task.l_id);
            break;
        }
        case GET_READY_S300:
        {
            s300_get_ready(todo_task.pid);
            break;
        }
    }
}


int main(int argc, char *argv[])
{
    srand(time(0));
	int opt = 0;
	char* endptr;
	static const char* sOpts = "i:s:d:";
	static const struct option lOpts[] = {
		{"iskander", required_argument, nullptr, 'i'},
		{"s300", required_argument, nullptr, 's'},
		{"distance", required_argument, nullptr, 'd'},
		{nullptr, 0, nullptr, 0},
	};

    while ((opt = getopt_long(argc, argv, sOpts, lOpts, nullptr)) != -1)
        {
            switch (opt)
            {
                case 0:
                    break;

                case 'i':
                    
                    iskanders = strtoul(optarg, &endptr, 10);
                    if (*endptr != '\0')
                    {
                        cerr << "Number of iskanders must be a positive integer.\n";
                        return EXIT_FAILURE;
                    }
                    if (iskanders <= 0)
                    {
                        cerr << "Number of iskanders must be a positive number.\n";
                        return EXIT_FAILURE;
                    }


                    break;

                case 's':
                    s300_n = strtoul(optarg, &endptr, 10);

                    if (*endptr != '\0')
                    {
                        cerr << "Number of s300 must be a positive number.\n";
                        return EXIT_FAILURE;
                    }

                    if (s300_n < 1 or s300_n > 4)
                    {
                        cerr << "Number of s300 must be a positive number and < 4.\n";
                        return EXIT_FAILURE;
                    }

                    break;
                
                case 'd':
                    dist_to_target = stof(optarg);

                    if (dist_to_target <= 5)
                    {
                        cerr << "Distance must be a float and 5 < dist <= 2000.\n";
                        return EXIT_FAILURE;
                    }

                    break;

                case '?':
                default:
                    help();
                    return EXIT_SUCCESS;
            }
        }

        if (optind < argc)
        {
            help();
            return EXIT_FAILURE;
        }

    cout << "------------------------------------------------------------\n"
		 << "                    START OF SIMULATION\n"
		 << "------------------------------------------------------------\n"
		 << "Running with " << iskanders << " number of iskanders.\n"
		 << "Running with " << s300_n << " number of s300.\n"
         << "Running with " << dist_to_target << "km. distance to target.\n"
		 << "------------------------------------------------------------\n"
		<< endl;
    ended = iskanders*2;
    iskander_amm = ended;
    iskander_ready = iskanders;
    sim_time = 0;
    task curr_task;
    if (dist_to_target < 175) detect_time = 0;
    else detect_time = (dist_to_target - 175)/main_speed;
    for(int i=0; i < s300_n; i++)
    {
        for(int j=0; j<3; j++)
        {
            syst[i].launchers[j].ammo=4;
            syst[i].launchers[j].ready=4;
        }
        syst[i].targets=6;
    }
    for(pid; pid < iskanders+12; pid++)
    {
        tasklist.push({LAUNCH_ISK, 0, pid, -1});
    }

    while (ended != 0)
    {
        if (!tasklist.empty())
        {
            curr_task = tasklist.get();
            sim_time = curr_task.a_time;
            execute(curr_task);
        }
        else break;
    }

    cout << "------------------------------------------------------------\n"
		 << "                 RESAULTS OF SIMULATION\n"
		 << "------------------------------------------------------------\n"
		 << "Target hits: " << target_hits << "\n"
		 << "Downed missles: " << downed_missiles << "\n"
		 << "------------------------------------------------------------\n"
		<< endl;
}
