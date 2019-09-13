#pragma once
#include "Util/Common.h"
#include "../Framebuffer.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class PostProcess {
public:
    PostProcess();
    virtual void Run(Framebuffer* famebufferIn, Framebuffer* famebufferOut);
};
}
}