
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "FileBuffer.h"

FileBuffer::FileBuffer()
{
	m_fd			= -1;
	m_FileName		= NULL;
	m_AccessCount	= 0;
	m_FileLength	= -1;
	dequeh_init(&m_PiecesDeque);
}

FileBuffer::~FileBuffer()
{
	// todo:
}

int FileBuffer::Open(char* file_name)
{	
	m_FileName = strdup(file_name);
	if(m_FileName == NULL)
	{
		return -1;
	}
	
	m_fd = open(m_FileName, O_RDONLY);
	if(m_fd < 0)
	{
		return -1;
	}

	m_FileLength = lseek(m_fd, 0, SEEK_END);
	lseek(m_fd, 0, SEEK_SET);
	return 0;
}

int64_t FileBuffer::GetFileLength()
{
	return m_FileLength;
}

int64_t FileBuffer::Read(int64_t file_offset, void* buffer, u_int64_t size)
{
	// todo:
	// find piece from piece deque.
	lseek(m_fd, file_offset, SEEK_SET);
	int64_t read_len = read(m_fd, buffer, size);
	return read_len;
}

CFile::CFile(FileBuffer* FileBufferp)
{
	m_FileBuffer = FileBufferp;
}

CFile::~CFile()
{
	
}

int64_t CFile::GetFileLength()
{
	int64_t file_len = m_FileBuffer->GetFileLength();
	return file_len;
}

int64_t CFile::Read(void* buffer, u_int64_t size)
{
	int64_t read_len = m_FileBuffer->Read(m_FileOffset, buffer, size);
	return read_len;
}

