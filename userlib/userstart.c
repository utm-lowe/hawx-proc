#include "userlib.h"

// Entry point and wrapper of all user programs
extern int main();

void user_start() {
    main();
    terminate(getpid());
}
