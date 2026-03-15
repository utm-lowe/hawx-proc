#include "expect.h"
#include <iostream>

int main()
{
    Expect e;
    e.setTimeout(60000);
    e.spawn("make qemu");
    e.expect("panic: All done! (for now...)", true);
    e.killProc();
}