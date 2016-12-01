#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent   : public AudioAppComponent,
                               public Slider::Listener,
                               public Button::Listener

{
public:
    MainContentComponent()
        : phase(0.0f),
          amplitude(0.2f),
          frequency(220.0f),
          phaseIncrement(0.0f),
          currentSampleRate(44100.0f),
          waveStatus(1)
    {
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
        
        squareWave.setColour(TextButton::buttonColourId, Colours::cornflowerblue);
        squareWave.setButtonText("Square Wave");
        squareWave.addListener(this);
        addAndMakeVisible(squareWave);
        
        sineWave.setColour(TextButton::buttonColourId, Colours::cornflowerblue);
        sineWave.setButtonText("Sine Wave");
        sineWave.addListener(this);
        addAndMakeVisible(sineWave);
        
        setSize (800, 600);
        
        setAudioChannels (0, 1);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
        levelSlider.removeListener(this);
        freqSlider.removeListener(this);
        squareWave.removeListener(this);
        sineWave.removeListener(this);
    }

    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &freqSlider)
            updatePhase();
    }

    void buttonClicked (Button* button) override
    {
        if (button == &squareWave)
        {
            waveStatus = 0;
        }
        else if (button == &sineWave)
        {
            waveStatus = 1;
        }
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
            if (waveStatus == 1)
            {
                channelData[i] = amplitude * std::sin(phase);
                phase = std::fmod (phase + phaseIncrement, 2.0f * float_Pi);
            }
            else if (waveStatus == 0)
            {
                if (std::sin(phase)>0)
                {
                    channelData[i] = 1.0f * amplitude;
                }
                else if (std::sin(phase)<0)
                {
                    channelData[i] = -1.0f * amplitude;
                }
                phase = std::fmod (phase + phaseIncrement, 2.0f * float_Pi);
            }
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
        squareWave.setBounds(200, 60, 100, 50);
        sineWave.setBounds(400, 60, 100, 50);
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
    TextButton squareWave;
    TextButton sineWave;
    int waveStatus;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
