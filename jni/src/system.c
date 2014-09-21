#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <paths.h>
#include <sys/wait.h>

extern char **environ;

int system(const char *command)
{
  pid_t pid;
        sig_t intsave, quitsave;
        sigset_t mask, omask;
        int pstat;
        char *argp[] = {"sh", "-c", NULL, NULL};

        if (!command)           /* just checking... */
                return(1);

        argp[2] = (char *)command;

        sigemptyset(&mask);
        sigaddset(&mask, SIGCHLD);
        sigprocmask(SIG_BLOCK, &mask, &omask);
        switch (pid = vfork()) {
        case -1:                        /* error */
                sigprocmask(SIG_SETMASK, &omask, NULL);
                return(-1);
        case 0:                         /* child */
                sigprocmask(SIG_SETMASK, &omask, NULL);
                execve("/system/bin/sh", argp, environ);
    _exit(127);
  }

        intsave = (sig_t)  bsd_signal(SIGINT, SIG_IGN);
        quitsave = (sig_t) bsd_signal(SIGQUIT, SIG_IGN);
        pid = waitpid(pid, (int *)&pstat, 0);
        sigprocmask(SIG_SETMASK, &omask, NULL);
        (void)bsd_signal(SIGINT, intsave);
        (void)bsd_signal(SIGQUIT, quitsave);
        return (pid == -1 ? -1 : pstat);
}
