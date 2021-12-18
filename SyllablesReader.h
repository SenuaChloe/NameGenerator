#pragma once

#include <filesystem>
#include <string>
#include <map>
#include <vector>

class SyllablesReader
{
    public:

        enum class SyllableType {
            PREFIX,
            MIDDLE,
            SUFFIX,
            PARTICLE
        };


        /**
        ** @brief Constructor of the classs. Calls the CSVReader class
        ** @param filepath : path to the CSV file to parse
        ** @param syllable_string_pairing : a map matching the dufferent type of syllables and the string it is refered to in the CSV file
        **/
        SyllablesReader(const std::filesystem::path & filepath, const std::map<std::string, SyllableType> & syllable_string_pairing, const std::string & count_range_string);

        ~SyllablesReader();

        /**
        ** @brief fetches a random syllable matching the given sriterias
        ** @param consonance : the desired consonance
        ** @param type : the desired type
        ** @return a random syllable from the desired consonance and type
        **/
        std::string get_random_syllable(const std::string & consonance, SyllableType type) const;

        /**
        ** @brief gives a random number of syllables matching a given consonance
        ** @param consonance : the desired consonance
        ** @return a random syllable count from the desired consonance
        **/
        int get_random_syllable_count(const std::string & consonance) const;

        /**
        ** @brief computes the list of consonances and returns it
        ** @return the list of consonances available in the dictionary
        **/
        std::vector<std::string> get_consonance_list() const;

    private:
        struct IntRange { int min, max; };
        using SyllableList = std::vector<std::string>;
        using SyllableGroup = std::map<SyllableType, SyllableList>;
        struct ConsonanceGroup { IntRange syllable_count_range; SyllableGroup syllables; };
        using ConsonanceName = std::string;
        using SyllableDict = std::map<ConsonanceName, ConsonanceGroup>;
        SyllableDict m_syllable_dictionary;
};
