/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendonc <dmendonc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 06:49:28 by anfreire          #+#    #+#             */
/*   Updated: 2022/11/07 21:44:10 by dmendonc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

void	brain(t_data *data)
{
	int	i;

	i = -1;
	data->built.b_counter = 0;
	if (data->line == NULL || data->line[0] == '\0')
		return ;
	while (data->par_line[++i])
	{
		if (builtin_detector (data, data->par_line[i]) >= 0)
		{
			parse_builtin(data, i, data->built.b_counter);
			exec_builtin(data, i);
			data->built.b_counter++;
		}
		else if (cmd_detector(data, data->par_line[i]) == 1 && \
		data->paths.p_str != NULL)
		{
			braining_cmds(data);
			return ;
		}
	}
}

void	braining_cmds(t_data *data)
{
	int	c;
	int	cmds;

	c = 0;
	cmds = data->cmd.cmd_nbr;
	if (data->cmd.cmd_nbr == 1)
	{
		data->ids.id = (int *)malloc(sizeof(int));
		parse_cmd(data, 0);
		run_one_cmd(data, data->ids.inp_list[0], data->ids.outp_list[0]);
		return ;
	}
	else
	{
		data->ids.id = (int *)malloc(data->cmd.cmd_nbr * sizeof(int));
		while (cmds > 0)
		{
			parse_cmd (data, c);
			cmds--;
			c++;
		}
		pipes (data);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;

	if (argc < 1 || *argv == NULL)
		return (0);
	signal (SIGQUIT, sig_ignore);
	signal (SIGINT, sig_handler);
	get_envp (&data, envp);
	data.line = NULL;
	data.andre.exit = 0;
	while (1)
	{
		get_line(&data);
		data.andre.args = 0;
		data.par_line = parse_line(&data);
		get_paths(&data);
		if (data.paths.p_str != NULL)
		{
			parse_alloc(&data);
			if (redirect(&data) < 0)
				continue ;
			brain (&data);
			free_line_info (&data);
		}
	}
	return (0);
}
