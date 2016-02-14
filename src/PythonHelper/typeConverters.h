#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/iter_fold.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/mpl/range_c.hpp>
#include <datetime.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
MSC_RESTORE_WARNINGS
#include <tuple>
#include <type_traits>

// convert python types to user defined C++ types
// based on https://misspent.wordpress.com/2009/09/27/how-to-write-boost-python-converters/

namespace PythonHelper
{

template<typename PyType, typename CType>
struct PyTypeChecker {};

template<typename CType>
struct PyTypeChecker<boost::python::dict, CType>
{
    bool operator()(PyObject* pObj)
    {
        return PyDict_Check(pObj);
    }
};

template<typename CType>
struct PyTypeChecker<boost::python::str, CType>
{
    bool operator()(PyObject* pObj)
    {
        return PyString_Check(pObj)
            || PyUnicode_Check(pObj);
    }
};

template<typename CType>
struct PyTypeChecker<boost::python::tuple, CType>
{
    bool operator()(PyObject* pObj)
    {
        return PyTuple_Check(pObj);
    }
};

template<typename T, typename S>
struct PyTypeChecker<boost::python::tuple, std::pair<T,S>>
{
    bool operator()(PyObject* pObj)
    {
        return PyTuple_Check(pObj)
            && PyTuple_Size(pObj) == 2;
    }
};

// add ability for preprocessing the python type
// before converting to C type
template<typename PyType, typename CType>
struct PyConverter
{
    void operator()(PyObject* pObj, void* storage)
    {
        using namespace boost::python;

        handle<> hndl(borrowed(pObj));
        new (storage) CType(extract<PyType>(object(hndl)));
    }
};

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

template<typename CType>
struct PyConverter<boost::python::dict, CType>
{
    void operator()(PyObject* pObj, void* storage)
    {
        namespace bp = boost::python;
        namespace bf = boost::fusion;
        using PythonHelper::getFromDict;

        bp::handle<> hndl(bp::borrowed(pObj));
        auto d = bp::extract<bp::dict>(bp::object(hndl));
        auto& s = *new (storage) CType();

        apply(bf::begin(s), bf::end(s), [&d, &s](const std::string& name, auto& member)
        {
            if (getFromDict(d, name, member))
            {
                s.isInitialized.insert(name);
            }
        });
    }
};

template<typename Key, typename Value>
struct PyConverter<boost::python::dict, std::map<Key, Value>>
{
    void operator()(PyObject* pObj, void* storage)
    {
        namespace bp = boost::python;

        bp::handle<> hndl(bp::borrowed(pObj));
        bp::dict d = bp::extract<bp::dict>(bp::object(hndl));

        using it = bp::stl_input_iterator<typename std::map<Key, Value>::value_type>;

        new(storage)std::map<Key, Value>(it(d.iteritems()), it());
    }
};

template<>
struct PyConverter<boost::python::str, std::string>
{
    void operator()(PyObject* pObj, void* storage)
    {
        if (PyUnicode_Check(pObj))
        {
            pObj = PyUnicode_AsUTF8String(pObj);
        }

        new (storage) std::string(PyString_AsString(pObj));
    }
};

template<>
struct PyConverter<boost::python::str, std::vector<char>>
{
    void operator()(PyObject* pObj, void* storage)
    {
        if (PyUnicode_Check(pObj))
        {
            pObj = PyUnicode_AsUTF8String(pObj);
        }

        auto pChar = PyString_AsString(pObj);
        new (storage) std::vector<char>(pChar, std::next(pChar, PyObject_Length(pObj)));
    }
};

template<int...> struct seq {};
template<int N, int... S> struct gens : gens<N - 1, N - 1, S...> {};
template<int... S> struct gens<0, S...> { typedef seq<S...> type; };

template <typename... Ts>
inline std::tuple<Ts...> createTuple(const boost::python::tuple& t)
{
    return createTuple<Ts...>(t, typename gens<sizeof...(Ts)>::type());
}

template<typename... Ts, int... S>
inline std::tuple<Ts...> createTuple(const boost::python::tuple& t, seq<S...>)
{
    return std::make_tuple((static_cast<Ts>(boost::python::extract<Ts>(t[S])))...);
}

template<typename... Ts>
struct PyConverter<boost::python::tuple, std::tuple<Ts...>>
{
    void operator()(PyObject* pObj, void* storage)
    {
        namespace bp = boost::python;

        bp::handle<> hndl(bp::borrowed(pObj));
        bp::tuple t = bp::extract<bp::tuple>(bp::object(hndl));

        new(storage) std::tuple<Ts...>(createTuple<Ts...>(t));
    }
};

template<typename T, typename S>
struct PyConverter<boost::python::tuple, std::pair<T, S>>
{
    void operator()(PyObject* pObj, void* storage)
    {
        namespace bp = boost::python;

        bp::handle<> hndl(bp::borrowed(pObj));
        bp::tuple t = bp::extract<bp::tuple>(bp::object(hndl));

        new(storage) std::pair<T, S>(bp::extract<T>(t[0]), bp::extract<S>(t[1]));
    }
};

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
        PyTypeChecker<PyType, CType> checker;
		if (!checker(pObj))
			return nullptr;

		return pObj;
	}

	// Convert obj_ptr into a QString
	static void construct(PyObject* pObj,
						  boost::python::converter::rvalue_from_python_stage1_data* data)
	{
		// Grab pointer to memory into which to construct the new CType
		void* storage = 
            reinterpret_cast<boost::python::converter::rvalue_from_python_storage<CType>*>(data)->storage.bytes;

        PyConverter<PyType, CType> converter;
        converter(pObj, storage);

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
		
		// Grab pointer to memory into which to construct the new QString
		void* storage = (
			(converter::rvalue_from_python_storage<Container>*)
			data)->storage.bytes;

        using value_type = typename Container::value_type;
        using reference = typename Container::reference;
        using iterator = boost::mpl::if_c<std::is_reference<reference>::value, 
            stl_input_iterator<value_type>,
            typename Container::iterator>::type;

		handle<> hndl(borrowed(pObj));
		new (storage) Container(iterator(object(hndl)), iterator());

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
        namespace bp = boost::python;
        namespace bf = boost::fusion;
        using PythonHelper::setToDict;

        bp::dict d;
        apply(bf::begin(s), bf::end(s), [&d, &s](const std::string& name, auto& member)
        {
            if (s.isInitialized.find(name) != s.isInitialized.end())
            {
                setToDict(d, name, member);
            }
        });

        return bp::incref(d.ptr());
	}
};

struct BoostPTimeToPyDateTimeConverter
{
    static PyObject* convert(const boost::posix_time::ptime& time)
    {
        auto d = time.date();
        auto tod = time.time_of_day();
        auto usec = static_cast<int>(tod.total_microseconds() % 1000000);

        PyDateTime_IMPORT;
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

/*!
convert boost::variant to python
from https://blind.guru/boost_python-and-boost_variant.html
*/
template<typename... Ts>
struct BoostVariantToPyConverter : boost::static_visitor<PyObject *>
{
    static result_type convert(const boost::variant<Ts...>& v)
    {
        return boost::apply_visitor(BoostVariantToPyConverter(), v);
    }

    template<typename T>
    result_type operator()(T const &t) const
    {
        namespace bp = boost::python;
        return boost::python::incref(bp::converter::arg_to_python<T>(t).get());
    }
};


} // namepsace PythonHelper
