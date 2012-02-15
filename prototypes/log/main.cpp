#include <iostream>
#include <tr1/memory>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/SimpleLayout.hh>
 
static const char * const LOGFILE = "/tmp/log4cpp-test.log";
 
int main(int argc, char *argv[])
{
    // Setting up Appender, layout and Category
    // log4cpp::Appender *appender = new log4cpp::FileAppender("logfile-manager", LOGFILE);
    log4cpp::Appender *appender = new log4cpp::OstreamAppender("OstreamAppender", &std::cout);
    log4cpp::Layout *layout = new log4cpp::SimpleLayout();
    log4cpp::Category& category = log4cpp::Category::getInstance("hello");
 
    appender->setLayout(layout);
    category.setAppender(appender);
    category.setPriority(log4cpp::Priority::WARN); // INFO);
 
    // The actual logging
    category.info("This is for tracing the flow");
    category.notice("This is to notify certain events");
    category.warn("This is to generate certain warnings");
    return 0;
}

