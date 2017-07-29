#include "error_log.h"


ErrorLog::ErrorLog()
{
	
}

void ErrorLog::DeleteOld() const
{
	std::ofstream errorFile;
	errorFile.open( Game::GetConfigDir() + ERRLOG_FILENAME, std::ofstream::out | std::ofstream::trunc );
	errorFile.close();
}

void ErrorLog::Write( const std::string error ) const
{
	std::ofstream errorFile;
	errorFile.open( Game::GetConfigDir() + ERRLOG_FILENAME, std::ios_base::app );
	errorFile << error << std::endl;
	errorFile.close();
}

void ErrorLog::WriteWithReplace( std::string error, const std::string replacements[], const int numReplacements ) const
{
	for ( int i = 0; i < numReplacements; i++ ) // Using a C array for efficiency
	{
		size_t startPos = error.find( std::to_string( i ) );
		if ( startPos == std::string::npos )
			return;
		error.replace( startPos, std::to_string( i ).length(), replacements[i]);
	}

	Write( error );
}

ErrorLog::~ErrorLog()
{
}