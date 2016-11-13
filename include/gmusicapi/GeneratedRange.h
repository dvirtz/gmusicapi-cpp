#pragma once
#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/range.hpp>
#include <boost/range/any_range.hpp>
MSC_RESTORE_WARNINGS
#include <pybind11/pybind11.h>

namespace GMusicApi
{

namespace py = pybind11;

// used when get_all_songs returns a generator
template<typename T>
class GeneratorIterator :
	public boost::iterator_facade<
	GeneratorIterator<T>,
	T, 
	boost::single_pass_traversal_tag,
	T>
{
public:
	GeneratorIterator()
	{}

	GeneratorIterator(const pybind11::iterator& generator)
		: m_generatorIt(generator)
	{
		if (m_generatorIt != m_generatorEnd)
		{
			m_currentIt = generatedIterator(m_generatorIt);
            skipEmpty();
		}
	}

private:
	friend class boost::iterator_core_access;
    
    void skipEmpty()
    {
        while (m_currentIt == m_currentEnd
               && ++m_generatorIt != m_generatorEnd)
        {
            m_currentIt = generatedIterator(m_generatorIt);
        }
    }

	void increment()
	{
		if (++m_currentIt == m_currentEnd)
		{
			if (++m_generatorIt != m_generatorEnd)
			{
				m_currentIt = generatedIterator(m_generatorIt);
                skipEmpty();
			}
		}
	}

	bool equal(GeneratorIterator const& other) const
	{
		return m_generatorIt == other.m_generatorIt
			&& m_currentIt == other.m_currentIt;
	}

	T dereference() const
	{
		return (*m_currentIt).template cast<T>();
	}

    py::iterator generatedIterator(const py::iterator& it) const
    {
        return (*it).begin();
    }

	py::iterator m_generatorIt;
	py::iterator m_generatorEnd;
	py::iterator m_currentIt;
	py::iterator m_currentEnd;
};

template<typename T>
class TypedIterator :
public boost::iterator_facade<
    TypedIterator<T>,
    T,
    boost::single_pass_traversal_tag,
    T>
{
public:
    TypedIterator()
    {}

    TypedIterator(const pybind11::iterator& it)
        : m_it(it)
    {}

private:
    friend class boost::iterator_core_access;

    void increment()
    {
        ++m_it;
    }

    bool equal(TypedIterator const& other) const
    {
        return m_it == other.m_it;
    }

    T dereference() const
    {
        return (*m_it).template cast<T>();
    }

    py::iterator m_it;
};

template<typename T>
using GeneratedRange = boost::any_range<T, boost::single_pass_traversal_tag, T>;

} // namespace GMusicApi

namespace pybind11
{
namespace detail
{

template<typename T>
struct type_caster<GMusicApi::GeneratedRange<T>>
{
    PYBIND11_TYPE_CASTER(GMusicApi::GeneratedRange<T>, _(""));
    
    // Convert Python->C++
    bool load(handle src, bool /*implicit*/)
    {
        if (!PyGen_Check(src.ptr())
            && !PySequence_Check(src.ptr()))
        {
            return false;
        }

        namespace gm = GMusicApi;
                
        if (PyGen_Check(src.ptr()))
        {
            value = gm::GeneratedRange<T>(gm::GeneratorIterator<T>(iterator(src, true)),
                                          gm::GeneratorIterator<T>());
        }
        else
        {
            value = gm::GeneratedRange<T>(gm::TypedIterator<T>(src.begin()),
                                          gm::TypedIterator<T>(src.end()));
        }

        return true;
    }
};

} // namespace detail
} // namespace pybind11
