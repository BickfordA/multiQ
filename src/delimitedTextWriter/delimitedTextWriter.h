#ifndef DELIMITEDTEXTWRITER_H
#define DELIMITEDTEXTWIRTER_H

#include <list>
#include <string>
#include <vector>

class DelimitedTextWriter
{

public:
	DelimitedTextWriter();

	void addData(std::vector<double> columnValues, std::string colmnName);
	void addData(std::vector<std::string> columnValues, std::string columnName);

	bool writeFileColumns(std::string fileName, std::string delimiter = ",");
	bool writeFileRows(std::string fileName, std::string delimiter = ",");

	std::string errMsg(){ return _errMsg; }


private:
	std::string _errMsg;

	std::vector<std::pair<std::string, std::vector<std::string> > > _dataValues;
};




#endif // DELIMITEDTEXTWIRTER_H