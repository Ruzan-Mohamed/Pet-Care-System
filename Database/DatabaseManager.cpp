#include "DatabaseManager.h"

using namespace PawCare;

DatabaseManager::DatabaseManager()
{
    connected = false;
}

bool DatabaseManager::connect()
{
    // MySQL connection will be implemented in Part 6.
    connected = false;
    return connected;
}

void DatabaseManager::disconnect()
{
    connected = false;
}

bool DatabaseManager::isConnected()
{
    return connected;
}
