/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ratinhosujo <ratinhosujo@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 21:31:48 by dmendonc          #+#    #+#             */
/*   Updated: 2022/11/08 14:57:48 by ratinhosujo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header.h"

//------------------------------------------------------------------------------
// args: i as par_line[i]
// index has in wich cmd index, flag has flag = 1 : </>
//										flag = 2 : <</>>
// task: sets all counters to zero,
//------------------------------------------------------------------------------

int	redirect(t_data *data)
{
	int	index;
	int	size;
	int i;
	int	flag_i;
	int	flag_o;
	int	ret;
	
	i = 0;
	index = -1;
	flag_i = 0;
	flag_o = 0;
	size = data->cmd.cmd_nbr + data->built.builtin_n;
	while(++index < size)
	{
		while(data->par_line[i])
		{
			ret = redir_detector (data, data->par_line[i]);
			if (ret == 1)
			{
				i++;
				break ;
			}
			if (ret > 1)
			{
				if (ret < 4 && flag_i == 0)
				{
					if(bridge_infiles(data, index, i) < 0)
						return(-1);
					if (ret == 2)
					{
						extract_input(data, index, i + 1);
						if(exec_redirect(data, index, i) < 0)
							return(-1);
					}
					else 
					{
						extract_hdockey(data, i + 1);
						heredoc(data, index);
					}
					flag_i++;
				}
				if ( ret > 3 && flag_o == 0)
				{
					i = find_i_for_outfile(data, index);
					if(bridge_outfiles(data, index, i) < 0)
						return(-1) ;
					extract_output(data, index, i + 1);
					if(exec_redirect(data, index, i) < 0)
						return(-1) ;
					flag_o++;
				}
			}
			i++;
		}
		flag_i = 0;
		flag_o = 0;
	}
	return (1);
}

//------------------------------------------------------------------------------
// args: i as par_line[i], index has in wich cmd index, flag has flag = 1 : </>
//																flag = 2 : <</>>
// task: reads the fd and if different then the standard, opens with the
// corresponding flags.
//------------------------------------------------------------------------------

int	exec_redirect(t_data *data, int index, int i)
{
	int save;

	save = redir_detector(data, data->par_line[i]);
	if (save == 2)
	{
		data->ids.inp_list[index] = open (data->redir.input[index], O_RDONLY);
		if(data->ids.inp_list[index] < 0)
		{
			printf("Error: the file %s does not exist.", data->redir.input[index]);
			return (-1);
		}	
	}
	if (save == 3)
	{
		data->ids.inp_list[index] = open (data->redir.input[index], O_RDONLY); // VER FLAGS HEREDOC
		if(data->ids.inp_list[index] < 0)
		{
			printf("Error: the file %s does not exist.", data->redir.input[index]);
			return (-1);
		}
	}
	if (save == 4)	//output
	{
		data->ids.outp_list[index] = open(data->redir.output[index], O_CREAT | O_TRUNC | O_RDWR, 0644);
		if(data->ids.outp_list[index] < 1)
		{
			printf("Error: the file %s had issues on open().",data->redir.output[index]);
			return (-1);
		}
	}
	if (save == 5)	//output
	{
		data->ids.outp_list[index] = open(data->redir.output[index], O_CREAT | O_TRUNC | O_RDWR); // VER FLAGS APPEND
		if(data->ids.outp_list[index] < 0)
		{
			printf("Error: the file %s had issues on open().",data->redir.output[index]);
			return (-1);
		}
	}
	return (0);
}