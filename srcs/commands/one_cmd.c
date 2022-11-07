/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendonc <dmendonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 07:30:34 by anfreire          #+#    #+#             */
/*   Updated: 2022/11/07 21:23:22 by dmendonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header.h"

void	run_one_cmd(t_data *data, int in_fd, int out_fd)
{
	if (acessing_cmd(data, 0) == 0)
	{
		printf("%s: command not found.\n", data->paths.path_cmd[0]);
		return ;
	}
	if (pipe(data->ids.pfd[0]) != 0)
	{
		printf("ERROR on piping.\n");
		exit (0);
	}
	data->ids.id[0] = fork();
	if (data->ids.id[0] == 0)
	{
		if (in_fd > 0)
			dup2 (in_fd, STDIN_FILENO);
		if (out_fd > 1)
			dup2 (out_fd, STDOUT_FILENO);
		execve (data->paths.path_cmd[0], data->cmd.cmdx[0], data->envp);
	}
	else
		run_one_father(data);
}

void	run_one_father(t_data *data)
{
	waitpid(data->ids.id[0], &data->andre.exit, 0);
	WEXITSTATUS(data->andre.exit);
	data->andre.exit = data->andre.exit / 256;
	free_cmds (data);
}
