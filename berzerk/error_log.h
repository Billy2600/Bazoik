#pragma once
#include <fstream>
#include <algorithm>
#include <string>
#include "game.h"

#define ERRLOG_FILENAME "errorlog.txt"
#define ERROR_READ_FILE "Error reading '0': Did not find 1"

class ErrorLog
{
public:
	ErrorLog();
	void DeleteOld() const; // Delete contents of old log (should only run once on startup)
	void Write( const std::string error ) const; // Writes to file
	// Will replace numbers in the string with the matching value in the array, then write it out; you must provide the number of replacements
	void WriteWithReplace( std::string error, const std::string replacements[], const int numReplacements ) const;
	~ErrorLog();
};

