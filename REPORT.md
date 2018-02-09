# Ecs150 libuthread Winter 2018
## Written by Joel Moens and John Tracy

For this project we developed a user level thread library that can be used in
a program via API calls to create threads and run multiple functions
concurrently. In order to create this functioning uthread library, we first
had to create a non type specific FIFO queue that would hold the pointers to
memory of the thread blocks that are currently waiting to be run. Once the 
queue was created, we then could use it inside uthread.c in order to keep
a priority queue of threads made by libuthread. Context.h was used to create
new contexts for new threads (which gives us registers and a stack pointer for
the new context). The thread blocks we created needed to be pointing to these
contexts and their respective stack pointers. Threads also need to keep track
of their current state (Ready, Running, Blocked, Zombie) for yielding purposes
(Non ready states should be put back onto the queue). Once the basic uthread.c 
was created, we then finalized the uthreadjoin function which Blocks all
parent processes until their children have completed and need to be collected.
Up until this point the threads were running in a cooperative model, meaning
the threads continue to run until they have yielded or been blocked. The final
phase of the project led us to allow the preemptive model for our threads.
We use a signal handler and a timer which allows the machine to switch context
and control to other ready threads in the queue without the running thread
yielding or being blocked. 

## Data Structures
###We created two structs to better organize and process these commands:
1. Node Struct
...This struct allows for the creation of a linked list of nodes that each
store a void * address to the node's data. The node * next allows the node to
be linked to another node and so on. All node declarations are node * because
we want to use the node * outside of our source c file.
2. Queue Struct
...This struct simply stores the size of the queue, as well as a node pointer
for the head and the tail of the queue. On the very first enqueue of the 
queue, the queue actually assigns the new node with the head and the tail of 
the queue. After this first assignment, though, only the tail is changed when 
another enqueue occurs. All queue declarations are actually queue_t (queue 
pointer) initializations throughout the code.
3. Thread_Block Struct
...The purpose of this struct is to store control block for each thread 
created by the libuthread library. A control block for a thread requires the 
thread's id, return value, state, stack pointer, store of registers, as well
as a threadblock pointer to the current thread's parent, and a threadblock
pointer to the current thread's children to collect during a join. The address
of all created user threads is then stored in uthread.c. The running thread is
stored outside of the queue because it is in use, and all other threads are 
waiting in the queue.

## Primary Functions
###We have 4 primary functions that implement the shell

1. Queue Functions
... Most of the queue functions are pretty straight-forward in implementation.
Queuecreate allocates memory for a queue and returns the pointer, Queue
enqueue pushes a new item onto the tail of the queue (and head if size = 0).
Queuedequeue pops off queue's head and returns the data stored in the node.
Queuedelete iterates through queue using the linked list to find and delete
an item. Queuelength returns queue->size, and Queuedestroy frees the queue 
if it is empty. Queueiterate was the only tricky function to implement, but we
soon realized that we are simply iterating through the queue and calling func
with the given arguments. We give the function the whole queue as well as the
specific data of the current node being iterated in the queue. 

2. Uthread Functions
... The Uthread functions were at first very confusing to our group. Once we
realized how much uthread.c relied on ucontext.h and context.c calls, we began
to understand how to complete the uthread_create function. We first created a
uthread_init function that would initialize the Main 0 thread. The function
then creates the first user thread we then intialize its context and enqueue.
To simplify our code for reusability we also created a uthread_malloc and 
uthread_free functions, which allocates or frees memory for the thread block.
We also created a uthread_ready function, which iterates through the queue and
returns the thread nearest the front of the queue that is ready to run. 
We use uthread_ready in our uthread_yield function to return the next thread
to switch contexts and switch this thread's state to running. Our uthread_exit 
function is called whenever a thread has completed it's given function with a 
return value.

3. Preempt Functions
... These functions were very simple to implement and easy to use. Basically, 
start initializes preempt as enabled which assigns a pause function which just 
yields to the next avaliable thread. Then start enables a virtual timer and 
starts it. Disable simply changes the handler function for SIGVTALRM to NULL. 
Preempts are disabled whenever the uthread library is manipulating threads or 
switching contexts and enabled when context is restored to thread functions.


## Testing
###Most of our testing was done by hand.
1. Queue Testing
... TestQueue.c was our testfile for testing all queue functions. It began
simply by creating a queue, enqueueing the queue, dequeueing, and casting the
data to the expected data that was enqueued. This was to ensure that the data
being passed in enqueue was being returned correctly in dequeue. We didn't
think queuedelete() would need to be tested, but, after we modified the join 
function, we realized the function was not working as we thought it was. If
the item being searched for was at the head or tail, then delete worked fine,
but at any other point in the queue the data was being tampered. We realized
the linked list was actually being broken in this code during phase 3 and 
fixed it then.

2. Uthread Testing
... Uthread_Hello.c was our first testing file to truly get the ball rolling on
our uthreads. Inside of our uthread functions we set printf statements that 
output which thread was running, which was yielding, which was exiting, etc.
Testing the yield functionality was as simple as making sure that the yield 
test we were given still provides the right output of thread1 then 2 then 3 
with successive imporvements to join. Once that had passes we changed it to 
test when threads were not created in a cascading manner which still worked. 

3. Preempt Testing
... uthread_preempt.c has two threads which contain while true loops that will
never exit. The point of this test is to make sure that preempt will interupt 
this and switch context to a thread that is ready. We also wanted to make sure
that join worked properly and collected the return value for thread 2 the way
it is expected to. These tests worked and returned with a normal exit code. 







