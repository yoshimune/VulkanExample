#pragma once

#include "../common/VulkanBase.h"

class SimpleModel : public VulkanBase
{
public:
	SimpleModel() : VulkanBase() {}
	~SimpleModel() {}

protected:
	const char* appTitle() override { return "SimpleModel"; }


	void render() override;

private:

};
