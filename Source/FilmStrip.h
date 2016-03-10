/*
  ==============================================================================

    FilmStrip.h
    Created: 10 Mar 2016 3:53:50pm
    Author:  John Flynn

  ==============================================================================
*/

#ifndef FILMSTRIP_H_INCLUDED
#define FILMSTRIP_H_INCLUDED


//==============================================================================
/**
    Slider that uses a PNG filmstrip for display
    --------------------------------------------

*/
class FilmStrip  : public Slider
{
public:
    FilmStrip (Image image, const int numFrames)
        : Slider {},
          numFrames_ {numFrames},
          filmStrip_ {image}
    {
        //setTextBoxStyle(NoTextBox, 0, 0, 0);
        setTextBoxStyle (TextBoxBelow, false, 80, 15);
        setColour (Slider::textBoxTextColourId, Colour (0xff404040));
        setColour (Slider::textBoxBackgroundColourId, Colour (0x10000000));
        setColour (Slider::textBoxOutlineColourId, Colour (0x00000000));
        setSliderStyle (RotaryHorizontalVerticalDrag);

        frameHeight_ = filmStrip_.getHeight() / numFrames_;
        frameWidth_ = filmStrip_.getWidth();
    }
    
    void paint(Graphics& g)
    {
        const float sliderPos = (float) valueToProportionOfLength (getValue());

        int value = sliderPos * (numFrames_ - 1);

        g.drawImage (filmStrip_, 0, 0, getWidth(), getHeight(),
                    0, value * frameHeight_, frameWidth_, frameHeight_);
        
        if (isMouseOverOrDragging())
        {
        }
    }
    
    int getFrameWidth() const  { return frameWidth_; }
    int getFrameHeight() const { return frameHeight_; }
    
private:
    const int numFrames_;
    Image filmStrip_;
    int frameWidth_;
    int frameHeight_;
};


#endif  // FILMSTRIP_H_INCLUDED
