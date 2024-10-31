/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:17:46 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/31 17:06:16 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	write_temp_file(const char *limiter, int fd, char *temp_file)
{
	char	*line;

	line = NULL;
	while (1)
	{
		write(STDOUT_FILENO, "here_doc> ", 10);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			perror("Failed to read line");
			unlink(temp_file);
			close(fd);
			exit(EXIT_FAILURE);
		}
		if (ft_strncmp(line, limiter, ft_strlen(limiter))
			== 0 && line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
}

void	handle_here_doc(const char *limiter, char *temp_file)
{
	int		fd;

	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Failed to open temp_file");
		exit(EXIT_FAILURE);
	}
	write_temp_file(limiter, fd, temp_file);
	close(fd);
}

void	check_infile(t_prog *prog, char *infile)
{
	prog->infile = open(infile, O_RDONLY);
	if (prog->infile < 0)
	{
		if (errno == EACCES || errno == ENOENT || errno == EISDIR)
		{
			perror(infile);
			prog->infile = STDIN_FILENO;
		}
	}
}

void	check_outfile(t_prog *prog, char *outfile)
{
	if (prog->here_doc == 1)
		prog->outfile = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		prog->outfile = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (prog->outfile < 0)
	{
		if (errno == EACCES)
		{
			perror(outfile);
			ft_close(prog);
			exit(1);
		}
		else
		{
			perror(outfile);
			ft_close(prog);
			exit(0);
		}
	}
}

void	prepare_files(t_prog *prog, int argc, char **argv, char *temp_file)
{
	if (prog->here_doc == 1)
	{
		handle_here_doc(argv[2], temp_file);
		check_infile(prog, temp_file);
	}
	else
		check_infile(prog, argv[1]);
	check_outfile(prog, argv[argc - 1]);
}
