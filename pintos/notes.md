Child-parent relations
======================

* `process_execute` starts `thread_create` passing the `start_process` function which is probably 
run when  the thread starts
* The new thread  can be scheduled before `thread_create` is run so the child-parent stuff needs to be set up
before somewhere in `thread_create`. Probably before the `thread_unblock` call
* The new thread is not `thread_current` in `thread_create`
