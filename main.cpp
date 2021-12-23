#include <iostream>
#include "SyllableAssociator.h"
#include "NameAssociator.h"


const int C_NUMBER_OF_NAMES_GENERATED = 10;

int main()
{
    std::srand(time(0));

    SyllableAssociator::SyllablePairing syllable_pairing;
    syllable_pairing["Debut"] = SyllableAssociator::SyllableType::PREFIX;
    syllable_pairing["Milieu"] = SyllableAssociator::SyllableType::MIDDLE;
    syllable_pairing["Fin"] = SyllableAssociator::SyllableType::SUFFIX;
    syllable_pairing["Particule"] = SyllableAssociator::SyllableType::PARTICLE;

    NameAssociator::SymbolNamePairing symbol_name_pairing;
    symbol_name_pairing['P'] = { NameAssociator::NameType::FIRST_NAME, 1 };
    symbol_name_pairing['N'] = { NameAssociator::NameType::LAST_NAME, 1 };
    symbol_name_pairing['Z'] = { NameAssociator::NameType::PARTICLE, 1 };
    symbol_name_pairing['D'] = { NameAssociator::NameType::PARTICLE, 2 };

    const std::string range_string = "Syllabes";

    const std::filesystem::path syllable_associations_filename = "SyllableAssociations.csv";
    const std::filesystem::path name_associations_filename = "NameAssociations.csv";

    SyllableAssociator syllable_associator(syllable_associations_filename, syllable_pairing, range_string);
    NameAssociator name_associator(name_associations_filename, std::move(syllable_associator), std::move(symbol_name_pairing));

    const auto region_list = name_associator.get_region_list();

    int i = 0;
    std::cout << "Make a choice:" <<std::endl;
    for (auto region: region_list)
        std::cout << "  " << (i++) << ". " << region << std::endl;

    int choice = 0;
    do {
        std::cout << "> ";
        std::cin >> choice;
    } while (choice < 0 || choice >= region_list.size());

    std::cout << std::endl << "Format: " << name_associator.get_formated_full_name_format(region_list[choice]) << std::endl << std::endl;

    for (int i = 0 ; i < C_NUMBER_OF_NAMES_GENERATED ; ++i)
        std::cout << name_associator.generate_random_full_name(region_list[choice]) << std::endl;

    std::cout << std::endl;

    std::system("pause");

    return 0;
}
