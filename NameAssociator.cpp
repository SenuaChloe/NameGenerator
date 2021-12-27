#include "NameAssociator.h"

#include <utility>
#include "CSVReader.h"
#include "ErrorHandler.h"

/******************************************************************************/
NameAssociator::NameAssociator(const std::filesystem::path & filepath, SyllableAssociator && syllable_reader, SymbolPairing && symbol_pairing):
            m_syllable_reader(std::forward<SyllableAssociator>(syllable_reader)),
            m_symbol_pairing(std::forward<SymbolPairing>(symbol_pairing))
{
    CSVReader csv_reader(filepath);

    auto it_line = csv_reader.cbegin();

    // Getting data from the first line

    ErrorHandler::assert(it_line->size() >= 3, "File format is wrong (initialization data)");

    auto it = it_line->cbegin();

    const int first_name_buffer_size = std::stoi(*it++);
    const int last_name_buffer_size = std::stoi(*it++);
    const int particle_buffer_size = std::stoi(*it++);
    ++it_line;

    // Filling the name generator with the data from the CSV
    for (; it_line != csv_reader.cend() ; ++it_line)
    {
        ErrorHandler::assert(it_line->size() >= (2+first_name_buffer_size+last_name_buffer_size+particle_buffer_size), "File format is wrong (content data)");

        auto it = it_line->cbegin();
        const RegionLabel region_label = *it++;
        m_full_name_dict[region_label].format = *it++;

        ConsonanceList first_names;
        ConsonanceList last_names;
        ConsonanceList particles;

        for (size_t i = 0 ; i < first_name_buffer_size ; ++i)
        {
            if (*it != "")
                first_names.push_back(*it);
            ++it;
        }
        for (size_t i = 0 ; i < last_name_buffer_size ; ++i)
        {
            if (*it != "")
                last_names.push_back(*it);
            ++it;
        }
        for (size_t i = 0 ; i < particle_buffer_size ; ++i)
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
std::vector<NameAssociator::RegionLabel> NameAssociator::get_region_list() const
{
    std::vector<RegionLabel> region_list;
    for (const auto & item : m_full_name_dict)
        region_list.push_back(item.first);
    return region_list;
}


/******************************************************************************/
std::vector<NameAssociator::RegionLabel> NameAssociator::get_region_list(const std::filesystem::path & filepath)
{
    std::vector<RegionLabel> region_list;

    CSVReader csv_reader(filepath);

    auto it_line = csv_reader.cbegin();

    ++it_line; // Discarding the first line

    // Filling the name generator with the data from the CSV (taking only the first element of the line)
    for (; it_line != csv_reader.cend() ; ++it_line)
        region_list.push_back(*it_line->cbegin());

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
        if (m_symbol_pairing.find(c) != m_symbol_pairing.end())
        {
            NameEntity name_entity = m_symbol_pairing.at(c);
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


/******************************************************************************/
std::string NameAssociator::get_formated_full_name_format(const RegionLabel & region) const
{
    std::string result;
    const std::string source_string = m_full_name_dict.at(region).format;

    for (size_t ic = 0 ; ic < source_string.size() ; ++ic)
    {
        const char c = source_string.at(ic);
        if (m_symbol_pairing.find(c) != m_symbol_pairing.end())
        {
            switch (m_symbol_pairing.at(c).name_type)
            {
                break; case NameType::FIRST_NAME: result += "[first name]";
                break; case NameType::LAST_NAME: result += "[last name]";
                break; case NameType::PARTICLE: result += "[particle]";
            }
        }
        else
        {
            result += c;
        }
    }

    return result;
}
