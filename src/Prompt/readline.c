#include "../minishell.h"

#define STDOUT_SIMPLE O_RDWR|O_CREAT|O_TRUNC
#define STDOUT_DOUBLE O_RDWR|O_CREAT|O_APPEND

int redir_out(char **cmdvector){
	int i;
	int fd;
	int flag;	// >발견시 1, >>발견시 2값을 저장한다.

	// 리다이렉션 존재여부 판단
	for(i=0;cmdvector[i]!=NULL;i++){
		if(!strcmp(cmdvector[i], ">"))
		{
			flag = 1;
			break;
		}
		if(!strcmp(cmdvector[i], ">>"))
		{
			flag = 2;
			break;
		}
	}

	if(cmdvector[i]){
		if(!cmdvector[i+1]) return -1;
		else{
			if(flag == 1)
				fd = open(cmdvector[i+1], STDOUT_SIMPLE, 0644);
			else
				fd = open(cmdvector[i+1], STDOUT_DOUBLE, 0644);
			if (fd == -1)
			{
				perror(cmdvector[i+1]);
				return -1;
			}
		}
		dup2(fd, 1);    // 표준출력의 방향은 1 -> fd
		close(fd);
		for(;cmdvector[i+2]!=NULL;i++){
			cmdvector[i] = cmdvector[i+2];  // 리다이렉션기호, 파일명 토큰제거
		}
		cmdvector[i] = NULL;
	}
	return 0;
}

void parse_line(char *line, char **env)
{
    //static const char tok[] = " \t\n";
    //char *arg[100]; // MAXARG 명시필요.
    char **arg;
    //char *str;
    //char *save;
    //int argv;
    int pid;
    //int stdin_fd = dup(0); // 표준입력
    int stdout_fd = dup(1); // 표준출력
    //int temp_fd = 1;    // 표준출력 임시fd값저장.

    // argv = 0;
    // str = strtok_r(line, tok, &save);
    // while (str != NULL)
    // {
    //     arg[argv++] = str;
    //     str = strtok_r(NULL, tok, &save);
    // }
    // arg[argv] = (char *)0;
    arg = ft_split(line, ' ');

    redir_out(arg);
<<<<<<< HEAD
    // [DEBUG]리다이렉트 처리 후 실행인자
    for(int i=0;i<4;i++){
        printf("[%d]%s\n", i, arg[i]);
    }
=======
>>>>>>> 64a8fd66ef0fb3be415aaf4188597369acaea3b5

	// check_builtIn : 빌트인명령이라면 부모프로세스에서 수행됩니다.
	if (execute_builtin(arg[0], arg, env) == 0)
	{
		dup2(stdout_fd, 1);
		return ;
	}

	// 자식프로세스 생성
    if ((pid = fork()) == -1) printf("FORK ERROR\n");
    else if (pid != 0)
	{
		// parent's process
        pid = wait(NULL);
    }
    else
	{
		// child's process
		printf("I think this command is not built-in!\n");
		execute(arg[0], arg, env);
        printf("AFTER execute\n");
    }

    // 표준출력을 원래대로 되돌린다.
    dup2(stdout_fd, 1);
}

void read_line(t_list *env_list, char *line, char **envv)
{

    char *info = ft_colorstr(get_node(env_list, get_position(env_list, "PWD"))->data);
    line = readline(info);
    free(info);
    if (line == NULL)
        exit(0);

    //printf("%s\n", line);
    parse_line(line, envv);

    add_history(line);
    free(line);
    line = NULL;
}
