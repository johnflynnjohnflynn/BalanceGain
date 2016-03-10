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
    - Vertical filmstrip (usually containing rotating marker/texture).
    - Square image frame to calculate number of frames automatically by image
      size.
    - Using a top layer to be composited over filmstrip (usually containing
      shadows, bevels and layer styles etc.)

*/
class FilmStrip  : public Slider
{
public:
    FilmStrip (Image filmStripImage, Image topLayerImage)
        : Slider {},
          filmStrip_ {filmStripImage},
          topLayer_  {topLayerImage},
          filmStripHeight_ {filmStrip_.getHeight()},
          width_           {filmStrip_.getWidth()},
          numFrames_       {filmStripHeight_ / width_}
    {
        jassert (filmStripHeight_ > width_);
        jassert (numFrames_ > 0);

        //setTextBoxStyle (NoTextBox, 0, 0, 0);
        setTextBoxStyle (TextBoxBelow, false, 80, 15);
        setColour (Slider::textBoxTextColourId, Colour (0xff404040));
        setColour (Slider::textBoxBackgroundColourId, Colour (0x10000000));
        setColour (Slider::textBoxOutlineColourId, Colour (0x00000000));
        setSliderStyle (RotaryHorizontalVerticalDrag);

        Logger::outputDebugString ((String) filmStripHeight_);
        Logger::outputDebugString ((String) width_);
        Logger::outputDebugString ((String) numFrames_);

    }
    
    void paint(Graphics& g)
    {
        const float sliderPosition = static_cast<float> (valueToProportionOfLength (getValue()));

        int imagePositionRatio = sliderPosition * (numFrames_ - 1);

        g.drawImage (filmStrip_,
                     0,
                     0,
                     getSize() / 2,
                     getSize() / 2,
                     0,
                     imagePositionRatio * width_,
                     getSize(),
                     getSize());

        g.drawImage (topLayer_, 0, 0, getSize() / 2, getSize() / 2, 0, 0, getSize(), getSize());

        if (isMouseOverOrDragging())
        {
        }
    }

    int getSize() { return width_; } // return vertical/horizontal dimension of our
                                     // square component
    
private:
    const Image filmStrip_;
    const Image topLayer_;
    const int filmStripHeight_;
    const int width_;
    const int numFrames_;
};


#endif  // FILMSTRIP_H_INCLUDED
