#include "CControllers.h"

int main (){
    CPageController controller;
    try {
        controller.run();
    } catch (const CException & exception) {
        std::cout << exception;
    }
    return 0;
}