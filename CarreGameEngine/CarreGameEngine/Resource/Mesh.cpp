#include "Mesh.h"

const std::string& Mesh::GetFilePath() const
{
	return m_filePath;
}

void Mesh::SetFilePath(const std::string & filePath)
{
	m_filePath = filePath;
}
