#pragma once

#include <filesystem>
#include <string>
#include <map>
#include <vector>

class SyllableAssociator
{
    public:

        enum class SyllableType {
            PREFIX,
            MIDDLE,
            SUFFIX,
            PARTICLE
        };

        struct IntRange { int min, max; };
        using SyllableList = std::vector<std::string>;
        using SyllableGroup = std::map<SyllableType, SyllableList>;
        struct ConsonanceGroup { IntRange syllable_count_range; SyllableGroup syllables; };
        using ConsonanceLabel = std::string;
        using SyllableDict = std::map<ConsonanceLabel, ConsonanceGroup>;
        using SyllablePairing = std::map<std::string, SyllableType>;


        /**
        ** @brief Constructor of the classs. Calls the CSVReader class
        ** @param filepath : path to the CSV file to parse
        ** @param syllable_string_pairing : a map matching the dufferent type of syllables and the string it is refered to in the CSV file
        **/
        SyllableAssociator(const std::filesystem::path & filepath, const SyllablePairing & syllable_string_pairing, const std::string & count_range_string);

        virtual ~SyllableAssociator();

        /**
        ** @brief computes the list of consonances and returns it
        ** @return the list of consonances available in the dictionary
        **/
        std::vector<ConsonanceLabel> get_consonance_list() const;

        /**
        ** @brief generates a random name given a consonance
        ** @param consonance : the desired consonance
        ** @return the generated name
        **/
        std::string generate_random_name(const ConsonanceLabel & consonance) const;

        /**
        ** @brief generates a random particle given a consonance
        ** @param consonance : the desired consonance
        ** @return the generated particle
        **/
        std::string generate_random_particle(const ConsonanceLabel & consonance) const;

    private:

        /**
        ** @brief fetches a random syllable matching the given sriterias
        ** @param consonance : the desired consonance
        ** @param type : the desired type
        ** @return a random syllable from the desired consonance and type
        **/
        std::string get_random_syllable(const ConsonanceLabel & consonance, SyllableType type) const;

        /**
        ** @brief gives a random number of syllables matching a given consonance
        ** @param consonance : the desired consonance
        ** @return a random syllable count from the desired consonance
        **/
        int get_random_syllable_count(const ConsonanceLabel & consonance) const;

    private:
        SyllableDict m_syllable_dictionary;
};
