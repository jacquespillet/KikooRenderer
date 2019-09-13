#include "PostProcessor.hpp"


namespace KikooRenderer {
namespace CoreEngine {

PostProcessor::PostProcessor() {

}

void PostProcessor::AddProcess(PostProcess* process) {
    processes.push_back(process);
}

void PostProcessor::RemoveProcess(PostProcess* process) {
    for (int i = 0; i < processes.size(); i++) {
        if (processes[i] == process) {
            processes.erase(processes.begin() + i);
            break;
        }
    }
}

void PostProcessor::Destroy() {
    for (int i = 0; i < processes.size(); i++) {
        delete processes[i];
    }
}

void PostProcessor::Run(Framebuffer* framebufferIn, Framebuffer* framebufferOut) {
    for (int i = 0; i < processes.size(); i++) {
        processes[i]->Run(framebufferIn, framebufferOut);
    }
}
}
}