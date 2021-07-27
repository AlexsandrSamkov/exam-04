#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define TYPE_END;
#define TYPE_PIPE;
typedef struct s_cmd
{
	char **args;
	int  type;
	int     fds[2];
	struct s_cmd *next;
	struct s_cmd *prev;
}   t_cmd;

t_cmd *add_cmd(t_cmd *cmd, char **args, int type)
{
	t_cmd *new = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		new->args = args;
		new->type = type;
		new->prev = 0;
		new->next = 0;
	}
	else
	{
		t_cmd *begin = cmd;
		while (begin->next)
			begin = begin->next;
		begin->next = new;
		new->prev = cmd;
		new->next = 0;
		new->args = args;
		new->type = type;
	}
	return (cmd);
}

void del_cmd(t_cmd *cmd)
{
	t_cmd *del;

	while(cmd)
	{
		del = cmd;
		cmd = cmd->next;
		free(del->args);
		free(del);
	}
}

void get_args(char *s, char **args)
{
	char *res;
	int i = 0;
	if (!args)
	{
	   res = malloc((char *) * 2)
	   res[0] = *s;
	   res[1] = 0;
	}
		while (args[i])
			i++;
		res = malloc((char *) * i + 2);
		i = 0;
		while (args[i])
		{
			res[i] = args[i];
			i++;
		}
		res[i++] = s;
		res[i] = 0; 
	}
	free(args);
	args = res;
}

t_cmd *parse(char **read_line)
{
	char **args = 0;
	int i = 0;
	t_cmd *cmd = 0;
	while (read_line[i])
	{
		while (read_line[i] && !strcmp(read_line[i],";") || !strcmp(read_line[i],"|"))
		{
			get_args(read_line[i],args);
			i++;
		}
		if (!strcmp(read_line[i],"|"))
			cmd = add_cmd(cmd,args,TYPE_PIPE);
		else
			cmd = add_cmd(cmd,args,TYPE_END);
		args = 0;	
		if (read_line[i])
			i++;
	}
	return(cmd);
}

void run(t_cmd *cmd,char **env)
{
	int pid = -1;
	int status;
	t_cmd *prev;
	while (cmd)
	{
		prev = cmds->prev;
		if (!cd)
		{
			if (cmd->type == TYPE_PIPE)
				pipe(cmd->fds);
			pid = fork();
			if (!pid)
			{
				if (cmd->type == TYPE_PIPE)
				{
					dup2(fds[1],1);
					close(fds[1]);
					close(fds[0]);
				}
				if (prev && prev == TYPE_PIPE)
				{
					dup2(fds[0],0);
					close(fds[0]);
					close(fds[1]);
				}
				execve(cmd->args[0],args,env);
			}
			else
				{
					if (pid != -1)
						waitpid(pid,&status,0);
					if (cmd->type == TYPE_PIPE)
						close(cmds->fds[1]);
					if (prev && prev == TYPE_PIPE)
						close(cmds->fds[0]);	
				}
		}
		cmd = cmd->next;
	}
	
}