#include "Server.hpp"

/* static */

/* public */

void
	Server::setAutoindex(std::string on_off_string)
{
	if (on_off_string == "on")
		auto_index = true;
	else if (on_off_string == "off")
		auto_index = false;
	else
		throw BadDirective(on_off_string);
}

/* private */
