#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>

#define MAX_LEN_LINE    100
#define LEN_HOSTNAME	30


int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    
    while (true) {
        char *s;
        int len;
        char hostname[LEN_HOSTNAME + 1];
        memset(hostname, 0x00, sizeof(hostname));
        printf("\x1b[31musername: %s\n", getpwuid(getuid())->pw_name);

        gethostname(hostname, LEN_HOSTNAME);
        printf("\x1b[31mhostname: %s\n", hostname);
       
       printf("\x1b[36mYongsShell $ ");
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
        
        printf("[%s]\n", command);
        if(strcmp(command,"exit") == 0){                                                 /* exit*/
        	break;
        }
 
       
        if(strcmp(command,"pwd")==0){
	char buf[BUFSIZ];                                                                /* pwd */
	
	getcwd(buf,BUFSIZ);
	printf("%s\n",buf);
        }
        if(strcmp(command,"mkdir")==0){                                                 /* mkdir */
	char namef[] ="";
	printf("만들 폴더 이름을 입렵하시오: ");
	scanf("%s", namef);
	
	char strFolderPath[] = { "/mnt/c/Users/YSL/toyshell/" }; 
	
	strcat(strFolderPath,namef);
	
	int nResult = mkdir( strFolderPath,0755 );

	if( nResult == 0 )
	{
		printf( "폴더 생성을 성공했습니다.\n" );
	}
	else if( nResult == -1 )
	{
		perror( "폴더 생성을 실패했습니다.\n" );
	}
        }
        if(strcmp(command,"ls")==0){                                             /* ls */
	char * cwd = (char *)malloc(sizeof(char) * 1024);
        DIR * dir = NULL;
        struct dirent * entry = NULL;
        getcwd(cwd, 1024);
        if( (dir = opendir(cwd)) == NULL)
        {
                printf("current directory error\n");
                exit(1);
        }
        while( (entry = readdir(dir)) != NULL)
        {
                printf("%s\n", entry->d_name);
        }
        
    
        free(cwd);
        closedir(dir);
        return 0;
        }

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }
    return 0;
}
