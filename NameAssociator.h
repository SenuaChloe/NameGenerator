#pragma once

#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include "SyllableAssociator.h"
#include "SymbolPairing.h"

class NameAssociator
{
    public:

        using RegionLabel = std::string;
        using ConsonanceList = std::vector<SyllableAssociator::ConsonanceLabel>;
        struct ConsoncancePossibilities { ConsonanceList first_names, last_names, particles; };
        using FullNameFormat = std::string;
        struct FullNameMixup { FullNameFormat format; ConsoncancePossibilities possibilities; };
        using FullNameDict = std::map<RegionLabel, FullNameMixup>;

        /**
        ** @brief Constructor of the classs. Calls the CSVReader class
        ** @param filepath : path to the CSV file to parse
        ** @param syllable_reader : an instance of the Syllable Associator
        ** @param SymbolPairing : How the format symbol should be read
        **/
        NameAssociator(const std::filesystem::path & filepath, SyllableAssociator && syllable_reader, SymbolPairing && symbol_pairing);

        /**
        ** @brief computes the list of available regions and returns it
        ** @return the list of regions available in the dictionary
        **/
        std::vector<RegionLabel> get_region_list() const;

        /**
        ** @brief computes the list of available regions and returns it (static version only : only reads the first column of the file)
        ** @param filepath : path to the CSV file to parse
        ** @return the list of regions available in the dictionary
        **/
        static std::vector<RegionLabel> get_region_list(const std::filesystem::path & filepath);

        /**
        ** @brief generates a random name given a region
        ** @param region : the desired region
        ** @return the generated name
        **/
        std::string generate_random_full_name(const RegionLabel & region) const;

        /**
        ** @brief generates a formated string of the full name format
        ** @param region : the desired region
        ** @return the format formated
        **/
        std::string get_formated_full_name_format(const RegionLabel & region) const;


    private:

        SyllableAssociator m_syllable_reader;
        SymbolPairing m_symbol_pairing;
        FullNameDict m_full_name_dict;
};
