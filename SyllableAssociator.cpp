#include "SyllableAssociator.h"

#include <iostream>
#include "CSVReader.h"
#include "ErrorHandler.h"


/******************************************************************************/
SyllableAssociator::SyllableAssociator(const std::filesystem::path & filepath)
{
    CSVReader csv_reader(filepath);

    // Filling the syllable dictionary with the data from the CSV
    for (const auto & line : csv_reader)
    {
        auto it = line.cbegin();
        const auto consonance = *it++;
        const auto & type_label = *it++;

        if (type_label == C_RANGE_STRING)
        {
            IntRange count_range;
            count_range.min = std::stoi(*it++);
            count_range.max = std::stoi(*it++);
            m_syllable_dictionary[consonance].syllable_count_range = count_range;
        }
        else
        {
            if (C_SYLLABLE_PAIRING.find(type_label) == C_SYLLABLE_PAIRING.cend())
                ErrorHandler::raise_error("Error: Syllable type '", type_label, "' is unknown");

            const auto type = C_SYLLABLE_PAIRING.at(type_label);
            for ( ; it != line.cend() ; ++it)
            {
                if (*it != "")
                    m_syllable_dictionary[consonance].syllables[type].push_back(*it);
            }
        }
    }
}


/******************************************************************************/
std::vector<SyllableAssociator::ConsonanceLabel> SyllableAssociator::get_consonance_list() const
{
    std::vector<std::string> consonance_list;
    for (const auto & item : m_syllable_dictionary)
        consonance_list.push_back(item.first);
    return consonance_list;
}


/******************************************************************************/
static std::string syllable_type_to_string(SyllableAssociator::SyllableType type)
{
    switch (type)
    {
        break; case SyllableAssociator::SyllableType::PREFIX:
            return "PREFIX";
        break; case SyllableAssociator::SyllableType::MIDDLE:
            return "MIDDLE";
        break; case SyllableAssociator::SyllableType::SUFFIX:
            return "SUFFIX";
        break; case SyllableAssociator::SyllableType::PARTICLE:
            return "PARTICLE";
    }
}

/******************************************************************************/
std::string SyllableAssociator::get_random_syllable(const ConsonanceLabel & consonance, SyllableType type) const
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
int SyllableAssociator::get_random_syllable_count(const ConsonanceLabel & consonance) const
{
    if (m_syllable_dictionary.count(consonance) == 0)
        ErrorHandler::raise_error("Consonance '", consonance, "' not found");

    const IntRange & range = m_syllable_dictionary.at(consonance).syllable_count_range;
    const int modulo = range.max-range.min+1;
    const int rcount = std::rand() % modulo + range.min;
    return rcount;
}


/******************************************************************************/

std::string SyllableAssociator::generate_random_name(const ConsonanceLabel & consonance) const
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
std::string SyllableAssociator::generate_random_particle(const ConsonanceLabel & consonance) const
{
    return get_random_syllable(consonance, SyllableType::PARTICLE);
}

