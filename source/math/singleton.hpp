#pragma once

template<typename HeirT>
class Singleton
{
public:
    static HeirT &get( )
    {
        static HeirT instance;
        return instance;
    }
};
