#include "tempi/log.h"

int main(int argc, char *argv[])
{
    using namespace tempi;
    Logger& logger = Logger::getInstance();
    logger.setLevel(ERROR);
    logger.log(DEBUG, "DEBUG");
    logger.log(INFO, "INFO");
    logger.log(WARNING, "WARNING");
    logger.log(CRITICAL, "CRITICAL");
    //logger.log(ERROR, "ERROR");
    return 0;
}

