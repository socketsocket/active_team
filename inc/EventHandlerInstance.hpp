#ifndef EVENTHANDLERINSTANCE_HPP
# define EVENTHANDLERINSTANCE_HPP

# include "EventHandler.hpp"

class	EventHandlerInstance
{
public:
	EventHandlerInstance()	{}
	~EventHandlerInstance()	{}

	operator EventHandler() 		{ return getInstance(); }

	static EventHandler	&getInstance()	{ return (instance); }
	EventHandler		*operator->()	{ return &getInstance(); }

private:
	EventHandlerInstance(const EventHandlerInstance &other);

	EventHandlerInstance&	operator=(const EventHandlerInstance &other);

	static EventHandler	instance;
};

#endif
