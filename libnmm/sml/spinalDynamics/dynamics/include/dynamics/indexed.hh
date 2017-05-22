#ifndef __INDEXED_HH__
#define __INDEXED_HH__

#include <dynamics/config.hh>

/**
 * @brief Class used to associate a unique index to object that inherit from this class
 * @details Each object created from a class inheriting from Indexed will have a unique const field id.
 */
class Indexed
{
 public:
    Indexed();
    /**
     * Constant field that can be used as a unique identifier to the associated object.
     */
    const UInt id;

 private:
    static UInt idCounter;
};

#endif