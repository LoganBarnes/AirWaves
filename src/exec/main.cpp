#include <vmp/SimTest.hpp>
#include <kfr/math.hpp>

int main(const int argc, const char *argv[])
{
    try
    {
        sim::SimInitData initData;
        initData.title = "Mesh Sim";
        sim::OpenGLSimulation<Simulator>(initData).runNoFasterThanRealTimeLoop();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Program failed: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}