#pragma once

#include <window.h>

class Application
{
public:
	Application()                       = default;
	virtual ~Application()              = default;
	Application(const Application&)     = delete;
	Application(const Application&&)    = delete;
	void operator=(const Application&)  = delete;
	void operator=(const Application&&) = delete;

	virtual void Run() = 0;
	
};
