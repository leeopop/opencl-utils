/*
 * cl_init.cpp
 *
 *  Created on: 2014. 5. 3.
 *      Author: 근홍
 */

#include "cl_util.hpp"

#include <vector>
#include <fstream>
#include <iostream>

using std::vector;
using cl::Platform;
using cl::Device;

using std::vector;

namespace clutil
{

cl::Device CLUtil::promptDevice(std::istream &in, std::ostream &out)
{
	try {
		vector<Device> allDevices;
		vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		if (platforms.size() == 0) {
			out << "Platform size 0"<<std::endl;
			return 0;
		}

		for(auto platform : platforms)
		{
			std::string platform_name;
			platform.getInfo(CL_PLATFORM_NAME, &platform_name);
			out<<"Platform("<<platform()<<"): "<<platform_name<<std::endl;

			vector<Device> devices;
			platform.getDevices(CL_DEVICE_TYPE_ALL,&devices);
			for(auto device : devices)
			{
				std::string device_name = device.getInfo<CL_DEVICE_NAME>();
				std::string extensions = device.getInfo<CL_DEVICE_EXTENSIONS>();
				std::string version = device.getInfo<CL_DEVICE_VERSION>();
				cl_ulong local_memory = device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
				cl_ulong global_memory = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
				cl_ulong max_memory = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

				allDevices.push_back(device);
				out<<"\tDevice("<<allDevices.size()<<"): "<<device_name<<std::endl;
				out<<"\tVersion: "<<version<<std::endl;
				out<<"\t\tMemory: "<<local_memory<<"(local), "<<global_memory<<"(global), "<<max_memory<<"(max_local)"<<std::endl;
				out<<"\t\tExtensions: "<<extensions<<std::endl;
			}
		}

		out<<"Select device: ";
		size_t dev_idx=3;
		in>>dev_idx;

		if(dev_idx >= allDevices.size() || dev_idx < 1)
		{
			out<<"Wrong index number "<<dev_idx<<std::endl;
			return 0;
		}
		return allDevices[dev_idx-1];
	}
	catch (cl::Error &err) {
		std::cerr
		<< "ERROR: "
		<< err.what()
		<< "("
		<< err.err()
		<< ")"
		<< std::endl;
	}

	return 0;
}

cl::Program CLUtil::compileProgram(std::string path, cl::Context ctx, std::vector<cl::Device> devices, std::ostream &err_out)
{
	std::ifstream file(path, std::ios::binary);
	if(!file)
		throw cl::Error(CL_INVALID_VALUE, ("File not exist(" + path + ")").c_str());
	std::string source_code = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

	cl::Program::Sources source(1,
			std::make_pair(source_code.c_str(),source_code.size()));
	cl::Program program = cl::Program(ctx, source);
	try
	{
		program.build(devices);
	}
	catch(cl::Error &error)
	{
		if(error.err() == CL_BUILD_PROGRAM_FAILURE){
			for(cl::Device devIter : devices)
			{
				if(program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(devIter) == CL_BUILD_ERROR)
				{
					err_out << "Build failed for device " << devIter.getInfo<CL_DEVICE_NAME>() << std::endl;
					err_out << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devIter) << std::endl;
				}
			}
		}
		throw error;
	}
	return program;
}
}
