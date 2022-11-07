/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anfreire <anfreire@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 10:51:25 by anfreire          #+#    #+#             */
/*   Updated: 2022/10/04 22:13:10 by anfreire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header.h"

void	exit_shell(t_data *data)
{
	if (!data->line)
		exit_shell_sig(1);
}

void	exit_shell_sig(int sig)
{
	if (sig == 1)
		printf("exit\n");
	exit(0);
}