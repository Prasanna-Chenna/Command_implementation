//implementing ps -el | grep pts | wc -l

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	int a1[2],a2[2];
	if(pipe(a1)==-1)
	{
		perror("pipe");
		exit(0);
	}
	if(pipe(a2)==-1)
	{
		perror("pipe2");
		exit(0);
	}
	int ret=fork();
	if(ret==-1)
	{
		perror("fork");
		exit(0);
	}
	if(ret)
	{
		int ret1=fork();
		if(ret1==-1)
		{
			perror("fork");
			exit(0);
		}
		if(ret1)
		{
			//in parent process
			close(a1[0]);	//closed read end of pipe
			close(a2[0]);
			close(a2[1]);
			dup2(a1[1],1); //duplicating fd1 (stdout) in write fd of a1
			execlp("ps","ps","-el",NULL);
		}
		else
		{
			//in child process
			close(a1[1]); //closed write end of 1st pipe
			dup2(a1[0],0); //duplicating stdiin to readend of the pipe

			close(a2[0]); //closed readend of 2nd pipe
			dup2(a2[1],1); //duplicating writeend with 1(stdout)

			execlp("grep","grep","pts",NULL);
		}
	}
	else
	{		//in child1 process
		close(a2[1]); //closed writeend of 2nd pipe
		close(a1[0]);
		close(a1[1]);
		dup2(a2[0],0); //dup with stdin to readend of 2nd pipe
		execlp("wc","wc","-l",NULL);
	}
}
