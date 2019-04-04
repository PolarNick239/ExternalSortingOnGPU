#pragma once

#include <string>
#include <vector>

#ifdef CUDA_SUPPORT

class CUDAEnum {
public:
	CUDAEnum();
	~CUDAEnum();

	class Device {
	public:
		Device()
		{
			id					= 0;
			compute_units		= 0;
			mem_size			= 0;
			clock				= 0;
			pci_bus_id			= 0;
			pci_device_id		= 0;
		}

		int						id;
		std::string				name;
		unsigned int			compute_units;
		unsigned long long		mem_size;
		unsigned int			clock;
		unsigned int			pci_bus_id;
		unsigned int			pci_device_id;
	};

	bool	enumDevices();
	std::vector<Device> &	devices()	{ return devices_;		}

	static	bool printInfo(int id);

protected:
	static	bool	compareDevice(const Device &dev1, const Device &dev2);

	std::vector<Device>		devices_;
};

#endif
