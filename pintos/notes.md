File stuff
==========
Inodes are pointers to disk. Each process that opens a specific file should have the same inode for that file.

Reopen check is done in inode_open.

Does inode_open need to be locked

###
Does the inode_list_lock need to lock the whole open_cnt == 0 if stmt or just the list removal part in close

You probably need to lock the whole inode list throughout inode_close to avoid another thread getting a ptr
to an inode that is about to be deleted


