#include <iostream>

#include "SyllablesReader.h"


using namespace std;

int main()
{
    map<std::string, SyllablesReader::SyllableType> as;
    as["Debut"] = SyllablesReader::SyllableType::PREFIX;
    as["Milieu"] = SyllablesReader::SyllableType::MIDDLE;
    as["Fin"] = SyllablesReader::SyllableType::SUFFIX;
    as["Particule"] = SyllablesReader::SyllableType::PARTICLE;

    const std::string rs = "Syllabes";

    SyllablesReader sr("Syllabes.csv", as, rs);
    auto l = sr.get_consonance_list();

    for (auto item : l)
    {
        std::cout << sr.get_random_syllable_count(item)
            << " " << sr.get_random_syllable(item, SyllablesReader::SyllableType::PREFIX)
            << " " << sr.get_random_syllable(item, SyllablesReader::SyllableType::MIDDLE)
            << " " << sr.get_random_syllable(item, SyllablesReader::SyllableType::SUFFIX)
            << std::endl;
    }

    return 0;
}
