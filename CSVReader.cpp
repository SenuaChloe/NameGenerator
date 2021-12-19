#include "CSVReader.h"

#include <fstream>
#include <algorithm>
#include <iostream>

const std::vector<char> CSVReader::C_SEPARATORS = {',',';'};

CSVReader::CSVReader(const std::filesystem::path & filepath)
{
    /// @TODO À protéger

    // Create an input filestream
    std::ifstream csv_file(filepath);

    std::string line;

    // Read data, line by line
    while(std::getline(csv_file, line))
    {
        if (line.size() == 0)
            continue;

        // Getting and splitting the line
        std::vector<std::string> split_line;

        size_t left = 0;
        for (size_t right = 0 ; right < line.size() ; ++right)
        {
            if (std::find(C_SEPARATORS.cbegin(), C_SEPARATORS.cend(), line[right]) != C_SEPARATORS.cend())
            {
                split_line.push_back(line.substr(left, right-left));
                left = right+1;
            }
        }
        split_line.push_back(line.substr(left, line.size()-left));

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
