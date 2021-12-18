#include "CSVReader.h"

#include <fstream>

//#include <boost/any.hpp>

const std::vector<char> CSVReader::C_SEPARATORS = {',',';'};

CSVReader::CSVReader(const std::filesystem::path & filepath)
{
    // Create an input filestream
    std::ifstream csv_file(filepath);

    std::string line;

    // Read data, line by line
    while(std::getline(csv_file, line))
    {
        // Getting and splitting the line
        std::vector<std::string> split_line;
        //boost::algorithm::split(split_line, line, boost::is_any_of(C_SEPARATORS));
        m_csv_data.push_back(split_line);

        // Trimming the last cells if they are empty
        while (m_csv_data.back().begin() != m_csv_data.back().end() && m_csv_data.back().back() == "")
            m_csv_data.back().pop_back();
    }

    // Close file
    csv_file.close();
}


std::vector<std::vector<std::string>>::iterator CSVReader::begin()
{
    return m_csv_data.begin();
}

std::vector<std::vector<std::string>>::iterator CSVReader::end()
{
    return m_csv_data.end();
}

std::vector<std::vector<std::string>>::const_iterator CSVReader::cbegin() const
{
    return m_csv_data.cbegin();
}

std::vector<std::vector<std::string>>::const_iterator CSVReader::cend() const
{
    return m_csv_data.cend();
}
