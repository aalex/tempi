/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * Logging utilities
 */

#ifndef __TEMPI_LOG_H__
#define __TEMPI_LOG_H__

#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <sstream> // to provide std::ostringstream

namespace tempi {

/**
 * Log level
 */
typedef enum
{
    DEBUG = log4cpp::Priority::DEBUG,
    INFO = log4cpp::Priority::INFO,
    WARNING = log4cpp::Priority::WARN,
    CRITICAL = log4cpp::Priority::CRIT,
    ERROR = log4cpp::Priority::ERROR
} LogLevel;

class Logger
{
    public:
        /**
         * Retrieves the singleton.
         */
        static Logger& getInstance();
        /**
         * Sets log level.
         */
        void setLevel(LogLevel level);
        /**
         * Logs a message.
         */
        static log4cpp::Category& log(LogLevel level, const char *message);
        ~Logger();
    private:
        log4cpp::Appender *appender_;
        log4cpp::PatternLayout *layout_;
        static const char * const category_name_;
        // Private constructor
        Logger();
        // Dont forget to declare these two. You want to make sure they
        // are unaccessable otherwise you may accidently get copies of
        // your singelton appearing.
        Logger(Logger const&);         // Don't Implement
        void operator=(Logger const&); // Don't implement
};

} // end of namespace

#endif // ifndef

