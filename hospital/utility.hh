/* Module: utility
*
 * Module for different utilities needed in the program.
 *
 * */
#ifndef UTILITY_HH
#define UTILITY_HH

#include "date.hh"
#include <string>
#include <vector>

namespace utility {
/**
 * @brief split
 * @param str
 * @param delim
 * @return vector containing the parts, no delim chars
 * Splits the given string at every delim char
 */
std::vector<std::string> split(std::string& str, char delim = ';');

/**
 * @brief is_numeric
 * @param s
 * @param zero_allowed : if zero is allowed to be the whole string
 * @return true if given string is numeric
 */
bool is_num(std::string s, bool zero_allowed);

/**
 * @brief today
 * Static means this variable will only be created once per run.
 */
static Date today = Date(24, 2, 2021);

}

#endif // UTILITY_HH