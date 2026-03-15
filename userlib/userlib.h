#include "kernel/types.h"
#include "kernel/syscall.h"

/* io.c */
/*
 * Read a character from the input.
 * Parameters: None
 * Returns:
 *   - The read character.
 */
int getchar();

/*
 * Write a character to the output.
 * Parameters:
 *   - ch: The character to write.
 * Returns: None
 */
void putchar(int ch);

/*
 * Formatted output to the console.
 * Supports %d, %x, %p, %s.
 * Parameters:
 *   - format: Format string.
 *   - ...: Arguments matching the format specifiers in format.
 * Returns: None
 */
void printf(char *format, ...);

/* syslib.c */
/*
 * Write to a port.
 * Parameters:
 *   - port: The port number to write to.
 *   - buffer: The buffer containing the data to write.
 *   - count: The number of bytes to write.
 * Returns:
 *   - The number of bytes written, or -1 on error.
 */
int port_write(int port, char *buffer, int count);

/*
 * Read from a port.
 * Parameters:
 *   - port: The port number to read from.
 *   - buffer: The buffer to store the read data.
 *   - count: The number of bytes to read.
 * Returns:
 *   - The number of bytes read, or -1 on error.
 */
int port_read(int port, char *buffer, int count);

/*
 * Acquire a port for exclusive use.
 * Parameters:
 *   - port: The port number to acquire. -1 causes it 
 *           to allocate the next available port.
 * Returns:
 *   - Port number on success, or -1 on error.
 */
int port_acquire(int port);

/*
 * Release a previously acquired port.
 * Parameters:
 *   - port: The port number to release.
 * Returns:
 *   - 0 on success, or -1 on error.
 */
int port_close(int port);

/*
 * Create a new process by cloning the current one.
 * Parameters: None
 * Returns:
 *   - The PID of the new process
 */
int clone();

/*
 * Load an ELF binary into memory and execute it, replacing the
 * current process's memory.
 * Parameters:
 *   - bin: A pointer to the ELF binary.
 *   - size: The size of the binary.
 * Returns:
 *   - Does not return on success, or -1 on failure.
 */
int load_elf(void *bin, int size);

/*
 * Get the current process ID.
 * Parameters: None
 * Returns:
 *   - The PID.
 */
int getpid();

/*
 * Get the size of the current process's memory.
 * Parameters: None
 * Returns:
 *   - The size of the process's memory.
 */
int getsize();

/*
 * Change the size of the current process's memory.
 * Parameters:
 *   - new_size: The new size.
 * Returns:
 *   - The new size of the process.
 */
int resize(int new_size);

/*
 * Terminate the specified process.
 * Parameters:
 *   - pid: The PID of the process to terminate.
 * Returns:
 *   - 0 on success, or -1 on failure.
 */
int terminate(int pid);

/*
 * Get the status of the specified process.
 * Parameters:
 *   - pid: The PID of the process.
 * Returns:
 *   - The status code, or -1 on error.
 */
int status(int pid);

/* syscall.s */
/*
 * Make a system call.
 * Parameters:
 *   - syscall_num: The system call number.
 *   - ...: Additional arguments for the system call.
 * Returns:
 *   - The result from the system call.
 */
uint64 syscall(int syscall_num, ...);
