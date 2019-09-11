#ifndef _PYTHON27_FILTER_H
#define _PYTHON27_FILTER_H
/*
 * Fledge "Python 2.7" filter class.
 *
 * Copyright (c) 2019 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Massimiliano Pinto
 */

#include <mutex>

#include <filter_plugin.h>
#include <filter.h>

#include <Python.h>

// Relative path to FLEDGE_DATA
#define PYTHON_FILTERS_PATH "/scripts"

/**
 * Python27Filter class is derived from FledgeFilter
 * It handles loading of a python module (provided script name)
 * and the Python2.7 C API calls
 */
class Python27Filter : public FledgeFilter
{
	public:
		Python27Filter(const std::string& name,
			       ConfigCategory& config,
			       OUTPUT_HANDLE* outHandle,
			       OUTPUT_STREAM output) :
			       FledgeFilter(name,
					config,
					outHandle,
					output)
		{
			m_pModule = NULL;
			m_pFunc = NULL;
		};

		// Set the additional path for Python3.5 Fledge scripts
		void	setFiltersPath(const std::string& dataDir)
		{
			// Set Fledge dataDir + filters dir
			m_filtersPath = dataDir + PYTHON_FILTERS_PATH;
		}
		const std::string&
			getFiltersPath() const { return m_filtersPath; };
		bool	setScriptName();
		bool	configure();
		bool	reconfigure(const std::string& newConfig);
		void	lock() { m_configMutex.lock(); };
		void	unlock() { m_configMutex.unlock(); };
		void	logErrorMessage();
		// Filtering methods for Reading objects
		PyObject*
			createReadingsList(const std::vector<Reading *>& readings);
		std::vector<Reading *>*
			getFilteredReadings(PyObject* filteredData);

	public:
		// Python 3.5 loaded filter module handle
		PyObject*	m_pModule;
		// Python 3.5 callable method handle
		PyObject*	m_pFunc;
		// Python 3.5  script name
		std::string	m_pythonScript;

	private:
		// Scripts path
		std::string	m_filtersPath;
		// Configuration lock
		std::mutex	m_configMutex;
};
#endif
