#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent   : public AudioAppComponent,
                               public Slider::Listener
{
public:
    MainContentComponent()
        : phase(0.0f),
          amplitude(0.2f),
          frequency(220.0f),
          phaseIncrement(0.0f),
          currentSampleRate(44100.0f)
    {
        setSize (800, 600);

        setAudioChannels (0, 1);
        
        levelSlider.setRange(0.0f, 1.0f);
        levelSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
        levelLabel.setText ("Level", dontSendNotification); 
        addAndMakeVisible (levelSlider);
        addAndMakeVisible (levelLabel);
        
        freqSlider.setRange(20, 20000);
        freqSlider.setSkewFactorFromMidPoint (2000.0);
        freqSlider.addListener (this);
        freqSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, 20);
        freqLabel.setText ("Frequency", dontSendNotification); 
        addAndMakeVisible (freqSlider);
        addAndMakeVisible (freqLabel);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &freqSlider)
            updatePhase();
    }
    
    void updatePhase()
    {
        frequency = (float)freqSlider.getValue();
        phaseIncrement = frequency * 2.0f * float_Pi / currentSampleRate;
        
    }
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        currentSampleRate = (float)sampleRate;
        updatePhase();
    }
    
    void releaseResources() override
    {
        
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        int startSample = bufferToFill.startSample;
        int numSamples = bufferToFill.numSamples;
        AudioSampleBuffer *buffer = bufferToFill.buffer;
        
        amplitude = (float)levelSlider.getValue();
        
        float *channelData = buffer->getWritePointer(0,startSample);
        
        for(int i = 0; i < numSamples; ++i)
        {
            channelData[i] = amplitude * std::cos(phase);
            phase = std::fmod (phase + phaseIncrement, 2.0f * float_Pi);
        }
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::white);
    }

    void resized() override
    {
        levelLabel.setBounds (10, 10, 90, 20);
        levelSlider.setBounds (100, 10, getWidth() - 110, 20);
        freqLabel.setBounds (10, 40, 90, 20);
        freqSlider.setBounds (100, 40, getWidth() - 110, 20);
    }


private:

    float phase;
    float amplitude;
    float frequency;
    float phaseIncrement;
    float currentSampleRate;
    Slider levelSlider;
    Label levelLabel; 
    Slider freqSlider;
    Label freqLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
