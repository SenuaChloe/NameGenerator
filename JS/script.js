////
const DEBUG_MODE=true;
////

/*
  Syllable dict format
  
  consonance{} -> RANGE{} -> MIN -> int
               |            -> MAX -> int
               -> PREFIX -> string[]
               -> MIDDLE -> string[]
               -> SUFFIX -> string[]


  Name dict format

  region{} -> FIRST_NAME -> string[]
           -> LAST_NAME -> string[]
           -> PARTICLE -> string[]
           -> FORMAT -> string
         
         
  Symbol dict format
  
  character{} -> TYPE -> string
              -> VALUE -> int
*/


//==============================================================================
// Globals

const RANGE = "RANGE";
const FIRST_NAME = "FIRST_NAME";
const LAST_NAME = "LAST_NAME";
const PARTICLE = "PARTICLE";
const MIN = "MIN";
const MAX = "MAX";
const PREFIX = "PREFIX";
const MIDDLE = "MIDDLE";
const SUFFIX = "SUFFIX";
const VALUE = "VALUE";
const TYPE = "TYPE";
const FORMAT = "FORMAT";

//==============================================================================
// Events

document.addEventListener('keydown', e => {

  if (e.key === "Enter") {
    e.preventDefault();
    on_generate();
  }
});


//==============================================================================
// Utilities

function log_error(msg)
{
  document.getElementById("error_log").innerHTML += "Error: " + msg + "<br/>"
}

function rand(max) 
{
  return Math.floor(Math.random() * max);
}

function clear_HTML_list(ul)
{
  while(ul.firstChild) 
    ul.removeChild(ul.firstChild);
}

function append_to_HTML_list(ul, li_string)
{
    let li = document.createElement("li");
    li.appendChild(document.createTextNode(li_string));
    ul.appendChild(li);
}

function not_empty(a)
{ 
  return a.length != 0;
}
  
function text_to_table(text)
{
	let result = [];
	let text_array = text.split(/[\r\n]+/);
	for (let i_line in text_array)
	{
		let line_array = text_array[i_line].split(/[,;]/);
		result.push(line_array);
	}
  
  // Trimming trailing empty cells
  for (let i_line in result)
  {
    while (result[i_line].length > 0 && result[i_line].slice(-1)[0] == "")
      result[i_line].pop();
  }
  
  // Removing empty lines
  result = result.filter(not_empty);
  
	return result;
}

//==============================================================================
// CSV Parser

// CSV paths
const prefix_path = "data/";
const syllable_dict_filename = "SyllableAssociations.csv";
const name_dict_filename = "NameAssociations.csv";
const symbol_dict_filename = "SymbolPairing.csv";

// Global values
var syllable_dict_global = {};
var name_dict_global = {};
var symbol_dict_global = {};

async function parse_dicts()
{
  const [syllable_dict_global,
    name_dict_global,
    symbol_dict_global
  ] = await Promise.all([
    read_syllable_csv(),
    read_name_csv(),
    read_symbol_csv()
  ]);
}

async function read_syllable_csv()
{
  const fullpath = prefix_path + syllable_dict_filename;
  await fetch(fullpath)
    .then( r => r.text() )
    .then( t => parse_syllable_csv(t) );
}

async function read_name_csv()
{
  const fullpath = prefix_path + name_dict_filename;
  await fetch(fullpath)
    .then( r => r.text() )
    .then( t => parse_name_csv(t) );
}

async function read_symbol_csv()
{
  const fullpath = prefix_path + symbol_dict_filename;
  await fetch(fullpath)
    .then( r => r.text() )
    .then( t => parse_symbol_csv(t) );
}

