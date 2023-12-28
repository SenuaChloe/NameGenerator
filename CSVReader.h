#pragma once

#include <filesystem>
#include <vector>

class CSVReader
{
    public:
        CSVReader(const std::filesystem::path & filepath);

        std::vector<std::vector<std::string>>::iterator begin();
        std::vector<std::vector<std::string>>::iterator end();
        std::vector<std::vector<std::string>>::const_iterator cbegin() const;
        std::vector<std::vector<std::string>>::const_iterator cend() const;

        static const std::vector<char> C_SEPARATORS;

    private:
        std::vector<std::vector<std::string>> m_csv_data;

};

