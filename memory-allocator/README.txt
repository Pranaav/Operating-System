Applied first-fit algorithm

firstly i made a struct box which is the node of my free-list. it will have next element and previous and size of
that node and also the magic number which will help us to seeif valid pointer or not for our my_free function. 
then i initiate my free list first and then i initiate all 6 variables which will store heap_info variables. and 
then for
my_alloc : i am iterating from start and whenever i found a node whose size is required size then made that node 
allocated and remove from that free list and if the size of node( sizeofnode + headersize) is greater than the 
required size. we will split that node and made a new node as adjusted size and remove the previous node and adjust 
the new node in our free list.
my_free: i am checking first if pointer is in my 4096 range or not if yes then if it is header or not and if yes 
then if it is already freed or not then i am making a node with same address and iterate over free list and putting 
in between the free list as in increasing order according to our addresses of our pointer. now checkking if its 
previous node is able to merge our inserted node or not and if yes then i merge them as making size of previous 
node accordingly and also then checking if its next node is able to merge or not if yes then again merging setting 
nodes size accordingly.
my_clean : just calling munmap here
my_heapinfo : i am storing my variable in initialy and for max size setted it as it constant and then for current 
size and free memory and block allocated i am adjusting this while allocation memory and freeing memory i am 
calculating this in my my_alloc and my_free function as they call accordingly. now for smallest available and 
largest available chunk i made some obversations that when it can change whenever i call free and malloc then i 
found some case in which i don't have to traverse my free list again n again so i am making some case in that 
functions if this happen do this and if this then do this and there's case when we seeing it can change our f and e 
value then i put them as -1 so that whenever myheapinfo will call it will iterate his free list to find suitable 
answers;
