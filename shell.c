#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include<readline/readline.h> 
#include<readline/history.h> 
#define delim "\t\r\n\a"
void rmvChar(char* s, char c){
	int j = 0;
	int n = strlen(s);
	for (int i = 0; i < n; i++){
	        if (s[i] != c){
	            s[j++] = s[i];
		}
	} 
	s[j] = '\0';
}
void workingonread(char* worki){
	int position = 0;
	//char c;
	int w = 0;
	while(1){
		if(worki[position]==EOF||worki[position]=='\n'){
			worki[position] = '\0';
			rmvChar(worki, '"');
			return;
		}
		else{
			if(worki[position]=='"'){
				w++;
				if(w==2){
					w = 0;
				}
			}
			else{
				if(worki[position]==' '&&w==0){
				//if(strcmp(worki[position], ' ')==0&&w==0){
					worki[position] = '\n';
				}
			}
		}
		position++;
	}
	
}
void sigintHandler(){
	exit(0);
}
void pars(char** aftrwork, char* worki){
	char* one;
	one = strtok(worki, delim);
	int posi = 0;
	while(one){
		aftrwork[posi] = one;
		posi++;
		//printf("%d \n", posi);
		one = strtok(NULL, delim);
	}
	aftrwork[posi] = NULL;
}
int chandir(char** cdwork, char* curd){
	if((strcmp(cdwork[1], "~")==0)&&(cdwork[2]==NULL)){
		chdir(curd);
		if(chdir(curd)==-1){
			return 0;
		}
		else{
			return 1;
		}
	}
	int l =1;
	while(cdwork[l]!=NULL){
		if(chdir(cdwork[l])==-1){
			return 0;
		}
		l++;
	}
	return 1;
}
int main(){
	char* hist[5];
	char worki[1024];
	int qw = 0;
	int we = 0;
	char curd[1024];
	getcwd(curd, sizeof(curd));
	while(1){
		signal(SIGINT, sigintHandler);
    		char nowdi[1024];
		getcwd(nowdi, sizeof(nowdi));
		printf("MTL458:");
		if(strlen(nowdi)>=strlen(curd)){
			printf("~");
			for(int i=strlen(curd);i<strlen(nowdi);i++){
				printf("%c", nowdi[i]);
			}
			printf("$ ");
			fflush(stdout);
		}
		else{
			printf("%s", nowdi);
			printf("$ ");
			fflush(stdout);
		}
		fflush(stdout);
		fgets(worki, 1024, stdin);
		//printf("%shyyyyy\n", worki);
    		if(strlen(worki)!=0){
    			hist[qw] = strdup(worki);
    			qw = (qw+1)%5;
    			if(qw==0){
    				we = 1;
    			}
    		}
    		workingonread(worki);
    		
    		char pw[1024];
    		//printf("%s \n",worki);
    		strcpy(pw, worki);
		char** aftrwork = malloc(128 * sizeof(char*));
		pars(aftrwork, worki);
		//printf("%s \n",worki);
		/*printf("%s", aftrwork[0]);
		printf("\n");
		printf("%s", aftrwork[1]);
		printf("\n");
		printf("%s", aftrwork[2]);*/
		//fflush(stdout);
		char* che[2];
		che[0] = "cd";
		che[1] = "history";
		if((aftrwork[0]!=NULL)&&(strcmp(aftrwork[0], che[0])==0)){
			if(!chandir(aftrwork, curd)){
				printf("NO such file/directory found \n");
			}
		}
		else if((aftrwork[0]!=NULL)&&(strcmp(aftrwork[0], che[1])==0)){
			if(we==0){
				for(int i=0;i<qw;i++){
					printf("%s" , hist[i]);
					//printf("%d", i);
					fflush(stdout);
				}
				//fflush(stdout);
			}
			else{
				for(int i=0;i<5;i++){
					int w = (qw+i)%5;
					printf("%s" , hist[w]);
					fflush(stdout);
				}
			}
		}
		else{
			int rc = fork();
			if(rc==0){
				if((aftrwork[0]!=NULL)&&(execvp(aftrwork[0], aftrwork)==-1)){
					printf("error in argument\n");
					fflush(stdout);
				}
				exit(1);
			}
			if(rc>0){
				wait(NULL);
				//printf("complete\n");
				//fflush(stdout);
			}
		}
	}
}
