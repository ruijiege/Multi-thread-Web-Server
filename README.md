#Multi-thread Web Server
**author: Ruijie Ge**

>**DESCRIPTION:** myhttpd is a simple web server. It binds to a given port on the given address and waits for incoming HTTP/1.0 requests. It serves content from the given directory. That is, any requests for documents is resolved relative to this directory (the document root – by default, the directory where the server is running).

When starting myhttpd, the following are the possible options:

−d : Enter debugging mode. That is, do not daemonize, only accept one connection at a time and enable logging to stdout. Without this option, the web server should run as a daemon process in the background. Any argument value i.e. -d <number> will takes server to degugging mode. default is deamon mode.

−h : Print a usage summary with all options and exit.

−l file : Log all requests to the given file.

−p port : Listen on the given port. If not provided, myhttpd will listen on port 8080.

−r dir : Set the root directory for the http server to dir.

−t time : Set the queuing time to time seconds. The default should be 60 seconds.

−n threadnum: Set number of threads waiting ready in the execution thread pool to threadnum. The default should be 4 execution threads.

−s sched : Set the scheduling policy. It can be either FCFS or SJF. The default will be FCFS. Any value i.e. -s <number> will set the policy to SJF.

failure to provide one or more arguments to httpd will use the default values.

All http 1.0 standards are used for displaying the success and error messages.
