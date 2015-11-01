#pragma once
#include <string>
#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS

namespace GMusicApi
{

template<typename T>
class DictionaryValue
{
public:
	DictionaryValue(const boost::python::dict& dict, const std::string& key, const T&& defaultValue = T());

	DictionaryValue& operator=(const T& newValue);

	operator T() const;

	bool operator==(const T& t) const;

private:
	std::string			m_key;
	T					m_value;
	boost::python::dict	m_dict;
};

template<typename T>
DictionaryValue<T>::DictionaryValue(const boost::python::dict& dict,
									const std::string& key,
									const T&& defaultValue = T())
	: m_dict(dict), m_key(key), m_value(defaultValue)
{
	getFromDict(dict, m_key, m_value);
}

template<typename T>
DictionaryValue<T>& DictionaryValue<T>::operator=(const T& newValue)
{
	m_value = newValue;
	m_dict[m_key] = newValue;
	return *this;
}

template<typename T>
DictionaryValue<T>::operator T() const
{
	return m_value;
}

template<typename T>
bool DictionaryValue<T>::operator==(const T& t) const
{
	return m_value == t;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const DictionaryValue<T>& dictValue)
{
	return os << static_cast<T>(dictValue);
}

} // namespace GMusicApi