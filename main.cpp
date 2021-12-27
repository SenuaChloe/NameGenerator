#include <iostream>
#include <filesystem>
#include "SyllableAssociator.h"
#include "NameAssociator.h"
#include "ErrorHandler.h"

void usage();


int main(int argc, const char *argv[])
{
    std::filesystem::path syllable_associations_filename = "SyllableAssociations.csv";
    std::filesystem::path name_associations_filename = "NameAssociations.csv";
    std::filesystem::path symbol_pairing_filename = "SymbolPairing.csv";
    int generation_count = 10;
    std::string chosen_region = "";
    bool print_regions = false;

    std::string last_key = "";

    for (int iarg = 1 ; iarg < argc ; ++iarg)
    {
        const std::string sarg = argv[iarg];
        if (sarg[0] == '-')
        {
            if (sarg == "-h" || sarg == "--help")
            {
                usage();
                return 0;
            }
            if (sarg == "-p" || sarg == "--print-region")
            {
                print_regions = true;
                continue;
            }
            ErrorHandler::assert(last_key == "", "Error with '", last_key, "'. It must be followed by an argument (use --help for usage)");
            last_key = sarg;

            if ( sarg != "-c" && sarg != "--count"
              && sarg != "-r" && sarg != "--region"
              && sarg != "-S" && sarg != "--syllable-file"
              && sarg != "-N" && sarg != "--name-file"
              && sarg != "-P" && sarg != "--symbol-file" )
                ErrorHandler::assert(last_key == "", "Error with '", last_key, "'. It must be followed by an argument (use --help for usage)");
        }
        else
        {
            ErrorHandler::assert(last_key != "", "Error with '", sarg, "'. Each argument must follow an option (use --help for usage)");
            if (last_key == "-c" || last_key == "--count")
                generation_count = std::stoi(sarg);
            else if (last_key == "-r" || last_key == "--region")
                chosen_region = sarg;
            else if (last_key == "-S" || last_key == "--syllable-file")
                syllable_associations_filename = sarg;
            else if (last_key == "-N" || last_key == "--name-file")
                name_associations_filename = sarg;
            else if (last_key == "-P" || last_key == "--symbol-file")
                symbol_pairing_filename = sarg;
            last_key = "";

        }
    }

    const auto region_list = NameAssociator::get_region_list(name_associations_filename);

    if (print_regions)
    {
        std::cout << "Here are the available regions in the file" <<
        std::endl << "(from " << name_associations_filename << "):" <<
        std::endl <<
        std::endl;
        for (auto region : region_list)
            std::cout << region << std::endl;
        std::cout  << std::endl;
        return 0;
    }

    std::srand(time(0));

    SymbolPairing symbol_pairing(symbol_pairing_filename);
    SyllableAssociator syllable_associator(syllable_associations_filename);
    NameAssociator name_associator(name_associations_filename, std::move(syllable_associator), std::move(symbol_pairing));

    if (chosen_region == "")
    {
        for (int i = 0 ; i < generation_count ; ++i)
        {
            std::string region = region_list[(rand()%region_list.size())];
            std::cout << std::endl << region << " - Format: " << name_associator.get_formated_full_name_format(region) << std::endl;
            std::cout << name_associator.generate_random_full_name(region) << std::endl;
        }
    }
    else
    {
        ErrorHandler::assert(std::find(region_list.cbegin(), region_list.cend(), chosen_region) != region_list.cend(), "Error: region name '", chosen_region, "' doesn't exist.");
        std::cout << std::endl << "Format: " << name_associator.get_formated_full_name_format(chosen_region) << std::endl << std::endl;
        for (int i = 0 ; i < generation_count ; ++i)
            std::cout << name_associator.generate_random_full_name(chosen_region) << std::endl;
    }

    std::cout << std::endl;

    std::system("pause");

    return 0;
}


void usage()
{
    //           |----++++----++++----++++----++++----++++----++++----++++----++++----++++----++++|
    std::cout << "This program generates a random full name depending on specific details such as " <<
    std::endl << "region, consonance, and format and follows a fully-customizable syllables dict  " <<
    std::endl << "(c.f. README.md for more details on how to creates te csutomization files)      " <<
    std::endl <<
    std::endl << "Available options are:                                                          " <<
    std::endl << "    -h --help             This help. Does not induce name generation.           " <<
    std::endl << "    -p --print-regions    Prints the regions contained in the Name Associations " <<
    std::endl << "                          file. Requires the --name-file option. Does not induce" <<
    std::endl << "                          name generation.                                      " <<
    std::endl << "    -c --count            The number of random name you want to be generated at " <<
    std::endl << "                          a time. Optionnal. Default value: 10.                 " <<
    std::endl << "    -r --region           The name of the region from which you want the name to" <<
    std::endl << "                          be generated (leave empty for a random region).       " <<
    std::endl << "                          Optionnal. Default value: <empty>                     " <<
    std::endl << "    -S --syllable-file    The path to the Syllable Associations file (CSV).     " <<
    std::endl << "                          Optionnal. Default value: SyllableAssociations.csv    " <<
    std::endl << "    -N --name-file        The path to the Name Associations file (CSV).         " <<
    std::endl << "                          Optionnal. Default value: NameAssociations.csv        " <<
    std::endl << "    -P --symbol-file      The path to the Symbol Pairing file (CSV). Optionnal. " <<
    std::endl << "                          Default value: SymbolPairing.csv                      " <<
    std::endl <<
    std::endl;
    //           |----++++----++++----++++----++++----++++----++++----++++----++++----++++----++++|
}
