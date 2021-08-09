#ifndef EVENTHANDLERINSTANCE_HPP
# define EVENTHANDLERINSTANCE_HPP

# include "EventHandler.hpp"

class	EventHandlerInstance
{
public:
	EventHandlerInstance();
	~EventHandlerInstance();

	operator EventHandler() 		{ return getInstance(); }

	EventHandler	*operator->()	{ return &getInstance(); }

	EventHandler	&getInstance()	{ return (instance); }

private:
	EventHandlerInstance(const EventHandlerInstance &other);

	EventHandlerInstance&	operator=(const EventHandlerInstance &other);

	EventHandler	instance;
};

#endif