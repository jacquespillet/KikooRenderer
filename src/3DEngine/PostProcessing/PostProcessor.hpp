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

    void Run(Framebuffer* framebufferIn, Framebuffer* framebufferOut);

    void Destroy();

private:
    std::vector<PostProcess*> processes;
};

}
}