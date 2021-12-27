#pragma once

#include <map>
#include <filesystem>


enum class NameType
{
    FIRST_NAME,
    LAST_NAME,
    PARTICLE
};

using Symbol = char;
struct NameEntity { NameType name_type; unsigned int multiplicity; };

class SymbolPairing : public std::map<Symbol, NameEntity>
{
    public:

        /**
        ** @brief Constructor of the classs. Calls the CSVReader class
        ** @param filepath : path to the CSV file to parse
        **/
        SymbolPairing(const std::filesystem::path & filepath);
};
