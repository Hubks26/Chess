#include "sfmlGame/Engine.h"

int main()
{
	try
	{
		Board b;
		b.print();
		
		Engine engine;
		engine.run();
	}
	catch (std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
		return 1;
	}
	
    return 0;
}
