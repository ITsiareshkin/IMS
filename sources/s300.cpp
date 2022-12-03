#include "../headers/s300.hpp"

int s300_ready()
{
    for(int i=0; i < s300_n; i++)
    {
        if (syst[i].targets > 0)
            for(int j=0; j<3; j++)
            {
                if (syst[i].launchers[j].ready > 0 and syst[i].launchers[j].ammo > 0)
                {
                    return 4*i+j;
                }   
            }
        else continue;
    }
    return -1;
}

void s300_launch(int id)
{
    syst[id/4].launchers[id%4].ammo-=2;
    syst[id/4].launchers[id%4].ready=0;
}

void s300_taketarget(int id)
{
    syst[id/4].launchers[id%4].ready=0;
    syst[id/4].targets--;
}

void s300_freetarget(int id)
{
    syst[id/4].targets++;
}

void s300_get_ready(int id)
{
    syst[id/4].launchers[id%4].ready++;
}