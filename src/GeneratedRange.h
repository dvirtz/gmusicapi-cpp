#pragma once
#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/range.hpp>
#include <boost/range/any_range.hpp>
#include <boost/python/stl_iterator.hpp>
MSC_RESTORE_WARNINGS
#include <type_traits>

namespace GMusicApi
{

template<typename T>
using input_iterator = boost::python::stl_input_iterator<T>;

// used when get_all_songs returns a generator
template<typename T>
class GeneratorIterator :
	public boost::iterator_facade<
	GeneratorIterator<T>,
	typename input_iterator<T>::value_type, 
	typename input_iterator<T>::iterator_category,
	typename input_iterator<T>::reference>
{
public:
	GeneratorIterator()
	{}

	GeneratorIterator(const boost::python::object& generator)
		: m_generatorIt(generator)
	{
		if (m_generatorIt != m_generatorEnd)
		{
			m_currentIt = input_iterator<T>(*m_generatorIt);
		}
	}

private:
	friend class boost::iterator_core_access;

	void increment()
	{
		if (++m_currentIt == m_currentEnd)
		{
			if (++m_generatorIt != m_generatorEnd)
			{
				m_currentIt = input_iterator<T>(*m_generatorIt);
			}
		}
	}

	bool equal(GeneratorIterator const& other) const
	{
		return m_generatorIt == other.m_generatorIt
			&& m_currentIt == other.m_currentIt;
	}

	typename input_iterator<T>::reference dereference() const
	{
		return *m_currentIt;
	}

	input_iterator<boost::python::list>	m_generatorIt;
	input_iterator<boost::python::list>	m_generatorEnd;
	input_iterator<T>					m_currentIt;
	input_iterator<T>					m_currentEnd;
};

template<typename T>
using GeneratedRange = boost::any_range<
	typename GeneratorIterator<T>::value_type,
	typename boost::iterator_category_to_traversal<typename GeneratorIterator<T>::iterator_category>::type,
	// need to be const due to boost bug https://svn.boost.org/trac/boost/ticket/10493
	typename std::add_const<typename GeneratorIterator<T>::reference>::type,
	typename GeneratorIterator<T>::difference_type>;


template<typename T>
struct pyGeneratorToGeneratedRangeConverter
{
	static void registerConverter()
	{
		boost::python::converter::registry::push_back(
			&convertible,
			&construct,
			boost::python::type_id<GeneratedRange<T>>());
	}

	static void* convertible(PyObject* pObj)
	{
		if (!PyGen_Check(pObj))
			return nullptr;

		return pObj;
	}

	// Convert obj_ptr into a QString
	static void construct(PyObject* pObj,
						  boost::python::converter::rvalue_from_python_stage1_data* data)
	{
		using namespace boost::python;

		// Grab pointer to memory into which to construct the new QString
		void* storage = (
			(converter::rvalue_from_python_storage<GeneratedRange<T>>*)
			data)->storage.bytes;

		handle<> hndl(borrowed(pObj));
		new (storage) GeneratedRange<T>(GeneratorIterator<T>(object(hndl)),
										GeneratorIterator<T>());

		// Stash the memory chunk pointer for later use by boost.python
		data->convertible = storage;
	}
};

} // namespace GMusicApi
