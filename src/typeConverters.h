#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
MSC_RESTORE_WARNINGS
#include "Song.h"
#include "AlbumArt.h"

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
template<typename CType, typename PyType>
inline void pyConvert(PyObject* pObj, void* storage)
{
	using namespace boost::python;

	handle<> hndl(borrowed(pObj));
	new (storage) CType(extract<PyType>(object(hndl)));
}

template<>
inline void pyConvert<std::string, boost::python::str>(PyObject* pObj, void* storage)
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

		pyConvert<CType, PyType>(pObj, storage);

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

template<typename DictStruct>
struct DictStructToPyDicCoverter
{
	static PyObject* convert(const DictStruct& dictStruct)
	{
		return dictStruct.m_dict.ptr();
	}
};

} // namepsace GMusicApi