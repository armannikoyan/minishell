/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anikoyan <anikoyan@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:53:04 by anikoyan          #+#    #+#             */
/*   Updated: 2024/07/23 13:25:46 by anikoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdint.h>
# include <stdarg.h>
# include <stdbool.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_flags
{
	int	zero;
	int	minus;
	int	hash;
	int	space;
	int	plus;
	int	width;
	int	precision;
	int	upper;
}		t_flags;

void	ft_bzero(void *s, size_t n);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_lstadd_front(t_list **lst, t_list *new_node);
void	ft_lstadd_back(t_list **lst, t_list *new_node);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
void	ft_strrev(char *str);
void	ft_str_toupper(char *str, int upper);

void	*ft_memset(void *b, int c, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_calloc(size_t count, size_t size);

char	*ft_strcat(char *dest, const char *src);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char	*ft_uitoa(unsigned int u_nbr);
char	*ft_uitoa_base(size_t u_nbr, int base);
char	*ft_process_flags(char *number, int *padding_width, int *result,
			t_flags *flags);

char	**ft_split(char const *s, char c);

int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_isspace(char c);
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_atoi(const char *str);
int		ft_lstsize(t_list *lst);
int		ft_putchar(char c);
int		ft_putstr(char *str);
int		ft_putchar_n(char c, int *count);
int		ft_padding_width(char *number, t_flags *flags);
int		ft_printf(const char *fmt, ...);
int		ft_print_char(char c, t_flags *flags);
int		ft_print_string(char *str, t_flags *flags);
int		ft_print_unsigned(unsigned int u_nbr, t_flags *flags);
int		ft_print_int(int nbr, t_flags *flags);
int		ft_print_hex(unsigned int u_nbr, t_flags *flags);
int		ft_print_pointer(size_t u_nbr, t_flags *flags);

double	ft_atof(const char *str);

size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);

t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_flags	ft_printf_parse_flags(const char *fmt);

#endif
