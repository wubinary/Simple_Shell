#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
//#include <CString>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
//CString str;
#define word_len 1000
#define sent_len 100
#define total_len 100000
//char input2[total_len];
void input_string(char *input)
{
	// 把>&前面加空白
	//printf("%d",strlen(input));
	int i,k;
	char c,input2[total_len];
	
	for(i=0,k=0; i<strlen(input); i++ ,k++)
	{
		c = *(input+i);
		if( c=='>' || c=='&' || c=='<' || c=='|')
		{
			input2[k]=' ';
			input2[k+1]=input[i];
			input2[k+2]=' ';
			k+=2;
		}
		else
		{
			input2[k]=input[i];
		}
	}
	strcpy(input, input2);
	return ;
}
void input_string_delim(char *input,char **input_delim)
{
	/// 使用者"換行" "\n"處理
	int k=0;
	while(1==1)
	{
		if(input[k]=='\n')
		{	
			input[k]='\0';
			break;
		}
		k++; 
	}
	//// 使用者"換行" "\n"處理
	fflush(stdout);
	
	/////////////////// "一維" 轉 "二維"
	int i=0;	
	char delim[10]=" \t\r\n";
	input_delim[0] = strtok(input, delim);
	//printf("input_delim%d : %s\n",i,input_delim[i]);
	while(input_delim[i] != NULL)
	{
		i++;
		input_delim[i] = strtok(NULL, delim);
		//printf("input_delim%d : %s\n",i,input_delim[i]);
	}
	////////////////// "一維" 轉 "二維"
}
void free_(char *input, char **input_delim)
{
	//free(input);
	for(int i=0; i<strlen(input); i++)
	{
		*(input+i)='\0';
	}

	int i=0;
	while(input_delim[i]!=NULL)
	{
		//free(input_delim[i]);
		input_delim[i]=NULL;
		i++;
	}
}
void mem_alloc(char *input)
{
	input = (char *)realloc(input, total_len*sizeof(char));
}
bool redirect(char **instruction, char *file)
{
	int i=0;
	bool isredirect=false;
	while(instruction[i]!=NULL)
	{
		//printf("afasd:%s\n",instruction[i]);
		if(strcmp(instruction[i],">")==0)
		{
			isredirect=true;
			break;
		}
		i++;
	}
	if(isredirect==true)
	{
		strcpy(file, instruction[i+1]);
		instruction[i]=NULL;
	}	
	/*
	///// 除錯
	printf("<<");
	for(int k=0; k<i; k++)
		printf("%s,",instruction[k]);
	printf("%d>>\n",i);
	fflush(stdout);
	///// 除錯
	printf("-{%s}-",file);
	fflush(stdout);
	///// 除錯
	*/
	
	return isredirect;
}
bool pipe_(char **instruction, char ***instruction2)
{
	int i=0;
	bool ispipe=false;
	while(instruction[i]!=NULL)
	{
		if(strcmp(instruction[i], "|")==0)
		{
			ispipe=true;
			break;
		}
		i++;
	}
	//printf("0");
	//fflush(stdout);
	if(ispipe==true)
	{
		*instruction2 = &instruction[i+1];
		instruction[i] = NULL;
	}
	//printf("\n{in:%s}{in2:%s}\n",instruction[1],instruction[2]);
	
	/*
	/// 除錯
	int g=0;
	while(instruction[g]!=NULL)
	{
		printf("{%s}\n",instruction[g]);
		g++;
	}
	printf("\n");
	g=0;
	while(instruction2[g]!=NULL)
	{
		printf("{%s}\n",instruction2[g]);
		g++;
	}
	/// 除錯
	*/


	//printf("1");
	//fflush(stdout);
	return ispipe;
}
void pipe_read(int fd,char *readBuffer)
{
	dup2(fd, 1);
}
void childSignalHandler(int signo)
{
	int status;
	while(waitpid(-1,&status,WNOHANG)>0);
	//exit(0);
}
void ppp(char **instruction, char **instruction2)
{
	int aaaa=0;
	while(instruction[aaaa]!=NULL)
	{
		printf("%s\n",instruction[aaaa]);
		aaaa++;
	}
	aaaa=0;
	printf("\n");
	while(instruction2[aaaa]!=NULL)
	{
		printf("insturction2[0]:");
		fflush(stdout);
		printf("%s\n",instruction2[aaaa]);
		aaaa++;
	}
	
	
	//pipe
	int fd[2];
	pid_t pid;
	pipe(fd);
	pid = fork();
	switch(pid)
	{
		case -1:
			printf("you go die\n");
			exit(-1);
		case 0:
			close(fd[0]);
			dup2(fd[1], 1);
			close(fd[1]);
			execvp(instruction[0], instruction);
			printf("^.^\n");
			exit(-1);
		default :
			close(fd[1]);
	}
	pid = fork();
	switch(pid)
	{
		case -1:
			printf("you go die\n");
			exit(-1);
		case 0:
			close(fd[1]);
			dup2(fd[0], 0);
			close(fd[0]);
			execvp(instruction2[0], instruction2);
			printf("^.^\n");
			exit(-1);
		default :
			close(fd[0]);
	}
	wait(NULL);
	wait(NULL);

}
void shell(char **instruction, int do_wait=0) 
{
	int status,fd[2];
	char file[100],**instruction2;
	bool ispipe=false,isredir=false;
	//signal(SIGCHLD, wait4children);
	///// if "sleep" && do_wait=true
	if(strcmp(instruction[0], "sleep")==0 && do_wait!=true)
		signal(SIGCHLD, childSignalHandler);
	if((isredir=redirect(instruction,file))==true)
	{
		int adfa;
	}
	else if((ispipe=pipe_(instruction,&instruction2))==true)
	{
		//ppp(instruction,instruction2);
		pipe(fd);
	}

	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		// error
		printf("{error while making child}");
		exit(-1);
	}
	else if(pid == 0)
	{
		// child
		if(isredir==true)
		{
			int fd=open(file,\
					O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			dup2(fd,1);
			close(fd);
			execvp(instruction[0], instruction);
			exit(0);
		}
		else if(ispipe==true)
		{
			close(fd[0]);
			dup2(fd[1],STDOUT_FILENO); //把stdout導到pipe(Write)
			close(fd[1]);
			execvp(instruction[0], instruction);
			exit(0);
		}
		else
		{
			execvp(instruction[0], instruction);
			printf("{ERROR Not exist:%s}\n",instruction[0]);
			fflush(stdout);
			exit(0);
		}
	}
	else
	{
		// parent
		if(ispipe==true)
		{
			close(fd[1]);
			waitpid(pid, &status, 0);
			pid_t pid2;
			pid2 = fork();
			if(pid2<0)
			{
				printf("{error}\n");
				exit(-1);
			}
			else if(pid2==0)
			{
				//child
				//close(fd[1]);
				dup2(fd[0], STDIN_FILENO); //stdin導向pipe(Read)
				close(fd[0]);
				execvp(instruction2[0], instruction2);
				exit(0);
			}
			else
			{
				int status2;
				//close(fd[0]);
				close(fd[0]);
				waitpid(pid2, &status2, 0);
			}
		}
		/// 非'sleep',非'pipe',要wait()
		if(strcmp(instruction[0], "sleep")!=0 && ispipe!=true)
			waitpid(pid, &status, 0);
		/// 'sleep'後面沒有'&',要wait()
		if(do_wait==1)
			waitpid(pid, &status, 0);
		return ;
	}
}
void run(char **input_delim)
{
	int i=0,tmp=0;
	char *instruction[sent_len];
	while(input_delim[i]!=NULL)
	{
		//instruction[tmp]=input_delim[i];
		if(strcmp(input_delim[i],"&")==0)
		{
			instruction[tmp]=NULL;
			
			/*
			///// 除錯
			printf("{");
			for(int k=0; k<tmp; k++)
				printf("%s,",instruction[k]);
			printf("%d}\n",tmp);
			fflush(stdout);
			///// 除錯
			*/

			shell(instruction);


			//  清空instruction
			for(int k=0; k<tmp; k++)
			{
				instruction[k]=NULL;
			}
			// 清空instruction
			tmp=0;
			i++;
		}
		if((instruction[tmp]=input_delim[i])==NULL )
			break;
		tmp++;
		i++;
	}
	instruction[tmp]=NULL;

	/*
	/// 除錯
	printf("{");
	for(int k=0; k<tmp; k++)
		printf("%s,",instruction[k]);
	printf("%d}}\n",tmp);
	fflush(stdout);
	/// 除錯
	*/

	if(instruction[0]!=NULL)
		shell(instruction,1);
}
int main()
{
	char *input=(char *)malloc(total_len*sizeof(char));
	char *input_delim[sent_len];
	printf(">");
	while(fgets(input,total_len,stdin))
	{
		if(strcmp(input,"exit\n")==0)
			exit(0);

		input_string(input);
		input_string_delim(input, input_delim);
		
		run(input_delim);
		free_(input, input_delim);
		
		fflush(stdout);
		printf(">");
	}
	return 0;
}
