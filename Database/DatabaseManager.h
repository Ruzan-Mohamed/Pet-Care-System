#pragma once
using namespace System;

namespace PawCare
{
    public ref class DatabaseManager
    {
    private:
        bool connected;

    public:
        DatabaseManager();

        bool connect();
        void disconnect();
        bool isConnected();
    };
}
