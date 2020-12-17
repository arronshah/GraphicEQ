//
//  AudioFilePlayerComponent.hpp
//  AssignmentProject - App
//
//  Created by Arron Shah on 17/12/2020.
//

#pragma once

#include <JuceHeader.h>
#include "UIElementProperties.h"

class AudioFilePlayerComponent : public Component
{
public:
    AudioFilePlayerComponent();
    ~AudioFilePlayerComponent() = default;
    void resized() override;
    
private:
    TextButton play;
    TextButton loadFile;
    
};
