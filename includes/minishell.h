/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolifero <dolifero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:54:47 by dolifero          #+#    #+#             */
/*   Updated: 2024/07/14 17:32:15 by dolifero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//EXTERNAL INCLUDES

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <signal.h>
# include <ctype.h>

//INTERNAL INCLUDES

# include "../Libft/libft.h"
# include "enumtypes.h"
# include "structs.h"
# include "functions.h"

# ifndef FANCY
#  define FANCY 1
# endif

#endif
