#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/iter_fold.hpp>
#include <boost/mpl/range_c.hpp>
#include <datetime.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
MSC_RESTORE_WARNINGS
#include <type_traits>

// convert python types to user defined C++ types
// based on https://misspent.wordpress.com/2009/09/27/how-to-write-boost-python-converters/

namespace GMusicApi
{

template<typename PyType>
bool isPyType(PyObject* pObj);

template<>
inline bool isPyType<boost::python::dict>(PyObject* pObj)
{
	return PyDict_Check(pObj);
}

template<>
inline bool isPyType<boost::python::str>(PyObject* pObj)
{
	return PyString_Check(pObj)
		|| PyUnicode_Check(pObj);
}

// add ability for preprocessing the python type
// before converting to C type
template<typename PyType, typename CType>
inline void pyConvert(PyObject* pObj, void* storage,
                      typename std::enable_if<!std::is_same<PyType, boost::python::str>::value
                      && !std::is_same<PyType, boost::python::dict>::value>::type* = nullptr)
{
	using namespace boost::python;

	handle<> hndl(borrowed(pObj));
	new (storage) CType(extract<PyType>(object(hndl)));
}

template <typename First, typename F>
inline void
apply(const First& /*first*/, const First& /*last*/, F /*f*/)
{}

template <typename First, typename Last, typename F>
inline void
apply(const First& first, const Last& last, F f)
{
    namespace bf = boost::fusion;
    namespace bfe = bf::extension;

    auto& member = bf::deref(first);
    using seq_type = typename std::remove_const<typename First::seq_type>::type;
    auto name = bfe::struct_member_name<seq_type, First::index::value>::call();
    f(name, member);

    apply(boost::fusion::next(first), last, f);
}

template<typename PyType, typename CType>
inline void pyConvert(PyObject* pObj, void* storage,
                      typename std::enable_if<std::is_same<PyType, boost::python::dict>::value>::type* = nullptr)
{
    namespace bt = boost::python;
    namespace bf = boost::fusion;
    using PythonHelper::getFromDict;

    bt::handle<> hndl(bt::borrowed(pObj));
    auto d = bt::extract<bt::dict>(bt::object(hndl));
    auto& s = *new (storage) CType();
    
    apply(bf::begin(s), bf::end(s), [&d, &s](const std::string& name, auto& member)
    {
        if (getFromDict(d, name, member))
        {
            s.isInitialized.insert(name);
        }
    });
}

template<typename PyType, typename CType>
inline void pyConvert(PyObject* pObj, void* storage,
                      typename std::enable_if<std::is_same<PyType, boost::python::str>::value>::type* = nullptr)
{
    if (PyUnicode_Check(pObj))
    {
        pObj = PyUnicode_AsUTF8String(pObj);
    }

    new (storage) std::string(PyString_AsString(pObj));
}


template<typename PyType, typename CType>
struct PyToCppConverter
{
	static void registerConverter()
	{
		boost::python::converter::registry::push_back(
			&convertible,
			&construct,
			boost::python::type_id<CType>());
	}

	static void* convertible(PyObject* pObj)
	{
		if (!isPyType<PyType>(pObj))
			return nullptr;

		return pObj;
	}

	// Convert obj_ptr into a QString
	static void construct(PyObject* pObj,
						  boost::python::converter::rvalue_from_python_stage1_data* data)
	{
		// Grab pointer to memory into which to construct the new CType
		void* storage = (
			(boost::python::converter::rvalue_from_python_storage<CType>*)
			data)->storage.bytes;

		pyConvert<PyType, CType>(pObj, storage);

		// Stash the memory chunk pointer for later use by boost.python
		data->convertible = storage;
	}
};

template<typename Container>
struct PySequenceToCppContainerConverter
{
	static void registerConverter()
	{
		boost::python::converter::registry::push_back(
			&convertible,
			&construct,
			boost::python::type_id<Container>());
	}

	static void* convertible(PyObject* pObj)
	{
		if (!PySequence_Check(pObj))
			return nullptr;

		return pObj;
	}

	// Convert obj_ptr into a QString
	static void construct(PyObject* pObj,
						  boost::python::converter::rvalue_from_python_stage1_data* data)
	{
		using namespace boost::python;
		using value_type = typename Container::value_type;

		// Grab pointer to memory into which to construct the new QString
		void* storage = (
			(converter::rvalue_from_python_storage<Container>*)
			data)->storage.bytes;

		handle<> hndl(borrowed(pObj));
		new (storage) Container(boost::python::stl_input_iterator<value_type>(object(hndl)),
								boost::python::stl_input_iterator<value_type>());

		// Stash the memory chunk pointer for later use by boost.python
		data->convertible = storage;
	}
};

// conver C++ type to python
struct NullptrToNoneConverter
{
	static PyObject* convert(const nullptr_t &)
	{
		Py_RETURN_NONE;
	}
};

template<typename Container>
struct CppContainerToPyListConverter
{
	static PyObject* convert(const Container& container)
	{
		namespace py = boost::python;
		py::list l;
		for (auto& element : container)
		{
			l.append(element);
		}

		// increment reference before return pointer to local
		return py::incref(l.ptr());
	}
};

template<typename Struct>
struct StructToPyDictConverter
{
	static PyObject* convert(const Struct& s)
	{
        namespace bt = boost::python;
        namespace bf = boost::fusion;
        using PythonHelper::setToDict;

        bt::dict d;
        apply(bf::begin(s), bf::end(s), [&d, &s](const std::string& name, auto& member)
        {
            if (s.isInitialized.find(name) != s.isInitialized.end())
            {
                setToDict(d, name, member);
            }
        });

        return bt::incref(d.ptr());
	}
};

struct BoostPTimeToPyDateTimeConverter
{
    static PyObject* convert(const boost::posix_time::ptime& time)
    {
        auto d = time.date();
        auto tod = time.time_of_day();
        auto usec = static_cast<int>(tod.total_microseconds() % 1000000);
        return PyDateTime_FromDateAndTime(static_cast<int>(d.year()), 
                                          static_cast<int>(d.month()), 
                                          static_cast<int>(d.day()), 
                                          static_cast<int>(tod.hours()), 
                                          static_cast<int>(tod.minutes()), 
                                          static_cast<int>(tod.seconds()), usec);
    }
};

template<typename T>
struct BoostOptionalToPyConverter
{
    static PyObject* convert(const boost::optional<T>& opt)
    {
        namespace bp = boost::python;
        if (opt)
        {
            return bp::incref(bp::converter::arg_to_python<T>(*opt).get());
        }

        // if not set convert to None
        Py_RETURN_NONE;
    }
};

} // namepsace GMusicApi