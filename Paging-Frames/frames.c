#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
int main(int argc, char **argv){
	//printf("weiufbnwv");
	int page[1048576];
	memset(page, -1, 1048576*sizeof(page[0]));
	FILE *filer;
	int *ptr;
	//printf("hellooooooooooooo");
	char *access;
	ptr = (int*)(malloc(sizeof(int)));
	access = (char*)(malloc(sizeof(char)));
	char buff[100];
	int frame = atoi(argv[2]);
	//char *method = argv[3];
	filer = fopen(argv[1] , "r");
	int len = 0;
	while(fgets(buff, 100, filer)!=NULL){
		len++;
		ptr = realloc(ptr, len*sizeof(int));
		access = realloc(access, len*sizeof(char*));
		//int conv = conver(buf);
		char *spl = strtok(buff, "  ");
		int conv = strtol(spl, NULL, 0);
		spl = strtok(NULL, "  ");
		conv = conv>>12;
		ptr[len-1] = conv;
		access[len-1] = spl[0];
		//len++;
	}
	fclose(filer);
	//printf("hellooooooooooooo");
	int numbfault = 0;
	int numwrite = 0;
	int nummiss = 0;
	int numhit = 0;
	int numdrop = 0;
	bool verb = false;
	if(argc==5){
		if(strcmp(argv[4],"-verbose")==0){
			verb = true;
		}
	}
	//len = sizeof(ptr)/sizeof(int);
	int numacess = len;
	int qw = 0;
	int fil[frame];
	char filacc[frame];
	if(strcmp(argv[3], "OPT")==0){
		for(int i=0;i<len;i++){
			//printf("hellooooooooooooo325171264");
			int vpn = ptr[i];
			
			if(page[vpn]==-1){
			//printf("hellooooooooooooo");
				nummiss++;
				if(qw==frame){
					numbfault++;
					int farthest = 0;
					int ind;
					for(int l=0;l<frame;l++){
						bool p = false;
						for(int j=i+1;j<len;j++){
							if(fil[l]==ptr[j]){
								if(j>farthest){
									farthest = j;
									ind = l;
								}
								p = true;
								break;
							}
						}
						if(!p){
							ind = l;
							break;
						}
					}
					if(filacc[ind]=='W'){
						if(verb){
							printf("Page 0x%05x was read from disk, page 0x%05x was written to the disk.\n", vpn, fil[ind]);
						}
						numwrite++;
					}
					else{
						if(verb){
							printf("Page 0x%05x was read from disk, page 0x%05x was dropped (it was not dirty).\n", vpn, fil[ind]);
						}
						numdrop++;
					}
					page[fil[ind]] = -1;
					page[vpn] = ind;
					fil[ind] = vpn;
					filacc[ind] = access[i];
				}
				else{
					page[vpn] = qw;
					fil[qw] = vpn;
					filacc[qw] = access[i];
					qw++;
				}
			}
			else{
				if(access[i]=='W'){
					filacc[page[vpn]] = access[i];
				}
			}
		}
	}
	else if(strcmp(argv[3], "FIFO")==0){
		int cyc = 0;
		//printf("hellooooooooooooo");
		for(int i=0;i<len;i++){
			//printf("hellooooooooooooo");
			int vpn = ptr[i];
			//printf("%d        %d\n", page[vpn], vpn);
			if(page[vpn]==-1){
			//printf("hellooooooooooooo");
				nummiss++;
				if(qw==frame){
					numbfault++;
					if(filacc[cyc]=='W'){
						if(verb){
							printf("Page 0x%05x was read from disk, page 0x%05x was written to the disk.\n", vpn, fil[cyc]);
						}
						numwrite++;
					}
					else{
						if(verb){
							printf("Page 0x%05x was read from disk, page 0x%05x was dropped (it was not dirty).\n", vpn, fil[cyc]);
						}
						numdrop++;
					}
					page[fil[cyc]] = -1;
					page[vpn] = cyc;
					fil[cyc] = vpn;
					filacc[cyc] = access[i];
					cyc = (cyc+1)%frame;
				}
				else{
					page[vpn] = qw;
					fil[qw] = vpn;
					filacc[qw] = access[i];
					qw++;
				}
			}
			else{
				if(access[i]=='W'){
					filacc[page[vpn]] = access[i];
				}
			}
		}
	}
	else if(strcmp(argv[3], "RANDOM")==0){
		srand(5635);
		for(int i=0;i<len;i++){
			int vpn = ptr[i];
			if(page[vpn]==-1){
				nummiss++;
				if(qw==frame){
					numbfault++;
					int ind = rand()%frame;
					if(filacc[ind]=='W'){
						if(verb){
							printf("Page 0x%05x was read from disk, page 0x%05x was written to the disk.\n", vpn, fil[ind]);
						}
						numwrite++;
					}
					else{
						if(verb){
							printf("Page 0x%05x was read from disk, page 0x%05x was dropped (it was not dirty).\n", vpn, fil[ind]);
						}
						numdrop++;
					}
					page[fil[ind]] = -1;
					page[vpn] = ind;
					fil[ind] = vpn;
					filacc[ind] = access[i];
				}
				else{
					page[vpn] = qw;
					fil[qw] = vpn;
					filacc[qw] = access[i];
					qw++;
				}
			}
			else{
				if(access[i]=='W'){
					filacc[page[vpn]] = access[i];
				}
			}
		}
	}
	else if(strcmp(argv[3], "LRU")==0){
		int ti = 0;
		//printf("weiufbnwv");
		int time[frame];
		
		for(int i=0;i<len;i++){
			ti++;
			int vpn = ptr[i];
			if(page[vpn]==-1){
				nummiss++;
				if(qw==frame){
					numbfault++;
					int ind = -1;
					int min = ti;
					for(int j=0;j<frame;j++){
						if(time[j]<min){
							min = time[j];
							ind = j;
						}
					}
					if(filacc[ind]=='W'){
						if(verb){
							printf("Page 0x%05x was read from disk, page 0x%05x was written to the disk.\n", vpn, fil[ind]);
						}
						numwrite++;
					}
					else{
						if(verb){
							printf("Page 0x%05x was read from disk, page 0x%05x was dropped (it was not dirty).\n", vpn, fil[ind]);
						}
						numdrop++;
					}
					page[fil[ind]] = -1;
					fil[ind] = vpn;
					//ret[cyc] = vpn;
					filacc[ind] = access[i];
					page[vpn] = ind;
					time[ind] = ti;
				}
				else{
					page[vpn] = qw;
					fil[qw] = vpn;
					filacc[qw] = access[i];
					time[qw] = ti;
					qw++;
				}
			}
			else{
				time[page[vpn]] = ti;
				/*numhit++;
				int j;
				for(j=0;j<frame;j++){
					if(ret[j]==vpn){
						break;
					}
				}
				while(true){
					if(cyc!=0){
						if(j==(cyc-1)){
							break;
						}
					}
					else{
						if(j==(frame-1)){
							break;
						}
					}
					if(j!=(frame-1)){
						ret[j] = ret[j+1];
					}
					else{
						ret[j] = ret[0];
					}
					j = (j+1)%frame;
				}
				if(cyc!=0){
					ret[cyc-1] = vpn;
				}
				else{
					ret[frame-1] = vpn;
				}*/
				if((access[i]=='W')){
					filacc[page[vpn]] = 'W';
				}
			}
		}
	}
	else if(strcmp(argv[3], "CLOCK")==0){
		int chan[frame];
		int cyc = 0;
		for(int i=0;i<len;i++){
			int vpn = ptr[i];
			if(page[vpn]==-1){
				nummiss++;
				if(qw==frame){
					numbfault++;
					//cyc = 0;
					while(true){
						if(chan[cyc]==0){
							if(filacc[cyc]=='W'){
								if(verb){
									printf("Page 0x%05x was read from disk, page 0x%05x was written to the disk.\n", vpn, fil[cyc]);
								}
								numwrite++;
							}
							else{
								if(verb){
									printf("Page 0x%05x was read from disk, page 0x%05x was dropped (it was not dirty).\n", vpn, fil[cyc]);
								}
								numdrop++;
							}
							page[fil[cyc]] = -1;
							page[vpn] = cyc;
							fil[cyc] = vpn;
							chan[cyc] = 1;
							filacc[cyc] = access[i];
							cyc = (cyc+1)%frame;
							break;
						}
						else{
							chan[cyc] = 0;
							cyc = (cyc+1)%frame;
						}
					}
				}
				else{
					page[vpn] = qw;
					fil[qw] = vpn;
					filacc[qw] = access[i];
					qw++;
				}
			}
			else{
				numhit++;
				chan[page[vpn]] = 1;
				if(access[i]=='W'){
					filacc[page[vpn]] = access[i];
				}
			}
		}
	}
	printf("Number of memory accesses: %d\n", numacess);
	printf("Number of misses: %d\n", nummiss);
	printf("Number of writes: %d\n", numwrite);
	printf("Number of drops: %d\n", numdrop);
}
