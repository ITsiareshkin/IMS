#ifndef S300_HPP
#define S300_HPP
struct launcher
{
    int ready;
    int ammo;
};

struct s300
{
    launcher launchers[3];
    int targets;
};

extern int s300_n;
extern s300 syst[4];

int s300_ready();
void s300_launch(int);
void s300_taketarget(int);
void s300_freetarget(int);
void s300_get_ready(int);

#endif //S300_HPP