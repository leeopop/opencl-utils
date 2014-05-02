/*
 * cl_util.hpp
 *
 *  Created on: 2014. 5. 3.
 *      Author: 근홍
 */

#ifndef CL_UTIL_HPP_
#define CL_UTIL_HPP_

#include "cl_header.hpp"

namespace clutil
{

class CLUtil
{
public:
	static cl::Device promptDevice(std::istream &in = std::cin, std::ostream &out = std::cout);
	static cl::Program compileProgram(std::string path, cl::Context ctx, std::vector<cl::Device> devices, std::ostream &error = std::cerr);

};

}

#endif /* CL_UTIL_HPP_ */
