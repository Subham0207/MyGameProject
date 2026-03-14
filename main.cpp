#include<Editor.hpp>
#include <string>
#include <iostream>

int main(int argc, char * argv[])
{
    try
    {
        auto editor = new Editor();
        editor->openEditor();
    }
    catch(std::exception &error)
     {
         std::cout << "ERROR OCCURED: " << error.what() << std::endl;
     }

    return 0;
}