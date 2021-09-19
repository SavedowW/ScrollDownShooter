#include "DataSaver.h"

DataSaver::DataSaver(const std::string& fileName_) :
	m_fileName(fileName_)
{
}

void DataSaver::save()
{
	std::ofstream cfg(m_fileName, std::ios_base::trunc);

	for (auto& section : m_variables)
	{
		cfg << "[" << section.first << "]\n";
		for (auto& var : section.second)
		{
			cfg << var.first << " = " << var.second << std::endl;
		}
	}

	cfg.close();
}

bool DataSaver::load()
{
	std::ifstream cfg("settings.cfg");

	if (!cfg)
		return false;

	m_variables.clear();

	char s[256];
	std::string currentSection;
	while (cfg.getline(s, 256))
	{
		std::string_view str = s;
		if (str[0] == '[')
		{
			currentSection = str.substr(1, str.size() - 2);
		}
		else
		{
			std::string var, value;
			int eqPos = str.find('=');
			var = str.substr(0, eqPos - 1);
			value = str.substr(eqPos + 2);
			m_variables[currentSection][var] = value;
		}
	}

	cfg.close();
	return true;
}
