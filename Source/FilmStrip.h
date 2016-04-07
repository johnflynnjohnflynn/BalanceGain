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
class FilmStrip  : public Slider                                        // rename to knob?
{
public:
    FilmStrip (Image filmStripImage, Image topLayerImage)
        : Slider {},
          filmStrip {filmStripImage},
          topLayer  {topLayerImage},
          filmStripHeight {filmStrip.getHeight()},
          width           {filmStrip.getWidth()},
          numFrames       {filmStripHeight / width}
    {
        jassert (filmStripHeight > width);
        jassert (numFrames > 0);

        //setTextBoxStyle (NoTextBox, 0, 0, 0);
        setTextBoxStyle (TextBoxBelow, false, 80, 15);
        //setColour (Slider::backgroundColourId,        Colour (0x00000000));

        setColour (Slider::textBoxTextColourId,       Colour (0xff606060));
        setColour (Slider::textBoxBackgroundColourId, Colour (0x80c0c0c0));
        setColour (Slider::textBoxHighlightColourId,  Colour (0xffffffff));
        setColour (Slider::textBoxOutlineColourId,    Colour (0x00000000));
        setSliderStyle (RotaryHorizontalVerticalDrag);

        // outlineWhenEditingColourId but how to set?

        // LookAndFeel_V3 laf;
    }
    
    void paint(Graphics& g)
    {
        const float sliderPosition = static_cast<float> (valueToProportionOfLength (getValue()));

        int imagePositionRatio = sliderPosition * (numFrames - 1);                      // int?!

        g.drawImage (filmStrip,                    // Draw filmstrip then draw
                     0,                             // 'topLayer' image (should have
                     0,                             // transparancies) above.
                     getSize() / 2,
                     getSize() / 2,
                     0,
                     imagePositionRatio * width,
                     getSize(),
                     getSize());

        g.drawImage (topLayer, 0, 0, getSize() / 2, getSize() / 2, 0, 0, getSize(), getSize());

        if (isMouseOverOrDragging())
        {
        }
    }

    int getSize() { return width; } // return vertical/horizontal dimension of our
                                     // square component.
    
private:
    const Image filmStrip;
    const Image topLayer;
    const int filmStripHeight;
    const int width;
    const int numFrames;
};


#endif  // FILMSTRIP_H_INCLUDED
