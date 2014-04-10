#include <unistd.h>

#include "FilesMaster.h"

FilesMaster::FilesMaster()
{
}

FilesMaster::~FilesMaster()
{
}

int FilesMaster::AccessFile(char* file_name)
{
	FileBuffer* FileBufferp = FindFile(file_name);
	if(FileBufferp != NULL)
	{
		return 0;
	}
	
	int ret = 0;	
	ret = access(file_name, R_OK);
	return ret;
}

CFile*	FilesMaster::OpenFile(char* file_name)
{
	FileBuffer* FileBufferp = FindFile(file_name);
	if(FileBufferp == NULL)
	{
		FileBufferp = new FileBuffer();	
		int ret = 0;
		ret = FileBufferp->Open(file_name);
		if(ret < 0)
		{
			delete FileBufferp;
			FileBufferp = NULL;
			return NULL;
		}
		ret = this->AddFileBuffer(FileBufferp);
		if(ret < 0)
		{
			delete FileBufferp;
			FileBufferp = NULL;
			return NULL;
		}
	}	
	
	CFile* filep = new CFile(FileBufferp);
	return filep;
}


FileBuffer*	FilesMaster::FindFile(char* file_name)
{
	// todo:
	return NULL;
}

int	FilesMaster::AddFileBuffer(FileBuffer* FileBufferp)
{
	// todo:
	return 0;
}
