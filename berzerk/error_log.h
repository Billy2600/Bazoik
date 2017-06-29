#pragma once
#include <fstream>
#include <string>
#include "game.h"

#define ERRLOG_FILENAME "errorlog.txt"

class ErrorLog
{
public:
	ErrorLog();
	void DeleteOld() const; // Delete contents of old log (should only run once on startup)
	void Write( const std::string error ) const; // Writes to file
	~ErrorLog();
};

