#include "config.hh"
#include "indexed.hh"

UInt Indexed::idCounter = 0;

Indexed::Indexed() 
	: id( idCounter++ )
{ 
}