#include <iostream>
#include "SyllableAssociator.h"


using namespace std;

int main()
{
    std::srand(time(0));

    map<std::string, SyllableAssociator::SyllableType> association_strings;
    association_strings["Debut"] = SyllableAssociator::SyllableType::PREFIX;
    association_strings["Milieu"] = SyllableAssociator::SyllableType::MIDDLE;
    association_strings["Fin"] = SyllableAssociator::SyllableType::SUFFIX;
    association_strings["Particule"] = SyllableAssociator::SyllableType::PARTICLE;

    const std::string range_string = "Syllabes";

    const std::string syllables_filename = "SyllableAssociations.csv";

    SyllableAssociator syllable_reader(syllables_filename, association_strings, range_string);
    auto c_list = syllable_reader.get_consonance_list();

    while (true)
    {
        std::cout << std::endl;
        for (auto c : c_list)
            std::cout << c << " : " << syllable_reader.generate_random_name(c) << std::endl;
        std::cout << std::endl;
        std::system("pause");
    }

    return 0;
}
