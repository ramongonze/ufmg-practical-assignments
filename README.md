# threads-library

The goal of this assignment is to implement a thread library for Linux. The threads are "users threads", that is, is not true threads of kernel. It has the basic operations:

 *init*: Initializes any state necessary for the threadling library. It gets the first function to run (usually the main function of a program).
 
 *thread_create*: Creates a new thread.
 
 *yield*: Remove a thread from CPU.
 
 *exit*: Terminates the current thread.
 
 *wait*: Stops a thread until another thread terminates.
 
 *sleep*: Sleeps a thread for a period of time.

The assignment was based in a skeleton source code provided by the subject's professor, and it is available on  [Google Docs][1].

 [1]: https://docs.google.com/document/d/1kxpc6tS2NrmshUJPpY2xp7LXjRkCbPozounOQ5cfCwU/edit#

