/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_executable_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrunet <bbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 18:51:03 by grim              #+#    #+#             */
/*   Updated: 2020/10/14 18:15:28 by bbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

static void	ft_exec_cmd(t_list *cmd_elem, char **env_tab)
{
	t_cmd	*cmd;

	errno = 0;
	cmd = (t_cmd*)cmd_elem->content;

	// on commence par faire les redirections (ft_redirs)
	// puis on execute la commande (execve)
	// on gere enfin les cas d'erreurs
	// Remarque: si pas d'erreur, execve ne return() pas (seul le wait() permet de suspendre le programme jusqu'a la fin du programme execute par execve, et de recuperer la valeur de retour)

	if (ft_redirs(cmd) != FAILURE)
	{
		if (cmd->argc == 0)
			exit(0);
		if (cmd->cmd_path)
			execve(cmd->cmd_path, cmd->argv, env_tab);
		if (!cmd->cmd_path || errno == 2)
		{
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putendl_fd(": command not found", 2);
			exit(127);
		}
		if (errno == 13)
		{
			put_err(cmd->argv[0], NULL, ": Permission denied", TRUE);
			exit(126);
		}
	}
	else
		exit(g_glob.ret);
}

static int	ft_builtin_or_bin(t_list *cmd_elem, t_list **env, char **env_tab)
{
	t_cmd	*cmd;
	int		index;
	int		ret;

	// au sein du fork(), deux cas de figure pour executer la commande:
	//	1. la commande est un built in: on l'execute a l'aide des .c qu'on a codé
	//	2. la commande n'est pas un built-in: on recherche lexecutable (le binaire) correspondant a la commande, et on lexecute avec un execve()
	// Remarque: on est obligés de faire cette distinction, car on a pas dexecutable pour les built-ins (alternativement, on aurait pu creer ces executables a partir de nos .c)

	ret = FAILURE;
	cmd = (t_cmd*)cmd_elem->content;
	if (ft_check_built_in(cmd->argv[0], &index) == TRUE)
	{
		ret = ft_built_in(cmd, index, env);
		if (ret == FAILURE)
			exit(1);
		else
			exit(0);
	}
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ms_exit(NULL, cmd->argc, cmd->argv, env);
	else
		ft_exec_cmd(cmd_elem, env_tab);
	return (SUCCESS);
}

int			ft_executable_cmd(t_list *cmd_list, t_list *env)
{
	int		**fd;
	char	**env_tab;
	size_t	num_pipe;
	int		i;

	// meme principe que pour ft_executable_cmd_single, mais avec plusieurs commandes, separees par des "|"
	// chaque commande va etre executee au sein d'un fork(). tous les fork() sont appeles par le process principal
	// 	Rq: on attend pas qu'une commande ait termine pour lancer la suivante: execution de maniere concommittante
	i = 0;
	env_tab = ft_list_to_tab(env);
	fd = NULL;
	// Gestion des "|": build_pipe -> fork() -> dup_pipe ->close_pipe
	// "build_pipes": dans le process parent, on cree autant de pipes qu'il y a de "|" (avec la fonction pipe())
	// 	Rq: un pipe() permet a deux fd de comminiquer entre eux. la communication ne se fait que dans un sens: un des fd permet decrire dans le pipe, et l'autre permet de lire en provenance du pipe.
	//	un pipe est un int[2], ou int[0] est le fd qui permet de lire, et int[1] est le fd qui permet decrire. on stocke tous ces int[2] dans un int **fd. 
	//	fd[i][PIPE_WRITE]: fd qui permet d'ecrire dans le iem pipe
	// "dup_pipes": on duplique les deux fd du pipe. l'un vers stdout et l'autre vers stdin.
	//	stdout devient capable decrire dans le pipe, et stdin de lire dans le pipe: la communication est alors possible: stdout de la premiere commande est relie a stdin de la seconde
	// "close_pipes": dans chaque fork(), il faudra fermer tous les pipes une fois qu'on en a plus besoin
	// 	Rq: chaque child process (issu du fork()) va heriter de tous les pipes crees par son parent process
	// cf. m_executable_utils
	num_pipe = ft_build_pipes(cmd_list, &fd);
	if (fork() == 0 && dup_close_pipes(fd, 0, fd[i][PIPE_WRITE], num_pipe) && close(fd[i][PIPE_READ]))
		ft_builtin_or_bin(cmd_list, &env, env_tab);
	cmd_list = cmd_list->next;
	while (cmd_list->next)
	{
		if (fork() == 0 && dup_close_pipes(fd, fd[i][PIPE_READ],
			fd[i + 1][PIPE_WRITE], num_pipe) && close(fd[i][PIPE_WRITE]) && close(fd[i + 1][PIPE_READ]))
			ft_builtin_or_bin(cmd_list, &env, env_tab);
		cmd_list = cmd_list->next;
		i++;
	}
	if ((g_glob.pid = fork()) == 0
		&& dup_close_pipes(fd, fd[i][PIPE_READ], 0, num_pipe) && close(fd[i][PIPE_WRITE]))
		ft_builtin_or_bin(cmd_list, &env, env_tab);
	// on attend que toutes les commandes aient return (autant de wait que de commandes).
	// on ne recupere que la valeur de retour de la derniere commande (celle dont le pid est stockee dans g_glob.pid)
	// cf m_executable_utils
	close_wait_free(fd, num_pipe, env_tab);
	return (SUCCESS);
}

int			ft_executable_cmd_single(t_list *cmd_list, t_list *env)
{
	char	**env_tab;
	int		status;

	env_tab = ft_list_to_tab(env);
	// on execute la commande au sein d'un fork()
	if ((g_glob.pid = fork()) == 0)
		ft_builtin_or_bin(cmd_list, &env, env_tab);
	// on attend que la commande ait fini sont execution au sein du fork(), et on recupere son exit value
	wait(&status);
	if (WIFEXITED(status))
		g_glob.ret = WEXITSTATUS(status);
	g_glob.pid = 0;
	free_tab2(env_tab);
	return (SUCCESS);
}