function parse_syllable_csv(text)
{
  let text_table = text_to_table(text);
  for (let i_line in text_table)
  {
    if (text_table[i_line].length < 2)
    {
      log_error("["+syllable_dict_filename+"]:"+(i_line+1)+" - a single line must have at least 2 cells, got "+text_table[i_line].length);
      continue;
    }
    
    const consonance = text_table[i_line][0];
    const type = text_table[i_line][1];
    
    if (!(consonance in syllable_dict_global))
      syllable_dict_global[consonance] = {};
    
    if (type == RANGE)
    {
      if (!(type in syllable_dict_global[consonance]))
        syllable_dict_global[consonance][type] = {};
      if (text_table[i_line].length != 4)
      {
        log_error("["+syllable_dict_filename+"]:"+(i_line+1)+" - RANGE consonance type must have exactly 2 attributes, got "+(text_table[i_line].length-2));
        continue;
      }
      syllable_dict_global[consonance][type][MIN] = parseInt(text_table[i_line][2]);
      syllable_dict_global[consonance][type][MAX] = parseInt(text_table[i_line][3]);
    }
    else
    {
      if (!(type in syllable_dict_global[consonance]))
        syllable_dict_global[consonance][type] = [];
      for (let i_cell = 2 ; i_cell < text_table[i_line].length ; i_cell++)
        syllable_dict_global[consonance][type].push(text_table[i_line][i_cell]);
    }
  }
  console.log(syllable_dict_global);
}

function parse_name_csv(text)
{
  let text_table = text_to_table(text);
  if (text_table[0].length != 3)
  {
    log_error("["+name_dict_filename+"]:"+1+" - first line must contain exactly 3 cells, got "+text_table[i_line].length);
    return;
  }
  
  const firstn_buffer = parseInt(text_table[0][0]);
  const lastn_buffer = parseInt(text_table[0][1]);
  const particle_buffer = parseInt(text_table[0][2]);
    
  for (let i_line = 1 ; i_line < text_table.length ; i_line++)
  {
    
    if (text_table[i_line].length < 2)
    {
      log_error("["+name_dict_filename+"]:"+(i_line+1)+" - lines must have at least 2 cell2, got "+text_table[i_line].length);
      continue;
    }
    
    const region = text_table[i_line][0];
    
    let i_cell = 2;
    
    name_dict_global[region] = {};
    
    name_dict_global[region][FORMAT] = text_table[i_line][1];
      
    name_dict_global[region][FIRST_NAME] = [];
    for (; i_cell < (2+firstn_buffer) && i_cell < text_table[i_line].length ; i_cell++)
    {
      const value = text_table[i_line][i_cell];
      if (value != "")
        name_dict_global[region][FIRST_NAME].push(value);
    }
      
    name_dict_global[region][LAST_NAME] = [];
    for (; i_cell < (2+firstn_buffer+lastn_buffer) && i_cell < text_table[i_line].length ; i_cell++)
    {
      const value = text_table[i_line][i_cell];
      if (value != "")
        name_dict_global[region][LAST_NAME].push(value);      
    }
    
    name_dict_global[region][PARTICLE] = [];
    for (; i_cell < (2+firstn_buffer+lastn_buffer+particle_buffer) && i_cell < text_table[i_line].length ; i_cell++)
    {
      const value = text_table[i_line][i_cell];
      if (value != "")
        name_dict_global[region][PARTICLE].push(value);      
    } 
  }
  console.log(name_dict_global);
}

function parse_symbol_csv(text)
{
  let text_table = text_to_table(text);
  for (let i_line in text_table)
  {
    if (text_table[i_line].length != 3)
    {
      log_error("["+symbol_dict_filename+"]:"+(i_line+1)+" - a single line must have exactly 3 cells, got "+text_table[i_line].length);
      continue;
    }
    
    const symbol = text_table[i_line][0];
    const type = text_table[i_line][1];
    const value = parseInt(text_table[i_line][2]);
    
    symbol_dict_global[symbol] = {};
    symbol_dict_global[symbol][TYPE] = type;
    symbol_dict_global[symbol][VALUE] = value;    
  }
  console.log(symbol_dict_global);
}


//==============================================================================
// Main Generator

function generator_main(count, region) 
{
  let result_list = [];
  for (let i_count = 0 ; i_count < count ; i_count++)
  {
    result_list.push(generate_name(region));
  }
  
  return result_list;
}

