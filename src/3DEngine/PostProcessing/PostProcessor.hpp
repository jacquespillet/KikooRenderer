#pragma once
#include "Util/Common.h"
#include "PostProcess.hpp"
#include "../Framebuffer.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class PostProcessor {
public:
    PostProcessor();

    void AddProcess(PostProcess* process);
    
    void RemoveProcess(PostProcess* process);

     void Run(Framebuffer*& framebufferIn, Framebuffer*& framebufferOut);

    void Destroy();

    int numProcesses=0;

private:
    std::vector<PostProcess*> processes;
    Framebuffer* internalFBO0;
    Framebuffer* internalFBO1;
};

}
}