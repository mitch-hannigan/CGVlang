#include <iostream>
#include <fstream>
#include <sstream>
#include "file_reader.h"
std::string readFile(const std::string &filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("File not found!");
    }

    std::ostringstream oss;
    oss << file.rdbuf(); // Copia o conteúdo do arquivo para o stream
    return oss.str();    // Retorna o conteúdo do stream como uma string
}