function generate_name(region)
{
  let result="";

  const symbol_format = name_dict_global[region][FORMAT];
  for (let i_char in symbol_format)
  {
    const val_char = symbol_format.charAt(i_char);
    if (val_char in symbol_dict_global)
    {
      const consonance_type = symbol_dict_global[val_char][TYPE];
      const consonance_rand = rand(name_dict_global[region][consonance_type].length);
      const consonance = name_dict_global[region][consonance_type][consonance_rand];
      
      let single_name = "";
      if (consonance_type==PARTICLE)
        single_name = generate_single_particle(consonance, syllable_dict_global);
      else
        single_name = generate_single_name(consonance, syllable_dict_global, name_dict_global);
      
      let intermediate_result = "";
      const symbol_value = symbol_dict_global[val_char][VALUE];
      for (let i_symbol = 0 ; i_symbol < symbol_value ; i_symbol++)
        intermediate_result += single_name;
      
      intermediate_result = intermediate_result.charAt(0).toUpperCase() + intermediate_result.slice(1).toLowerCase();
      result += intermediate_result;
    }
    else
    {
      result += val_char;
    }
  }
  return result;
}


function generate_single_name(consonance)
{
  let result = "";
  
  const syllabl_count_min = syllable_dict_global[consonance][RANGE][MIN];
  const syllable_count_max = syllable_dict_global[consonance][RANGE][MAX];
  const syllable_count_rand = rand(syllable_count_max+1-syllabl_count_min)+syllabl_count_min;

  // Prefix
  const prefix_rand = rand(syllable_dict_global[consonance][PREFIX].length);
  result += syllable_dict_global[consonance][PREFIX][prefix_rand];
  
  // Middle
  for (let i_middle = 1 ; i_middle < syllable_count_rand-1 ; i_middle++)
  {
    const middle_rand = rand(syllable_dict_global[consonance][MIDDLE].length);
    result += syllable_dict_global[consonance][MIDDLE][middle_rand];
  }
  
  // Suffix
  if (syllable_count_rand >= 2)
  {
    const suffix_rand = rand(syllable_dict_global[consonance][SUFFIX].length);
    result += syllable_dict_global[consonance][SUFFIX][suffix_rand];
  }
  
  return result;
}

function generate_single_particle(consonance)
{
  const prefix_rand = rand(syllable_dict_global[consonance][PARTICLE].length);
  return syllable_dict_global[consonance][PARTICLE][prefix_rand];
}


//==============================================================================
// HMI-related functions

function hide_hmi()
{
  document.getElementById("gen_form").style.display = "none";
  document.getElementById("loading_message").style.display = "block";
}

function show_hmi()
{
  document.getElementById("loading_message").style.display = "none";
  document.getElementById("gen_form").style.display = "block";
  
}

function load_hmi_values()
{
  let select = document.getElementById("region_list");
  
  let region_list = [];  
  for (let region in name_dict_global)
    region_list.push(region);
  
  region_list.sort();
  
  for (let i_region in region_list)
  {
    const region = region_list[i_region];
    const newOption = new Option(region,region);
    select.add(newOption,undefined);
  }
  
  update_hmi_format();
}

const printable_format_dict_global = {
  FIRST_NAME:"[prénom]",
  LAST_NAME:"[nom]",
  PARTICLE:"[particule]",
}

function update_hmi_format()
{
  const select = document.getElementById("region_list");
  const region = select.options[select.selectedIndex].value;
    
  const symbol_format = name_dict_global[region][FORMAT];
  let result = "";
  for (let i_char in symbol_format)
  {
    const val_char = symbol_format.charAt(i_char);
    if (val_char in symbol_dict_global)
      result += printable_format_dict_global[symbol_dict_global[val_char][TYPE]];
    else
      result += val_char;

    document.getElementById("name_format").innerHTML = result;
  }
}
function check_count_value()
{
  let count_element = document.getElementById("generation_count");
  const count_value = count_element.value;
  if (count_value < 1)
    count_element.value = 1;
}

function on_generate()
{
  const region_select = document.getElementById("region_list");
  const region = region_select.options[region_select.selectedIndex].value;
  const count = document.getElementById("generation_count").value;
  
  const generated_list = generator_main(count, region);
  
  let result_list = document.getElementById("results1");
  clear_HTML_list(result_list);
    
  let i_result = 0;
  for (; i_result < generated_list.length/2 ; i_result++)
    append_to_HTML_list(result_list, generated_list[i_result]);
  
  result_list = document.getElementById("results2");
  clear_HTML_list(result_list);
    
  for (; i_result < generated_list.length ; i_result++)
    append_to_HTML_list(result_list, generated_list[i_result]);
}


//==============================================================================
// Functions called at runtime

async function on_load()
{
  hide_hmi();
  await parse_dicts();
  load_hmi_values();
  show_hmi();
}