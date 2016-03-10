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
    
    Assumes:
    - Vertical filmstrip.
    - Square image frame to calculate number of frames automatically by image
      size.

*/
class FilmStrip  : public Slider
{
public:
    FilmStrip (Image image)
        : Slider {},
          filmStrip_ {image},
          height_    {filmStrip_.getHeight()},
          width_     {filmStrip_.getWidth()},
          numFrames_ {height_ / width_}
    {
        jassert (height_ > width_);
        jassert (numFrames_ > 0);

        //setTextBoxStyle (NoTextBox, 0, 0, 0);
        setTextBoxStyle (TextBoxBelow, false, 80, 15);
        setColour (Slider::textBoxTextColourId, Colour (0xff404040));
        setColour (Slider::textBoxBackgroundColourId, Colour (0x10000000));
        setColour (Slider::textBoxOutlineColourId, Colour (0x00000000));
        setSliderStyle (RotaryHorizontalVerticalDrag);
    }
    
    void paint(Graphics& g)
    {
        const float sliderPosition = static_cast<float> (valueToProportionOfLength (getValue()));

        int imagePositionRatio = sliderPosition * (numFrames_ - 1);

        g.drawImage (filmStrip_,
                     0,
                     0,
                     width_,
                     height_,
                     0,
                     imagePositionRatio * height_,
                     width_,
                     height_);
    }
    
private:
    const Image filmStrip_;
    const int height_;
    const int width_;
    const int numFrames_;
};


#endif  // FILMSTRIP_H_INCLUDED
