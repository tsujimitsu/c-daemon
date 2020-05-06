#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>

static volatile int sigterm = 0;
static void handle_sigterm(int sig) { sigterm = 1; }

int main(void) {
    openlog("daemon", LOG_PID, LOG_DAEMON);

    //ここで子プロセスを生成し親は終了
    if(daemon(0, 0) == 0) {
        syslog(LOG_INFO, "%s", "daemon started");
        signal(SIGTERM, handle_sigterm);

        /* write daemon pid to the file */
        FILE* pidfile = fopen("/var/run/daemon.pid", "w+");
        if (pidfile) {
            int pid = getpid();
            fprintf(pidfile, "%d\n", pid);
            fclose(pidfile);
        } else {
            syslog(LOG_ERR, "%s", "failed to write pid");
        }
        
        while(!sigterm) {
            syslog(LOG_INFO, "%s", "log_test:LOG_INFO");
            usleep(1000000);
        }

    } else {
        syslog(LOG_ERR, "%s", "log_test:LOG_ERR");
    }

    if (remove("/var/run/daemon.pid") == 0) {
        printf("Deleted successfully"); 
    } else {
        printf("Unable to delete the file"); 
    }

    closelog();
    syslog(LOG_INFO, "%s", "daemon stopped");
    return 0;
}


