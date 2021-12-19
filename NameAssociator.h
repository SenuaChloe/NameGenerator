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
        using SymbolNameAssociation = std::map<Symbol, NameEntity>;
        using ConsonanceList = std::vector<SyllableAssociator::ConsonanceLabel>;
        struct ConsoncancePossibilities { ConsonanceList first_names, last_names, particles; };
        using FullNameFormat = std::string;
        struct FullNameMixup { FullNameFormat format; ConsoncancePossibilities possibilities; };
        using FullNameDict = std::map<RegionLabel, FullNameMixup>;




        NameAssociator(SyllableAssociator syllable_reader, SymbolNameAssociation symbol_name_association);
        virtual ~NameAssociator();

    private:
};
