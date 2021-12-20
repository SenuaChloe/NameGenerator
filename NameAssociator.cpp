#include "NameAssociator.h"

#include <utility>
#include "CSVReader.h"
#include "ErrorHandler.h"

/******************************************************************************/
NameAssociator::NameAssociator(const std::filesystem::path & filepath, SyllableAssociator && syllable_reader, SymbolNamePairing && symbol_name_association, size_t buffer_size):
            m_syllable_reader(std::forward<SyllableAssociator>(syllable_reader)),
            m_symbol_name_association(std::forward<SymbolNamePairing>(symbol_name_association))
{
    CSVReader csv_reader(filepath);

    // Filling the syllable dictionary with the data from the CSV
    for (const auto & line : csv_reader)
    {
        ErrorHandler::assert(line.size() < (2+3*buffer_size), "File format is wrong");

        auto it = line.cbegin();
        const RegionLabel region_label = *it++;
        m_full_name_dict[region_label].format = *it++;

        ConsonanceList first_names;
        ConsonanceList last_names;
        ConsonanceList particles;

        for (size_t i = 0 ; i < buffer_size ; ++i)
        {
            if (*it != "")
                first_names.push_back(*it);
            ++it;
        }
        for (size_t i = 0 ; i < buffer_size ; ++i)
        {
            if (*it != "")
                last_names.push_back(*it);
            ++it;
        }
        for (size_t i = 0 ; i < buffer_size ; ++i)
        {
            if (*it != "")
                particles.push_back(*it);
            ++it;
        }

        m_full_name_dict[region_label].possibilities.first_names = first_names;
        m_full_name_dict[region_label].possibilities.last_names = last_names;
        m_full_name_dict[region_label].possibilities.particles = particles;
    }
}


/******************************************************************************/
NameAssociator::~NameAssociator()
{
}


/******************************************************************************/
std::vector<NameAssociator::RegionLabel> NameAssociator::get_region_list() const
{
    std::vector<RegionLabel> region_list;
    for (const auto & item : m_full_name_dict)
        region_list.push_back(item.first);
    return region_list;
}


/******************************************************************************/
std::string NameAssociator::generate_random_full_name(const RegionLabel & region) const
{
    std::string full_name_result;

    const FullNameFormat & format = m_full_name_dict.at(region).format;
    for (size_t ic = 0 ; ic < format.size() ; ++ic)
    {
        char c = format[ic];
        if (m_symbol_name_association.find(c) != m_symbol_name_association.end())
        {
            NameEntity name_entity = m_symbol_name_association.at(c);
            std::vector<std::string> consonance_list;
            switch (name_entity.name_type)
            {
                break; case NameType::FIRST_NAME:
                    consonance_list = m_full_name_dict.at(region).possibilities.first_names;
                break; case NameType::LAST_NAME:
                    consonance_list = m_full_name_dict.at(region).possibilities.last_names;
                break; case NameType::PARTICLE:
                    consonance_list = m_full_name_dict.at(region).possibilities.particles;
            }
            size_t consonance_i = rand() % consonance_list.size();
            const SyllableAssociator::ConsonanceLabel & consonance = consonance_list[consonance_i];

            std::string generated_name;
            if (name_entity.name_type == NameType::PARTICLE)
                generated_name = m_syllable_reader.generate_random_particle(consonance);
            else
                generated_name = m_syllable_reader.generate_random_name(consonance);

            for (unsigned int m = 0 ; m < name_entity.multiplicity ; ++m)
                full_name_result += generated_name;
        }
        else
        {
            full_name_result += c;
        }
    }
    return full_name_result;
}
