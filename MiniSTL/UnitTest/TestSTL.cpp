/*
** TestSTL.cpp
** Created by Rayn on 2015/12/22
*/
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main( int argc, char* argv[]) {
    Catch::Session session;
    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0)
        return returnCode;
    return session.run();
}