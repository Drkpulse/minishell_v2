/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:35:19 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/10 02:09:40 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef enum e_retval
{
	OK = 0,
	NOK = -1,
	ERR_MEM = 1,
	ERR_IO = 2,
	ERR_FILE = 3,
	ERR_ARGS = 4
}	t_retval;

//LIBC FUNCTIONS//

/**
 * @brief Converts string to integer.
 * @param str String to convert.
 * @returns String as int or 0 if invalid.
*/
int		ft_atoi(const char *str);

/**
 * @brief Sets memory area to zero.
 * @param ptr Pointer to start of memory area to clear.
 * @param n Number of bytes to clear.
 * @returns Nothing.
 */
void	ft_bzero(void *s, size_t n);

/**
 * @brief Checks if char is alphanumeric.
 * @param c Character to check.
 * @returns 1 if true, 0 if false.
*/
int		ft_isalnum(int c);

/**
 * @brief Checks if char is a letter.
 * @param c Character to check.
 * @returns 1 if true, 0 if false.
*/
int		ft_isalpha(int c);

/**
 * @brief Checks if char is ASCII.
 * @param c Character to check.
 * @returns 1 if true, 0 if false.
*/
int		ft_isascii(int c);

/**
 * @brief Checks if char is digit.
 * @param c Character to check.
 * @returns 1 if true, 0 if false.
*/
int		ft_isdigit(int c);

/**
 * @brief Checks if char is printable.
 * @param c Character to check.
 * @returns 1 if true, 0 if false.
*/
int		ft_isprint(int c);

/**
 * @brief Scans for the first occurence of a value in a memory area.
 * @param s Pointer to the start of the memory area.
 * @param c Value to search for.
 * @param n size in bytes of the memory area.
 * @returns Pointer to the first occurence of the value or NULL if not found.
*/
void	*ft_memchr(const void *s, int c, size_t n);

/**
 * @brief Compares two memory areas up to n bytes.
 * @param s1 Pointer to memory area to compare.
 * @param s2 Pointer to memory area to compare.
 * @param n Number of bytes to compare.
 * @returns Diference between the first unequal bytes or 0 if equal.
*/
int		ft_memcmp(const void *str1, const void *str2, size_t n);

/** @brief Copies n bytes from src to dest. */
void	*ft_memcpy(void *str2, const void *src, size_t n);

/** @brief Copies n bytes from src to dest. The memory areas may overlap. */
void	*ft_memmove(void *str1, const void *str2, size_t n);

/** @brief Sets n bytes starting at s with c. */
void	*ft_memset(void *str, int c, size_t n);

/** @brief Locates the first occurrence of c in s. */
char	*ft_strchr(const char *str, int c);

/** @brief Appends src to the end of dst. */
size_t	ft_strlcat(char *dest, const char *src, size_t size);

/** @brief Copies up to size - 1 chars from src to dst, NULL-terminating. */
size_t	ft_strlcpy(char *dest, const char *src, size_t size);

/** @brief Computes the length of s. */
size_t	ft_strlen(const char *str);

/** @brief Compares first n bytes of s1 and s2. */
int		ft_strncmp(const char *str1, const char *str2, size_t n);

/** @brief Locates first occurrence of little in the first len chars of big. */
char	*ft_strnstr(const char *string, const char *substring, size_t len);

/** @brief Locates last occurrence of c in the string s. */
char	*ft_strrchr(const char *str, int c);

/** @brief Converts c to lowercase. */
int		ft_tolower(int c);

/** @brief Converts c to uppercase. */
int		ft_toupper(int c);

/**
 * @brief Allocates memory for a number of items and sets all bytes to 0.
 * @param nitems Number of items to alocate memory for.
 * @param size Size in bytes of each item.
 * @returns Pointer to allocated memory.
*/
void	*ft_calloc(size_t nmemb, size_t size);

/** @brief Duplicates str1. */
char	*ft_strdup(const char *src);

//NON LIBC FUNCTIONS//
char	*ft_substr(char const *s, unsigned int start, size_t len);

/** @brief Returns the concatenation of s1 and s2. */
char	*ft_strjoin(char const *s1, char const *s2);

/** @brief Returns a copy of s1 with the chars in set removed from the ends. */
char	*ft_strtrim(char const *s1, char const *set);

/** @brief Returns strings obtained by splitting s using c as a delimiter. */
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);

/** @brief Outputs c to the given file descriptor. */
void	ft_putchar_fd(char c, int fd);

/** @brief Outputs s to the given file descriptor, followed by newline. */
void	ft_putendl_fd(char *s, int fd);

/** @brief Outputs n to the given file descriptor. */
void	ft_putnbr_fd(int n, int fd);

/** @brief Outputs s to the given file descriptor. */
void	ft_putstr_fd(char *s, int fd);

/** @brief Applies function f to each character of s. */
void	ft_striteri(char *s, void (*f)(unsigned int, char *));

/** @brief Applies function f to each character of s to create a new string. */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));

/** @brief Safe version of free, resets ptr to NULL after freeing memory */
void	ft_free(void **ptr);

void	ft_free_array(void **array);

/**
 * @brief Checks if char is space.
 * @param c Character to check.
 * @returns 1 if true, 0 if false.
*/
int	ft_isspace(int c);

//BONUS FUNCTIONS//
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

t_list	*ft_lstnew(void *content);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif
