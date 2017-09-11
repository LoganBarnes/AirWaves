#include <sim-driver/OpenGLSimulation.hpp>
#include <sim-driver/renderers/MeshRenderer.hpp>

class Simulator
{
public:
    explicit Simulator(int, int, sim::SimData *pSimData);

    void onUpdate(double simTime, double timeStep);
    void onRender(int width, int height, double alpha);
    void onGuiRender(int width, int height);

    void keyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mods,
                     const sim::SimCallbacks <Simulator> &);

private:
    sim::MeshRenderer renderer_;
    sim::SimData &simData_;
    sim::Camera prevCam_;
};
