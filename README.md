Introduction
============
In this assignment, you will be implementing the proc subsystem which will
enable the kernel to load a user init program. This init program will
eventually be responsible for bringing up the user space component of
the kernel. For now, it will just start an idle daemon and then try
out some of the system calls. If you like, you can play with the code
in `user/init.c` to see what it's like programming for our system.

Remember that we do not have any notion of a filesystem in the HAWX
kernel, and so things will be a little bit different than they are in
xv6. We will be loading our init elf object from a binary string 
embedded as a BLOB in the kernel image. This image is the compiled
version of `user/init.c`. Note that `init.c` uses the functions in our
little `userlib` library. User programs in hawx begin execution in the
userlib's `user_start` function.

Procedure
=========
Go ahead and run
    make qemu
As expected, this does not work at all! You will need to add code to complete
the following functions in `proc.c`:
  - `proc_init`
  - `proc_load_user_init`
  - `proc_alloc`
  - `proc_free`
  - `proc_load_elf`
  - `proc_resize`
  - `proc_pagetable` 
  - `proc_free_pagetable`
  - `proc_loadseg`
  - `proc_vmcopy`

As always, these functions have analogous functions in xv6. The xv6
analogs can be found in:
  - `xv6-riscv/kernel/proc.c`
  - `xv6-riscv/kernel/exec.c`
  - `xv6-riscv/kernel/vm.c`
You will also want to pay attention to the kernel's header files.
These are your reference to the rest of the kernel!

This assignment will require you to understand the HAWX memory
functions in order to adapt the xv6 code. There are plenty of hints in
the comments for you to work with. For the most part, these functions
are all interrelated, so you will need to implement all of them in
order for this to work at all! In some cases, the deviation is mild,
but in others it will be quite significant. `proc_load_elf` is
possibly the hardest one. Once you get this to work properly, you will
have completed a major right of passage as a programmer! You'll also
have demonstrated that you understand the difference between our
micro-kernel and MIT's monolithic kernel. 

Now, before we can get anything to work, you need to implement the
scheduler in `scheduler.c`. You can do this by directly adapting the
xv6 scheduler, or you can get creative. The choice is yours! If you
play with other scheduling algorithms, I will give you extra credit.
Especially if you modify `init.c` in a way that demonstrates the
effectiveness of your scheduler. 

Once you get everything done, you should be able to run the system and
see something like this:

    $ make qemu
    HAWX kernel is booting
    
    Elf Loading...PASSED
    Daemons Started...PASSED
    Scheduler and Clone test...aaaaaaaaaabbbbbbbbbbamokamokamokamokamokamokamokamoka
    mokamokamok....PASSED

Note that your number of "amoks" may vary as will the ordering of your
a's and b's. That's determined by your processor speed and how your
process get scheduled. You should see all of this on the screen
though. If you think about it, you've had an operating system
simultaneously running 4 processes! That's pretty cool!