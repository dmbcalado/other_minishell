/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridges.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anfreire <anfreire@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 17:20:36 by ratinhosujo       #+#    #+#             */
/*   Updated: 2022/11/07 15:57:35 by anfreire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header.h"

int	bridge_infiles(t_data *data, int index, int last)
{
	int	count;
	int	i;
	int	ret;
	
	i = 0;
	count = -1;
	while(++count <= index)
	{
		while(data->par_line[i])
		{
			ret = redir_detector (data, data->par_line[i]);
			if (ret == 1)
				break;
			if (ret > 1)
			{
				if (ret < 4 && i != last)
				{
					if(open(data->par_line[i + 1], O_RDONLY) < 0)
					{
						printf("Error: the file %s does not exist.", data->par_line[i + 1]);
						return (-1);
					}
				}
			}
			i++;
		}
	}
	return (1);
}

int	bridge_outfiles(t_data *data, int index, int last)
{
	int	count;
	int	i;
	int	ret;
	
	i = 0;
	count = -1;
	while(++count <= index)
	{
		while(data->par_line[i])
		{
			ret = redir_detector (data, data->par_line[i]);
			if (ret == 1)
				break;
			if (ret > 1)
			{
				if (ret > 3 && i != last)
				{
					printf("creating garbage outfile\n");
					if(open(data->par_line[i + 1], O_CREAT | O_TRUNC , 0644) < 0)
						return (-1);
				}
			}
			i++;
		}
	}
	return (1);
}