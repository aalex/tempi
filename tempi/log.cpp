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

#include "tempi/log.h"
#include <iostream>

namespace tempi {

Logger& Logger::getInstance()
{
    static Logger instance; // Guaranteed to be destroyed.
    return instance; // Instantiated on first use.
}

void Logger::setLevel(LogLevel level)
{
    level_ = level;
    log4cpp::Category& category = log4cpp::Category::getInstance(category_name_);
    category.setPriority(level_);
}

bool Logger::isEnabledFor(LogLevel level)
{
    return level <= Logger::getInstance().getLevel();
}

LogLevel Logger::getLevel()
{
    return level_;
}

void Logger::log(LogLevel level, const std::ostringstream &os)
{
    Logger::log(level, os.str().c_str());
}

void Logger::log(LogLevel level, const std::string &text)
{
    Logger::log(level, text.c_str());
}

void Logger::log(LogLevel level, const char *message)
{
    Logger &self = Logger::getInstance();
    log4cpp::Category& category = log4cpp::Category::getInstance(self.category_name_);
    switch (level)
    {
        case DEBUG:
            category.debug(message);
            break;
        case INFO:
            category.info(message);
            break;
        case WARNING:
            category.warn(message);
            break;
        case CRITICAL:
            category.crit(message);
            break;
        case ERROR:
            category.error(message);
            break;
    }
}

Logger::~Logger()
{
    // No need to delete appender_ and layout_
}

Logger::Logger()
{
    appender_ = new log4cpp::OstreamAppender("console", &std::cout);
    layout_ = new log4cpp::PatternLayout();
    layout_->setConversionPattern("%d{%H:%M:%S,%l} %8p - %m %n"); // %d: 
    appender_->setLayout(layout_);
    log4cpp::Category& category = log4cpp::Category::getInstance(category_name_);
    category.setAppender(appender_);
    level_ = WARNING; // default
    category.setPriority(level_); //default
}

const char * const Logger::category_name_ = "default";

} // end of namespace

