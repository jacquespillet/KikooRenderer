#include "PostProcessor.hpp"


namespace KikooRenderer {
namespace CoreEngine {

PostProcessor::PostProcessor() {
    //Get Width and height as parameters of the constructor. 
    //Have a update size function when resizing window.

    internalFBO0 = nullptr;
    internalFBO1 = nullptr;

    // finalFBO = new Framebuffer(scene->windowWidth, scene->windowHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT, true, false, false);
}

void PostProcessor::AddProcess(PostProcess* process) {
    processes.push_back(process);
    numProcesses++;
}

void PostProcessor::RemoveProcess(PostProcess* process) {
    for (int i = 0; i < processes.size(); i++) {
        if (processes[i] == process) {
            processes.erase(processes.begin() + i);
            numProcesses--;
            break;
        }
    }
}

void PostProcessor::Destroy() {
    for (int i = 0; i < processes.size(); i++) {
        delete processes[i];
    }
}

void PostProcessor::OnResize(int w, int h) {
    for (int i = 0; i < processes.size(); i++) {
        processes[i]->OnResize(w, h);
    }
}

void PostProcessor::Run(Framebuffer*& framebufferIn, Framebuffer*& framebufferOut) {
    //0 : Read in, writes 1
    //1 : Reads 1, writes 0
    //2 : Reads 0, writes 1
    //Returns 1

    //OR 

    //0 : Read 0, writes 1
    //1 : Reads 1, writes 0
    //2 : Reads 0, writes 1
    //3 : Reads 1, writes 0

    if(internalFBO0 == nullptr || internalFBO0->width != framebufferIn->width|| internalFBO0->height != framebufferIn->height) {        
        internalFBO0 = new Framebuffer(framebufferIn->width, framebufferIn->height, framebufferIn->internalColorFormat, framebufferIn->colorFormat, framebufferIn->colorType, true, false, false);
        internalFBO1 = new Framebuffer(framebufferIn->width, framebufferIn->height, framebufferIn->internalColorFormat, framebufferIn->colorFormat, framebufferIn->colorType, true, false, false);
    }

    for (int i = 0; i < processes.size(); i++) {
        if(i==0)   {
            processes[i]->Run(framebufferIn, internalFBO1);
        }
        else {
            if(i%2==1)  processes[i]->Run(internalFBO1, internalFBO0);
            if(i%2==0)  processes[i]->Run(internalFBO0, internalFBO1);
        }
    }
    
    if(processes.size() % 2 == 0) {
        framebufferOut = internalFBO0;
    }
    else{ 
        framebufferOut = internalFBO1;
    }
}
}
}