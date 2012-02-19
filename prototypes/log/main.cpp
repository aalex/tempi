#include <iostream>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>

namespace log {

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
        static Logger& getInstance()
        {
            static Logger instance; // Guaranteed to be destroyed.
            return instance; // Instantiated on first use.
        }
        /**
         * Sets log level.
         */
        void setLevel(LogLevel level)
        {
            log4cpp::Category& category = log4cpp::Category::getInstance(category_name_);
            category.setPriority(level);
        }
        /**
         * Logs a message.
         */
        log4cpp::Category& log(LogLevel level, const char *message)
        {
            log4cpp::Category& category = log4cpp::Category::getInstance(category_name_);
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
        ~Logger()
        {
            // No need to delete appender_ and layout_
        }
    private:
        log4cpp::Appender *appender_;
        log4cpp::PatternLayout *layout_;
        static const char * const category_name_;

        // Private constructor
        Logger()
        {
            appender_ = new log4cpp::OstreamAppender("console", &std::cout);
            layout_ = new log4cpp::PatternLayout();
            layout_->setConversionPattern("%d: %8p - %m %n");
            appender_->setLayout(layout_);
            log4cpp::Category& category = log4cpp::Category::getInstance(category_name_);
            category.setAppender(appender_);
            category.setPriority(WARNING); //default
        }
        // Dont forget to declare these two. You want to make sure they
        // are unaccessable otherwise you may accidently get copies of
        // your singelton appearing.
        Logger(Logger const&);         // Don't Implement
        void operator=(Logger const&); // Don't implement
};

const char * const Logger::category_name_ = "default";

} // end of namespace

int main(int argc, char *argv[])
{
    using namespace log;
    Logger& logger = Logger::getInstance();
    logger.setLevel(INFO);
    logger.log(DEBUG, "DEBUG");
    logger.log(INFO, "INFO");
    logger.log(WARNING, "WARNING");
    logger.log(CRITICAL, "CRITICAL");
    logger.log(ERROR, "ERROR");
    return 0;
}

