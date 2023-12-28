================================================================================
    Usage

This program generates a random full name depending on specific details such as
region, consonance, and format and follows a fully-customizable syllables dict
(c.f. README.md for more details on how to creates te csutomization files)

Available options are:
-h --help             This help. Does not induce name generation.
-p --print-regions    Prints the regions contained in the Name Associations
                      file. Requires the --name-file option. Does not induce
                      name generation.
-c --count            The number of random name you want to be generated at
                      a time. Optionnal. Default value: 10.
-r --region           The name of the region from which you want the name to
                      be generated (leave empty for a random region).
                      Optionnal. Default value: <empty>
-S --syllable-file    The path to the Syllable Associations file (CSV).
                      Optionnal. Default value: SyllableAssociations.csv
-N --name-file        The path to the Name Associations file (CSV).
                      Optionnal. Default value: NameAssociations.csv
-P --symbol-file      The path to the Symbol Pairing file (CSV). Optionnal.
                      Default value: SymbolPairing.csv
================================================================================
    Rules for CSV files

There are three types of CSV files:
- SyllableAssociations file, associated to the --syllable-file option
- NameAssociator file, associated to the --name-file option
- SymbolPairing file, associated to the --symbol-file option

Each of these files are CSV which can be formated using both the ',' and the ';'
charactere. Watch out, you can't use one as separator and the other as plain
character.
================================================================================
    SyllableAssociations file

This file allows to define consonances. Each consonance has a sets of syllables
that will be the base of the randomization to generate names.
It independently handles prefixes, middle syllables and suffixes.
Particules are standalone syllables that will be used as particles.

For each line, you must have :
- The name of the consonance
- The type of syllable (RANGE is special)
- Any number of syllable sample

Types of syllable are as follow:
- PREFIX
- MIDDLE
- SUFFIX
- PARTICLE
- RANGE

RANGE is a special line where you can define the number of syllables for a
specific consonance. The second column is the minimum number of syllables and
the third is the maximum.
================================================================================
    NameAssociator file

This files defines which consonance of name can exist within a region, and the
format of names generated. You can separatly define consonances for first names,
last names and particules.

First line must hold three numbers.
The first number is the number of first name consonances. The second is the
number of last name consonances. The third id the number of particle consonances

Then, for each line:
- The name of the region
- The format of the full name (see the Format section for the details)
- The next N columns are a list the first name consonances (N is defined in the
first column of the first line).
- The next N' columns are a list the last name consonances (N' is defined in the
second column of the first line).
- The next N'' columns are a list the particles consonances (N'' is defined in
the third column of the first line).
================================================================================
    SymbolPairing file

Allows you to define symbols to use in the format section of the NameAssociator
file. Each symbol will be associated with a name type and a multiplicity.

A symbol is a single character.

Names types are:
- FIRST_NAME
- LAST_NAME
- PARTICLE

Multiplicity is a number larger or equal to 1 and represent the number of times
the syllable must be repeated (most of the time you'll want 1).
For instance, "Titi" is the syllable 'Ti' with a multiplicity of 2, and "Fafafa"
is the syllable 'Fa' with a multiplicity of 3.
================================================================================
    Format

Formating is made simple. It is a string. For each character, is it matches a
symbol from the SymbolPairing file, then it is replaced by the associated name
type, with the associated multiplicity.

Every other symbol is kept as it is.

For instance, take the following table:
F -> FIRST_NAME, 1
L -> LAST_NAME, 1
And the following format:
"F-as-L"
The name generated will take the following form:
<first name>-as-<last_name>
================================================================================
    Notes

Each name generated has its casing modified : first letter is always uppercase,
others anre always lowercase.

If any bug is found, constact me at chloe.lourseyre@gmail.com
================================================================================
    JavaScript
    
The folder "JS" contains a JavaScript portage of the program
================================================================================
end
