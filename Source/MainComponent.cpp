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
        
        sawWave.setColour(TextButton::buttonColourId, Colours::cornflowerblue);
        sawWave.setButtonText("Sawtooth Wave");
        sawWave.addListener(this);
        addAndMakeVisible(sawWave);
        
        triWave.setColour(TextButton::buttonColourId, Colours::cornflowerblue);
        triWave.setButtonText("Triangle Wave");
        triWave.addListener(this);
        addAndMakeVisible(triWave);
        
        noise.setColour(TextButton::buttonColourId, Colours::cornflowerblue);
        noise.setButtonText("Noise!");
        noise.addListener(this);
        addAndMakeVisible(noise);
        
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
        sawWave.removeListener(this);
        triWave.removeListener(this);
        noise.removeListener(this);
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
        else if (button == &sawWave)
        {
            waveStatus = 2;
        }
        else if (button == &triWave)
        {
            waveStatus = 3;
        }
        else if (button == &noise)
        {
            waveStatus = 4;
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
            switch (waveStatus)
            {
                case 0:
                    if (std::sin(phase)>0)
                    {
                        channelData[i] = 1.0f * amplitude;
                    }
                    else if (std::sin(phase)<0)
                    {
                        channelData[i] = -1.0f * amplitude;
                    }
                    break;
                case 1:
                    channelData[i] = amplitude * std::sin(phase);
                    break;
                case 2:
                    channelData[i] = amplitude * (1 -(1.0f / float_Pi * phase));
                    break;
                case 3:
                    if (phase < float_Pi)
                        channelData[i] = amplitude * (-1 + (2 / float_Pi) * phase);
                    else
                        channelData[i] = amplitude * (3 - (2 / float_Pi) * phase);
                    break;
                case 4:
                    channelData[i] = r.nextFloat();
                    break;
            }
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
        squareWave.setBounds(100, 60, 100, 50);
        sineWave.setBounds(220, 60, 100, 50);
        sawWave.setBounds(340, 60, 100, 50);
        triWave.setBounds(460, 60, 100, 50);
        noise.setBounds(580, 60, 100, 50);
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
    TextButton sawWave;
    TextButton triWave;
    TextButton noise;
    Random r;
    int waveStatus;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
