#pragma once

#include <iostream>
#include <istream>

void run_script(std::istream& script,
                std::ostream& output = std::cout,
                std::istream& input  = std::cin);
