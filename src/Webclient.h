#pragma once
#include "Python.h"
#include <string>

namespace GMusicApi
{

class Webclient
{
public:
	Webclient();
	~Webclient();

	void login(const std::string& email, const std::string& password);

};


} // namespace GMusicApi
