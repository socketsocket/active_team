/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seyu <seyu@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 16:27:54 by seyu              #+#    #+#             */
/*   Updated: 2021/07/27 16:42:24 by seyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "Message.hpp"

int	Response::raw(std::string *raw_string)
{
	raw_string->clear();
	raw_string->append(http_version);
	raw_string->append(" ");
	raw_string->append(std::to_string(code));
	raw_string->append(" ");
	raw_string->append(message);
	raw_string->append("\r\n");
	for (std::map<std::string, std::string>::const_iterator itr = header.begin();
			itr != header.end();
			++itr)
	{
		raw_string->append(itr->first);
		raw_string->append(" ");
		raw_string->append(itr->second);
		raw_string->append("\r\n");
	}
	raw_string->append("\r\n");
	if (body.empty() == false)
		raw_string->append(body);
	return (0);
}
