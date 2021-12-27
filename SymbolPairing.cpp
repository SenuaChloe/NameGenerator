#include "SymbolPairing.h"

#include <utility>
#include "CSVReader.h"
#include "ErrorHandler.h"


/******************************************************************************/
static NameType string_to_name_type(std::string name_type_s)
{
    if (name_type_s == "FIRST_NAME")
        return NameType::FIRST_NAME;
    else if (name_type_s == "LAST_NAME")
        return NameType::LAST_NAME;
    else if (name_type_s == "PARTICLE")
        return NameType::PARTICLE;
    ErrorHandler::raise_error("Error: ", name_type_s, " is not a name type.");
    throw; // To avoid a warning
}

/******************************************************************************/
SymbolPairing::SymbolPairing(const std::filesystem::path & filepath)
{
    CSVReader csv_reader(filepath);

    // Filling the name generator with the data from the CSV
    for (auto line : csv_reader)
    {
        ErrorHandler::assert(line.size() == 3, "File format is wrong");

        auto it = line.cbegin();

        const Symbol symbol = (*it++)[0];
        const NameType name_type = string_to_name_type(*it++);
        const unsigned int multiplicity = std::stoi(*it++);

        (*this)[symbol] = {name_type, multiplicity};
    }
}
