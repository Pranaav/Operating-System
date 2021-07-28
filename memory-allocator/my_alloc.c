#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>

struct box{
	int size;
	struct box *next;
	struct box *prev;
	int magic;
};

struct box *nod;
void *strt;
int aq, bq, cq, dq, eq, fq;

// Implement these yourself
int my_init(){
	nod = (void*)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
	strt = (void*) nod;
	if(nod==MAP_FAILED){
		return -1;
	}
	nod->size = 4096 - sizeof(struct box)- (6*sizeof(int));
	nod->magic = 69;
	aq = 4096 - (6*sizeof(int));
	bq = sizeof(struct box);
	cq = nod->size;
	dq = 0;
	eq = nod->size;
	fq = nod->size;
	return 0;
}

void my_split(int size, struct box *curr){
	if(fq==(curr->size)){
		fq = -1;
	}
	struct box *new = (void*)((void*)curr + size + sizeof(struct box));
	new->size = curr->size - size - sizeof(struct box);
	curr->magic = 12345;
	new->next = curr->next;
	new->prev = curr->prev;
	new->magic = 69;
	curr->size = size;
	if((new->next!=NULL)){
		new->next->prev = new;
	}
	if((new->prev!=NULL)){
		new->prev->next = new;
	}
	else{
		nod = new;
	}
	bq = bq + (size + sizeof(struct box));
	cq = cq - size - sizeof(struct box); 
	if(eq>(new->size)){
		eq = new->size;
	}
	return;
}

void* my_alloc(int size){
	// Replace malloc with your functionality
	struct box *curr;
	void *answ;
	answ = NULL;
	curr = nod;
	if(size%8!=0){
		return answ;
	}
	while(curr!=NULL){
	
		if(curr->size==(size)){
			if((curr->prev!=NULL)){
				curr->prev->next = curr->next;
			}
			else{
				nod = curr->next;
			}
			if((curr->next!=NULL)){
				curr->next->prev = curr->prev;
			}
			curr->magic = 12345;
			answ = (void *)((void *)(curr) + sizeof(struct box));
			dq++;
			bq += size;
			cq -= size;
			if(fq==size){
				fq = -1;
			}
			if(eq==size){
				eq = -1;
			}
			break;
		}
		else if((curr->size)>(size + sizeof(struct box))){
			//printf("DEBUG???? %d   %d\n", curr, curr->prev);
			my_split(size, curr);
			answ = (void *)((void *)(curr) + sizeof(struct box));
			dq++;
			break;
		}
		else{
			answ = NULL;
		}
		curr = curr->next;
	}
	return answ;
}

void my_free(void *ptr){
	// Replace free with your functionality
	if(ptr==NULL){
		return;
	}
	if((ptr<=((void*)((void*)(strt) + 4096)))&&(ptr>=(void*)(strt))){
		struct box *new = (void*)((void*)ptr - sizeof(struct box));
		if(new->magic!=12345){
			return;
		}
		dq--;
		bq -= new->size;
		cq += new->size;
		new->magic = 69;
		struct box* curr= nod;
		if(curr==NULL){
			nod = new;
			curr = new;
		}
		else{
			if((void*)new<(void*)curr){
				if((void*)((void*)new + sizeof(struct box) + new->size)==(void*)(curr)){
					if(curr->size==eq){
						eq = -1;
					}
					new->size = curr->size + new->size + sizeof(struct box);
					new->next = curr->next;
					new->prev = NULL;
					if(curr->next!=NULL){
						curr->next->prev = new;
					}
					curr = new;
					bq -= sizeof(struct box);
					cq += sizeof(struct box);
				}
				else{
					new->next = curr;
					curr->prev = new;
					new->prev = NULL;
					curr = new;
				}
				nod = new;
				
			}
			else{
				while(curr!=NULL){
					if(curr->next==NULL){
						if((void*)((void*)curr + sizeof(struct box) + curr->size)==(void*)(new)){
							if(curr->size==eq){
								eq = -1;
							}
							curr->size = curr->size + new->size + sizeof(struct box);
							bq -= sizeof(struct box);
							cq += sizeof(struct box);
						}
						else{
							new->next = NULL;
							curr->next = new;
							new->prev = curr;
							curr = new;
						}
						break;
					}
					else{
						if(((void*)curr<(void*)new)&&((void*)(curr->next)>(void*)new)){
							if((void*)((void*)curr + sizeof(struct box) + curr->size)==(void*)(new)){
								if(curr->size==eq){
									eq = -1;
								}
								curr->size = curr->size + new->size + sizeof(struct box);
								bq -= sizeof(struct box);
								cq += sizeof(struct box);
							}
							else{
								new->next = curr->next;
								new->next->prev = new;
								curr->next = new;
								new->prev = curr;
								curr = new;
							}
							if((void*)((void*)curr + sizeof(struct box) + curr->size)==(void*)(curr->next)){
								if(curr->next->size==eq){
									eq = -1;
								}
								curr->size = curr->size + curr->next->size + sizeof(struct box);
								curr->next = curr->next->next;
								if(curr->next!=NULL){
									curr->next->prev = curr;
								}
								bq -= sizeof(struct box);
								cq += sizeof(struct box);
							}
							break;
						}
					}
				}
			}
		}
		if((fq!=-1)&&(curr->size>fq)){
			fq = curr->size;
		}
		if(curr->size<eq){
			eq = curr->size;
		}
	}
	return;
}

void my_clean(){
	munmap((void*)nod, 4096);
	return;
}

void my_heapinfo(){
	int a, b, c, d, e, f;
	a = aq;
	b = bq;
	c = cq;
	d = dq;
	e = eq;
	f = fq;
	if(f==-1||e==-1){
		f = 0;
		e = 0;
		struct box *curr = nod;
		while((curr)){
			if(curr->size>f){
				f = curr->size;
			}
			if((curr->size<e)||(e==0)){
				e = curr->size;
			}
			curr = curr->next;
		}
	}
	fq = f;
	eq = e;
	
	// Do not edit below output format
	printf("=== Heap Info ================\n");
	printf("Max Size: %d\n", a);
	printf("Current Size: %d\n", b);
	printf("Free Memory: %d\n", c);
	printf("Blocks allocated: %d\n", d);
	printf("Smallest available chunk: %d\n", e);
	printf("Largest available chunk: %d\n", f);
	printf("==============================\n");
	// Do not edit above output format
	return;
}
