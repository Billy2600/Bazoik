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

ErrorLog::~ErrorLog()
{
}
