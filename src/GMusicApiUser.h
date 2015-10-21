#pragma once
#include "GMusicApi.h"

namespace GMusicApi
{

class GMusicApiUser
{
public:
	template<typename... Args>
	GMusicApiUser(Args&&... args);
	virtual ~GMusicApiUser() {}

	template<typename Ret, typename ...Args>
	Ret callMethod(const std::string& methodName, Args&&... args) const;

private:
	GMusicApi& instance() const;
	boost::python::object m_object;
};


template<typename... Args>
GMusicApiUser::GMusicApiUser(Args&&... args)
{
	m_object = instance().createObject(std::forward<Args>(args)...);
}

template<typename Ret, typename ...Args>
inline Ret GMusicApiUser::callMethod(const std::string& methodName, Args&& ...args) const
{
	return instance().callMethod<Ret>(m_object, methodName, std::forward<Args>(args)...);
}

} // namespace GMusicApi
