/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv_d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwang-do <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 18:54:52 by bwang-do          #+#    #+#             */
/*   Updated: 2018/10/24 18:28:27 by bwang-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.h"

long long	ft_check_mod_d(va_list ap, t_spec *spec)
{
	long long	n;

	if (spec->mod[0] == spec->mod[1] && spec->mod[1] == 'l')
		n = va_arg(ap, long long);
	else if (spec->mod[0] == 'l')
		n = va_arg(ap, long);
	else if (spec->mod[0] == 'j')
		n = va_arg(ap, intmax_t);
	else if (spec->mod[0] == 'z')
		n = va_arg(ap, ssize_t);
	else if (spec->mod[0] == spec->mod[1] && spec->mod[1] == 'h')
		n = (signed char)va_arg(ap, int);
	else if (spec->mod[0] == 'h')
		n = (short)va_arg(ap, int);
	else
		n = va_arg(ap, int);
	return (n);
}

static char	*ft_check_prec(long long n, t_spec *spec, char *ret)
{
	int	len;

	if (n == 0 && spec->prec == 0)
	{
		if ((ret = ft_strdup("")) == NULL)
			return (NULL);
	}
	else if ((ret = ft_lltoa(n)) == NULL)
		return (NULL);
	len = ft_strlen(ret);
	if (n < 0)
		len--;
	if (spec->prec > 0)
	{
		if (spec->prec > len)
			ret = ft_realloc_free(ft_nchar('0', spec->prec - len), ret);
		else if (!n)
			ret = ft_strsub(ret, 0, spec->prec - 1);
	}
	return (ret);
}

static char	*ft_check_flags(long long n, t_spec *spec, char *ret, t_data *data)
{
	int		len;

	len = ft_strlen(ret);
	if (spec->flags[2] == 1 && n >= 0)
	{
		if (spec->flags[1] == 2)
		{
			if ((ret = ft_width(ret, len + 1, spec, data)) == NULL)
				return (NULL);
		}
		if ((ret = ft_realloc_free(ft_strdup("+"), ret)) == NULL)
			return (NULL);
		if (spec->flags[1] != 2)
			ret = ft_width(ret, len + 1, spec, data);
	}
	else if (spec->flags[2] == 2 && n >= 0)
	{
		if ((ret = ft_width(ret, len + 1, spec, data)) == NULL)
			return (NULL);
		ret = ft_realloc_free(ft_strdup(" "), ret);
	}
	else
		ret = ft_width(ret, len, spec, data);
	return (ret);
}

static char	*ft_check_sign(char *ret)
{
	int	i;

	if (ret && ret[0] == '0')
	{
		i = 0;
		while (ret[i] == '0')
			i++;
		ret[i] = '0';
		ret[0] = '-';
	}
	return (ret);
}

char		*ft_conv_d(va_list ap, t_spec *spec, t_data *data)
{
	long long	n;
	char		*ret;
	int			checked;

	n = ft_check_mod_d(ap, spec);
	checked = 0;
	if (!(ret = ft_check_prec(n, spec, ret)))
		return (NULL);
	if (n < 0)
	{
		if (spec->width > ft_strlen(ret) && spec->flags[1] == 2)
		{
			if (!(ret = ft_check_flags(n, spec, ret, data)))
				return (NULL);
			checked = 1;
		}
		ret = ft_check_sign(ret);
	}
	if (!checked)
		ret = ft_check_flags(n, spec, ret, data);
	return (ret);
}
