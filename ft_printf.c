/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwang-do <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/03 16:21:42 by bwang-do          #+#    #+#             */
/*   Updated: 2018/10/28 17:17:11 by bwang-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.h"

t_data	*ft_init_data(t_data *data)
{
	int		i;
	t_spec	*spec;

	if ((data = (t_data*)malloc(sizeof(t_data))) == NULL)
		return (NULL);
	if ((spec = (t_spec*)malloc(sizeof(t_spec))) == NULL)
		return (NULL);
	data->i = 0;
	i = 0;
	while (i < 5)
	{
		spec->flags[i] = 0;
		i++;
	}
	spec->width = 0;
	spec->prec = -1;
	spec->mod[0] = 0;
	spec->mod[1] = 0;
	spec->conv = 0;
	data->spec = spec;
	data->len = 0;
	ft_bzero(data->buff, BUFF_SIZE);
	data->arg_len = 0;
	return (data);
}

t_data	*ft_reset_data(t_data *data)
{
	int		i;

	i = 0;
	data->len += data->arg_len;
	while (i < 5)
	{
		data->spec->flags[i] = 0;
		i++;
	}
	data->spec->width = 0;
	data->spec->prec = -1;
	data->spec->mod[0] = 0;
	data->spec->mod[1] = 0;
	data->spec->conv = 0;
	data->arg_len = 0;
	return (data);
}

char	*ft_buff_cat(char *str, t_data *data, int *i)
{
	if (!(str = ft_realloccat(str, data->buff, data->len, *i)))
		return (NULL);
	ft_bzero(data->buff, BUFF_SIZE);
	data->len += *i;
	*i = 0;
	return (str);
}

char	*ft_process(const char *f, va_list ap, t_data *data)
{
	int		i;
	char	*arg;
	char	*str;

	i = 0;
	str = NULL;
	while (f[data->i])
	{
		if (i == BUFF_SIZE)
		{
			if ((str = ft_buff_cat(str, data, &i)) == NULL)
				return (NULL);
		}
		if (f[data->i] == '%')
		{
			str = ft_buff_cat(str, data, &i);
			arg = ft_get_arg(f, ap, data);
			if (!(str = ft_realloccat_free(str, arg, data->len, data->arg_len)))
				return (NULL);
			ft_reset_data(data);
		}
		else
			data->buff[i++] = f[(data->i)++];
	}
	return (str = ft_buff_cat(str, data, &i));
}

int		ft_printf(const char *format, ...)
{
	va_list ap;
	char	*str;
	t_data	*data;
	int		len;

	len = 0;
	if (format == NULL)
		return (0);
	if ((data = ft_init_data(data)) == NULL)
		return (0);
	va_start(ap, format);
	str = ft_process(format, ap, data);
	va_end(ap);
	if (str == NULL)
		len = 0;
	else
	{
		len = write(1, str, data->len);
		free(str);
		str = NULL;
	}
	free(data->spec);
	free(data);
	data = NULL;
	return (len);
}
