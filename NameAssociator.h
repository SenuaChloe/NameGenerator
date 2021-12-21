#pragma once

#include <map>
#include <vector>
#include <string>
#include "SyllableAssociator.h"

class NameAssociator
{
    public:

        enum class NameType
        {
            FIRST_NAME,
            LAST_NAME,
            PARTICLE
        };

        using RegionLabel = std::string;
        using Symbol = char;
        struct NameEntity { NameType name_type; unsigned int multiplicity; };
        using SymbolNamePairing = std::map<Symbol, NameEntity>;
        using ConsonanceList = std::vector<SyllableAssociator::ConsonanceLabel>;
        struct ConsoncancePossibilities { ConsonanceList first_names, last_names, particles; };
        using FullNameFormat = std::string;
        struct FullNameMixup { FullNameFormat format; ConsoncancePossibilities possibilities; };
        using FullNameDict = std::map<RegionLabel, FullNameMixup>;

        /**
        ** @brief Constructor of the classs. Calls the CSVReader class
        ** @param filepath : path to the CSV file to parse
        ** @param syllable_reader : an instance of the Syllable Associator
        ** @param SymbolNamePairing : How the format symbol should be read
        **/
        NameAssociator(const std::filesystem::path & filepath, SyllableAssociator && syllable_reader, SymbolNamePairing && symbol_name_association);

        virtual ~NameAssociator();

        /**
        ** @brief computes the list of available regions and returns it
        ** @return the list of regions available in the dictionary
        **/
        std::vector<RegionLabel> get_region_list() const;

        /**
        ** @brief generates a random name given a region
        ** @param region : the desired region
        ** @return the generated name
        **/
        std::string generate_random_full_name(const RegionLabel & region) const;


    private:

        SyllableAssociator m_syllable_reader;
        SymbolNamePairing m_symbol_name_association;
        FullNameDict m_full_name_dict;
};
