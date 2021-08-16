#ifndef EVENTHANDLERINSTANCE_HPP
# define EVENTHANDLERINSTANCE_HPP

class	EventHandler;

# include "EventHandler.hpp"

class	EventHandlerInstance
{
public:
	EventHandlerInstance()	{}
	~EventHandlerInstance()	{}

	static void			setInstance(EventHandler *evh)	{ instance = evh; }
	static EventHandler	&getInstance()	{ return (*instance); }

private:
	EventHandlerInstance(const EventHandlerInstance &other);

	EventHandlerInstance&	operator=(const EventHandlerInstance &other);

	static EventHandler	*instance;
};

#endif
