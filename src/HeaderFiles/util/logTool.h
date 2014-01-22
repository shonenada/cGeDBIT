#ifndef _LOGSPACE_
#define _LOGSPACE_


#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/FileAppender.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

namespace logspace
{
	
	extern int _flag_;

	Logger logToFile();
	Logger logToConsole();

}

#endif