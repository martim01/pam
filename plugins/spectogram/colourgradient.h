#pragma once
#include <vector>>

class ColourGradient
{
private:
    struct ColourPoint  // Internal class used to store Colours at different points in the gradient.
    {
        float r,g,b;      // Red, green and blue values of our Colour.
        float val;        // Position of our Colour along the gradient (between 0 and 1).
        ColourPoint(float red, float green, float blue, float value)
            : r(red), g(green), b(blue), val(value) {}
    };
    std::vector<ColourPoint> m_vColour;      // An array of Colour points in ascending value.

public:
    //-- Default constructor:
    ColourGradient()
    {
        createDefaultHeatMapGradient();
    }

    //-- Inserts a new Colour point into its correct position:
    void addColourPoint(float red, float green, float blue, float value)
    {
        for(int i=0; i<m_vColour.size(); i++)
        {
            if(value < m_vColour[i].val)
            {
                m_vColour.insert(m_vColour.begin()+i, ColourPoint(red,green,blue, value));
                return;
            }
        }
        m_vColour.push_back(ColourPoint(red,green,blue, value));
    }

    //-- Inserts a new Colour point into its correct position:
    void clearGradient()
    {
        m_vColour.clear();
    }

    //-- Places a 5 Colour heapmap gradient into the "Colour" vector:
    void createDefaultHeatMapGradient()
    {
        m_vColour.clear();
        m_vColour.push_back(ColourPoint(0, 0, 0,   0.0f));      // Blue.
        m_vColour.push_back(ColourPoint(0, 0, 1,   0.2f));     // Cyan.
        m_vColour.push_back(ColourPoint(0, 1, 1,   0.4f));      // Green.
        m_vColour.push_back(ColourPoint(0, 1, 0,   0.6f));     // Yellow.
        m_vColour.push_back(ColourPoint(1, 1, 0,   0.775f));      // Red.
        m_vColour.push_back(ColourPoint(1, 0, 0,   0.875f));      // Red.
        m_vColour.push_back(ColourPoint(1, 1, 1,   1.0f));      // Red.
    }

    //-- Inputs a (value) between 0 and 1 and outputs the (red), (green) and (blue)
    //-- values representing that position in the gradient.
    void getColourAtValue(const float value, float &red, float &green, float &blue)
    {
        if(m_vColour.size()==0)
            return;

        for(int i=0; i<m_vColour.size(); i++)
        {
            ColourPoint &currC = m_vColour[i];
            if(value < currC.val)
            {
                ColourPoint &prevC  = m_vColour[std::max(0,i-1) ];
                float valueDiff    = (prevC.val - currC.val);
                float fractBetween = (valueDiff==0) ? 0 : (value - currC.val) / valueDiff;
                red   = (prevC.r - currC.r)*fractBetween + currC.r;
                green = (prevC.g - currC.g)*fractBetween + currC.g;
                blue  = (prevC.b - currC.b)*fractBetween + currC.b;
                return;
            }
        }
        red   = m_vColour.back().r;
        green = m_vColour.back().g;
        blue  = m_vColour.back().b;
        return;
    }
};
