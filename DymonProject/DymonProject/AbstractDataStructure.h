//created by Wang Jianwei on 01 Dec 2012
#ifndef ABSTRACTDATASTRUCTURE_H
#define ABSTRACTDATASTRUCTURE_H

#include <string>

namespace utilities{
	class AbstractDataStructure{
		
	public:
		
		AbstractDataStructure(){};
      
      // Getters and setters
      string getName(){ return _name; }
      void setName(string name){ _name = name; }

   protected:

      string _name;
	};
}
#endif