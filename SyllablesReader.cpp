#include <iostream>
#include "CSVReader.h"
#include "ErrorHandler.h"

#include "SyllablesReader.h"

/******************************************************************************/
SyllablesReader::SyllablesReader(const std::filesystem::path & filepath, const std::map<std::string, SyllableType> & syllable_string_pairing, const std::string & count_range_string)
{
    CSVReader csv_reader(filepath);

    // Filling the syllable dictionary with the data from the CSV
    for (const auto & line : csv_reader)
    {
        auto it = line.cbegin();
        const auto consonance = *it++;
        const auto & type_label = *it++;

        if (type_label == count_range_string)
        {
            IntRange count_range;
            count_range.min = std::stoi(*it++);
            count_range.max = std::stoi(*it++);
            m_syllable_dictionary[consonance].syllable_count_range = count_range;
        }
        else
        {
            if (syllable_string_pairing.find(type_label) == syllable_string_pairing.cend())
                ErrorHandler::raise_error("Error: Syllable type '", type_label, "' is unknown");

            const auto type = syllable_string_pairing.at(type_label);
            for ( ; it != line.cend() ; ++it)
                m_syllable_dictionary[consonance].syllables[type].push_back(*it);
        }
    }
}


/******************************************************************************/
SyllablesReader::~SyllablesReader()
{
}


/******************************************************************************/
std::vector<std::string> SyllablesReader::get_consonance_list() const
{
    std::vector<std::string> consonance_list;
    for (const auto & item : m_syllable_dictionary)
        consonance_list.push_back(item.first);
    return consonance_list;
}


/******************************************************************************/
static std::string syllable_type_to_string(SyllablesReader::SyllableType type)
{
    switch (type)
    {
        break; case SyllablesReader::SyllableType::PREFIX:
            return "PREFIX";
        break; case SyllablesReader::SyllableType::MIDDLE:
            return "MIDDLE";
        break; case SyllablesReader::SyllableType::SUFFIX:
            return "SUFFIX";
        break; case SyllablesReader::SyllableType::PARTICLE:
            return "PARTICLE";
    }
}

/******************************************************************************/
std::string SyllablesReader::get_random_syllable(const std::string & consonance, SyllableType type) const
{
    if (m_syllable_dictionary.count(consonance) == 0)
        ErrorHandler::raise_error("Consonance '", consonance, "' not found");
    else if (m_syllable_dictionary.at(consonance).syllables.count(type) == 0)
        ErrorHandler::raise_error("Type '", syllable_type_to_string(type), "' for consonance '", consonance, "' not found");

    const SyllableList & available_syllables = m_syllable_dictionary.at(consonance).syllables.at(type);
    const size_t modulo = available_syllables.size();
    const size_t rindex = static_cast<size_t>(std::rand()) % modulo;
    return available_syllables[rindex];
}


/******************************************************************************/
int SyllablesReader::get_random_syllable_count(const std::string & consonance) const
{
    if (m_syllable_dictionary.count(consonance) == 0)
        ErrorHandler::raise_error("Consonance '", consonance, "' not found");

    const IntRange & range = m_syllable_dictionary.at(consonance).syllable_count_range;
    const int modulo = range.max-range.min+1;
    const int rcount = std::rand() % modulo + range.min;
    return rcount;
}


/******************************************************************************/

std::string SyllablesReader::generate_random_name(const std::string & consonance) const
{
    std::string result;
    const int s_count = std::max(get_random_syllable_count(consonance)-2, 0);
    result += get_random_syllable(consonance, SyllableType::PREFIX);
    for (int i = 0 ; i < s_count ; ++i)
        result += get_random_syllable(consonance, SyllableType::MIDDLE);
    result += get_random_syllable(consonance, SyllableType::SUFFIX);

    return result;
}


/******************************************************************************/
std::string SyllablesReader::generate_random_particle(const std::string & consonance) const
{
    return get_random_syllable(consonance, SyllableType::PARTICLE);
}

