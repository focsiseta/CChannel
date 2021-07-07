Simple implementation of share memory by communicating don't communicate by sharing memory.
This is not even close to being done, but as it is it's usable.

As it is ch_read and ch_write are blocking (obv).
To send something you first need to wrap that something with wrap() and then you can send it, remember that it's usually advised to malloc what you are sending instead of allocating it on stack, weird things can happen if you don't.
//TODO impl multiple obj sending (figure)
//TODO impl ch_read and ch_write for non-blocking operations, useful if we have a pool of threads that may send/recv from 
multiple sources


