#include <iostream>

#include "SyllablesReader.h"


using namespace std;

int main()
{
    std::srand(time(0));

    map<std::string, SyllablesReader::SyllableType> association_strings;
    association_strings["Debut"] = SyllablesReader::SyllableType::PREFIX;
    association_strings["Milieu"] = SyllablesReader::SyllableType::MIDDLE;
    association_strings["Fin"] = SyllablesReader::SyllableType::SUFFIX;
    association_strings["Particule"] = SyllablesReader::SyllableType::PARTICLE;

    const std::string range_string = "Syllabes";

    const std::string syllables_filename = "Syllabes.csv";

    SyllablesReader syllable_reader(syllables_filename, association_strings, range_string);
    auto c_list = syllable_reader.get_consonance_list();

    for (auto c : c_list)
        std::cout << syllable_reader.generate_random_name(c) << std::endl;

    return 0;
}
