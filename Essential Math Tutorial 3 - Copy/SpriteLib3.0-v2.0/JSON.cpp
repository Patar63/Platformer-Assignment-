#include "JSON.h"

nlohmann::json File::LoadJSON(std::string linkAnimations)
{
	//Opens input file stream 
	std::ifstream i(File::defaultDir + linkAnimations);
	//Creates a json object
	nlohmann::json j;

	//if file fails to open
		//return empty json
	if (!i.good())
	{
		return j;
	}

	//Inputs the file contents into the json object
	i >> j;

	//Closes the file stream
	i.close();

	//Returns the filled json object
	return j;
}

void File::CreateJSON(std::string linkAnimations, nlohmann::json j)
{
	std::ofstream o(File::defaultDir + linkAnimations);

	//outputs the json object to the json file
	o << j;

	//Closes the file stream
	o.close();
}
