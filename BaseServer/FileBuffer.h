#ifndef __FILEBUFFER_H__
#define __FILEBUFFER_H__

#include <sys/types.h>
#include "dequeH.h"

typedef struct piece_t
{
	int		access_count;
	int 	size;
	int 	len;
	void* 	data;	
} PIECE_T;

class FileBuffer
{
public:
	FileBuffer();
	virtual ~FileBuffer();
	int			Open(char* file_name);
	int64_t		GetFileLength();
	int64_t		Read(int64_t offset, void* buffer, u_int64_t size);
protected:
	int 		m_fd;
	char*		m_FileName;
	int			m_AccessCount;
	int64_t		m_FileLength;
	DEQUEH_T	m_PiecesDeque;
};

// CFile: ClassFile, which is similiar to FILE(man fopen).
class  CFile
{
public:
	CFile(FileBuffer* FileBufferp);
	virtual ~CFile();
	int64_t		GetFileLength();
	int64_t		Read(void* buffer, u_int64_t size);
	
protected:
	FileBuffer*		m_FileBuffer;
	int64_t			m_FileOffset;
};

#endif