#include <getopt.h>
#include <iostream>
#include "../headers/tasks.hpp"
#include <stdlib.h>
#include <string.h>
using namespace std;

int iskanders, s300, ended, iskander_amm, iskander_ready, tracked_targets, pvo_amm, pvo_ready;
float dist_to_target, sim_time;
tasks tasklist;

void help()
{
	cerr << "Usage:\n"
		<< "\t-i or --iskander to specify the number of iskanders\n"
		<< "\t-s or --s300 to specify the number of s300\n"
		<< "\t-d or --distance to specify the distance to target in km\n";
}

void execute(task todo_task)
{
    switch(todo_task.task_name) {
        case FIRST_PART:
            cout << "Time: "<< sim_time << ". Missile near the target.\n";
            tasklist.push({SECOND_PART, sim_time + 5/2.6});
            break;
        case SECOND_PART:
            cout << "Time: "<< sim_time << ". The missile hit the target.\n";
            ended--;
            break;
        case RELOAD_ISK:
            cout << "Time: "<< sim_time << ". Iskander reloaded.\n";
            iskander_ready++;
            break;
        case DETECT_TARGET:
            cout << "Time: "<< sim_time << ". Target detected.\n";
            //TODO
            break;
    }
}


int main(int argc, char *argv[])
{
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
                    s300 = strtoul(optarg, &endptr, 10);

                    if (*endptr != '\0')
                    {
                        cerr << "Number of s300 must be a positive number.\n";
                        return EXIT_FAILURE;
                    }

                    if (s300 < 1 or s300 > 4)
                    {
                        cerr << "Number of s300 must be a positive number.\n";
                        return EXIT_FAILURE;
                    }

                    break;
                
                case 'd':
                    dist_to_target = stof(optarg);

                    if (dist_to_target <= 5)
                    {
                        cerr << "Distance must be a float and > 5.\n";
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
		 << "Running with " << s300 << " number of s300.\n"
         << "Running with " << dist_to_target << "km. distance to target.\n"
		 << "------------------------------------------------------------\n"
		<< endl;
    ended = iskanders*2;
    iskander_amm = ended;
    iskander_ready = iskanders;
    sim_time = 0;
    task curr_task;
    float detect_time;
    if (dist_to_target < 175)
        detect_time = 0;
    else
        detect_time = (dist_to_target - 175)/2.1;
    while (ended != 0)
    {
        if (!tasklist.empty())
        {
            curr_task = tasklist.pop();
            sim_time = curr_task.a_time;
            execute(curr_task);
            while(1)
            {
                if (!tasklist.empty())
                {
                    curr_task=tasklist.get();
                    if(curr_task.a_time == sim_time)
                    {
                        execute(curr_task);
                        tasklist.pop();
                    }
                    else break;
                }
                else break;
            }
        }
        while(iskander_amm != 0 and iskander_ready != 0)
        {
            iskander_amm--;
            iskander_ready--;
            tasklist.push({RELOAD_ISK, sim_time+60});
            tasklist.push({FIRST_PART, sim_time+(dist_to_target-5)/2.1});
            tasklist.push({DETECT_TARGET, sim_time+});
            cout << "Time: "<< sim_time << ". Launced iskander."<< "Left: "<< iskander_amm <<"\n";
        }
    }   
}
