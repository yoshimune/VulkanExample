#pragma once

#include <string>

#include "../common/VulkanBase.h"

class SimpleModel : public VulkanBase
{
public:
	SimpleModel() : VulkanBase() {}
	~SimpleModel() {}

protected:
	const char* appTitle() override { return "SimpleModel"; }

	void prepare() override;
	void render() override;

	void loadModel(std::string filename);

private:

};
