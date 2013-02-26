//created by Wang Jianwei on 01 Dec 2012
#ifndef ABSTRACTDAO_H
#define ABSTRACTDAO_H
#include <string>
#include <map>
#include <iostream>
#include "AbstractSession.h"

using namespace Session;
using namespace utilities;
using namespace std;

namespace DAO{
	class AbstractDAO: public AbstractSession{

	public:

		virtual void init(Configuration* cfg){
			_cfg=cfg;
		}

		virtual void retrieveRecord(){};

      virtual void printSuccessMsg(){
         if(_enabled) cout<<"Loading data for ["+_name+"] is complete!"<<endl;
      }

	protected:

		Configuration* _cfg;
      std::string _name;
		bool _enabled;
	};
}
#endif