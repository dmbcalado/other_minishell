/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendonc <dmendonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 16:53:40 by dmendonc          #+#    #+#             */
/*   Updated: 2022/11/07 21:42:19 by dmendonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header.h"

void	redirect_fds(int infd, int outfd)
{
	dup2 (infd, STDIN_FILENO);
	dup2 (outfd, STDOUT_FILENO);
}

void	run_processes(t_data *data, int index)
{
	data->ids.id[index] = fork();
	if (data->ids.id[index] == 0)
	{
		if (index == 0)
		{
			close (data->ids.pfd[index][0]);
			if (data->ids.inp_list[index] != STDIN_FILENO)
			{
				dup2 (data->ids.inp_list[index], STDIN_FILENO);
				data->redir.input_c++;
			}
			if (data->ids.outp_list[index] != STDOUT_FILENO)
			{
				dup2 (data->ids.outp_list[index], STDOUT_FILENO);
				data->redir.output_c++;
			}
			else
				dup2(data->ids.pfd[index][1], STDOUT_FILENO);
		}
		else if (index == data->cmd.cmd_nbr - 1)
		{
			close (data->ids.pfd[index - 1][1]);
			if (data->ids.inp_list[index] == STDIN_FILENO)
				dup2(data->ids.pfd[index - 1][0], STDIN_FILENO);
			else
			{
				dup2(data->ids.inp_list[index], STDIN_FILENO);
				data->redir.input_c++;
			}
			if (data->ids.outp_list[index] != STDOUT_FILENO)
			{
				dup2 (data->ids.outp_list[index], STDOUT_FILENO);
				data->redir.output_c++;
			}
		}
		else
		{
			close(data->ids.pfd[index - 1][1]);
			close(data->ids.pfd[index][0]);
			if (data->ids.inp_list[index] == STDIN_FILENO)
				dup2(data->ids.pfd[index - 1][0], STDIN_FILENO);
			else
			{
				dup2(data->ids.inp_list[index], STDIN_FILENO);
				data->redir.input_c++;
			}
			if (data->ids.outp_list[index] == STDOUT_FILENO)
				dup2(data->ids.pfd[index - 1][0], STDOUT_FILENO);
			else
			{
				dup2(data->ids.outp_list[index], STDOUT_FILENO);
				data->redir.output_c++;
			}
		}
		execve (data->paths.path_cmd[index], data->cmd.cmdx[index], data->envp);
	}
	else
		run_father(data, index);
}

void	run_father(t_data *data, int index)
{
	close (data->ids.pfd[index][1]);
	waitpid(data->ids.id[index], &data->andre.exit, 0);
	WEXITSTATUS(data->andre.exit);
	data->andre.exit = data->andre.exit / 256;
	if (data->ids.out_fd == STDOUT_FILENO)
		return ;
	else
		dup2 (data->ids.pfd[index][0], STDIN_FILENO);
}

void	pipes(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->cmd.cmd_nbr)
	{
		if (acessing_cmd(data, i) == 0)
		{
			printf ("%s: command not found.\n", data->paths.path_cmd[i]);
			return ;
		}
		if (pipe(data->ids.pfd[i]) != 0)
			return ;
		run_processes(data, i);
	}
	free_cmds(data);
}
