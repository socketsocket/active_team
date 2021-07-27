/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seyu <seyu@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 16:21:51 by seyu              #+#    #+#             */
/*   Updated: 2021/07/27 16:41:54 by seyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <map>

enum	Method { GET, POST, DELETE };

struct	Request
{
public:
	Method								method;
	std::string							uri;
	std::string							http_version;
	std::map<std::string, std::string>	header;
	std::string							body;
};

struct	Response
{
public:
	std::string							http_version;
	unsigned int						code;
	std::string							message;
	std::map<std::string, std::string>	header;
	std::string							body;

	int	raw(std::string *raw_string);
};

#endif
