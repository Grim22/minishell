/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_traitement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrunet <bbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 12:15:45 by grim              #+#    #+#             */
/*   Updated: 2020/10/08 10:22:15 by bbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

static int	contains_pipe(t_list *cmd_list)
{
	if (cmd_list->next)
		return (TRUE);
	else
		return (FALSE);
}

static int	ft_traitement_cmdlist(t_list *cmd_list, t_list **env)
{
	t_cmd	*cmd;
	int		index;

	// deux cas de figures:
		// 1. cmd_list ne contient qu'une seule commande (pas de "|"), et cette commande est un built-in
		//	--> execution de la commande au sein du programme (pas de fork()): cf ft_built_in.c
		// 2. autres cas de figures
		//	--> execution de la (des) commande(s) au sein d'un fork(): cf ft_executable_cmd.c
		// remarque: dans le cas de commandes mulitples (separees par des pipes), meme les built ins sont exectutes au sein de forks 
	
	if (contains_pipe(cmd_list))
		ft_executable_cmd(cmd_list, *env);
	else
	{
		cmd = (t_cmd*)cmd_list->content;
		if (ft_check_built_in(cmd->argv[0], &index) == TRUE)
			ft_built_in(cmd, index, env);
		else if (ft_strcmp(cmd->argv[0], "exit") == 0)
			return (EXIT_CODE);
		else
			ft_executable_cmd_single(cmd_list, *env);
	}
	return (SUCCESS);
}

int			ft_traitement(t_list *pipe_list, t_list **env)
{
	t_list	*cmd_list;
	t_cmd	*cmd;

	// pipe_list = ensemble de cmd_list, separees par des ";"
	// cmd_list = ensemble de cmd, separees par des "|"
	// on traite les "cmd_list" les unes apres les autres

	while (pipe_list)
	{
		cmd_list = (t_list*)pipe_list->content;
		// ft_expansion: substitution des $var: 
		// 	cette etape ne peut pas etre faite en amont: il faut attendre que cmd_list1 ai ete traitee pour faire la substitution dans cmd_list2 (ex: export var=truc; echo $var)
		ft_expansion(cmd_list, *env); 
		clean_quotes(cmd_list);
		fill_cmdlist_path(cmd_list, *env);
		cmd = (t_cmd*)cmd_list->content;
		if (ft_traitement_cmdlist(cmd_list, env) == EXIT_CODE)
			ms_exit(pipe_list, cmd->argc, cmd->argv, env);
		pipe_list = pipe_list->next;
	}
	return (SUCCESS);
}
