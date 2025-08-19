#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <fcntl.h>
#include "systemcalls.h"

#define STDOUT_FD (1)

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    int ret_val = system(cmd);
    if (ret_val < 0) {
        return false;
    }

    return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
        printf("__________%s__________\n", command[i]);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    pid_t pid, wait_pid;
    int ret_val;

    // Verify every command, if it not a flag
    for (i = 0; i < count; i ++) {
        if (command[i][0] != '-' && command[i][0] != '/') {
            printf("ERROR: the command's path should be a absolute path: [%s]\n", command[i]);
            return false;
        }
    }

    pid = fork();
    
    if (pid == 0) {
        printf("I'm a child process, executing the command %s....\n", command[0]);
        ret_val = execv(command[0], &command[0]);
        if (ret_val == -1) {
            printf("execv failed, exiting with error\n");
            exit(-1);
        }
    } else if (pid < 0) {
        printf("ERROR: fork failed with pid = %d\n", pid);
        return false;
    } else {
        printf("I'm a dad process, waiting for the child process finished...\n");
        wait_pid = wait(&ret_val);
        if (wait_pid != pid) {
            printf("Wait failed with wait_pid = %d\n", wait_pid);
            return false;
        } else {
            printf("Child executed the command successfully\n");
        }
    }

    va_end(args);

    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
        printf("__________%s__________\n", command[i]);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    pid_t pid, wait_pid;
    int ret_val;
    int fd;

    fd = open(outputfile, O_RDWR);
    if (fd == -1) {
        printf("ERROR: Open %s failed\n", outputfile);
        return false;
    }

    pid = fork();
    
    if (pid == 0) {
        printf("I'm a child process, executing the command %s....\n", command[0]);
        dup2(fd, STDOUT_FD);
        close(fd);
        ret_val = execv(command[0], &command[0]);
        if (ret_val == -1) {
            printf("execv failed, exiting with error\n");
            exit(-1);
        }
    } else if (pid < 0) {
        printf("ERROR: fork failed with pid = %d\n", pid);
        return false;
    } else {
        printf("I'm a dad process, waiting for the child process finished...\n");
        wait_pid = wait(&ret_val);
        if (wait_pid != pid) {
            printf("Wait failed with wait_pid = %d\n", wait_pid);
            return false;
        } else {
            printf("Child executed the command successfully\n");
        }
    }

    va_end(args);

    

    return ret_val;
}
