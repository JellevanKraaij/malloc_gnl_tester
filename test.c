/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvan-kra <jvan-kra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 16:00:40 by jvan-kra          #+#    #+#             */
/*   Updated: 2021/11/10 15:57:04 by jvan-kra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET  "\x1b[32m"

#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include "../get_next_line.h"

int g_free_count;
int	g_malloc_count;
int	g_malloc_target;

void	*malloc(size_t size)
{
	void *res;
	void	*(*real_malloc)(size_t);
	real_malloc = dlsym(RTLD_NEXT, "malloc");
	if (g_malloc_count + 1 >= g_malloc_target)
	{
		if (PRINT_ON)printf(COLOR_YELLOW "\nmalloc fail" COLOR_RESET);
		return (NULL);
	}
	g_malloc_count++;
	res = real_malloc(size);
	if (PRINT_ON)printf(COLOR_YELLOW "\nmalloc %p %lu" COLOR_RESET, res, size);
	return (res);
}

void	free(void *to_free)
{
	void	*(*real_free)(void *);

	real_free = dlsym(RTLD_NEXT, "free");
	real_free(to_free);
	if (to_free != NULL)
	{
		g_free_count++;
		if (PRINT_ON)printf(COLOR_YELLOW "\nfree %p" COLOR_RESET, to_free);
	}
}

void	my_test(void)
{
	int		test = open(FILE_NAME, O_RDONLY);
	char	*res = (char *)1;

	while (res != NULL)
	{
		res = get_next_line(test);
		if (PRINT_ON)printf("\n\"" COLOR_GREEN"%s"COLOR_RESET "\"", res);
		free(res);
	}
	close(test);
}

int	main(void)
{
	while (g_malloc_target < 100)
	{
		g_malloc_count = 0;
		g_free_count = 0;
		if (PRINT_ON) printf("-----  running fail on malloc %d with buffersize %d -----", g_malloc_target, BUFFER_SIZE);
		my_test();
		if (PRINT_ON)printf("\n-----  result times malloc=%d free=%d -----\n\n", g_malloc_count, g_free_count);
		if (g_malloc_count != g_free_count)
			exit(1);
		g_malloc_target++;
	}
	return (0);
}
