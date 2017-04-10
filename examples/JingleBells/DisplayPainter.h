/*
  Simple song player library for Meggy Jr RGB.
  Example: JingleBells
  
  Version 1.0 - 2011-01-01
  Copyright (c) 2011 Martin Gamwell Dawids.

    This file is part of the SongPlayer library for Meggy Jr RGB.

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library.  If not, see <http://www.gnu.org/licenses/>.

  For an overview of the functionality in this library,
  please see the file "SongPlayer.h".
*/

class DisplayPainter : public CoRoutine
{
private:
  byte snowLayer[8][8];
  byte starLayer[8][8];
  
  // Repaint info
  enum RepaintKind { NoRepaint, RepaintAll, RepaintPixel };
  RepaintKind repaintState;
  int drawX;
  int drawY;
  
  // Helper for drawing one pixel.
  void drawPixel(int x, int y)
  {
    // Lookup pixel in snow layer.
    byte pixel = snowLayer[y][x];
  
    // If it was dark (i.e. no snow) look for a star.
    if (pixel == Dark)
    {
      pixel = starLayer[y][x];
    }
    
    // Draw only pixel if not dark.
    if (pixel != Dark)
    {
      DrawPx(x,y,pixel);
    }
  }
  
public:
  DisplayPainter()
    : repaintState(NoRepaint), drawX(0), drawY(0)
  {
    // Clear display layers.
    memset(&snowLayer[0][0], Dark, 8 * 8 * sizeof(byte));
    memset(&starLayer[0][0], Dark, 8 * 8 * sizeof(byte));
  }

  void drawSnowPixel(int x, int y, byte color)
  {
    snowLayer[y][x] = color;
  }

  // Copy all lines in the snow layer one down, leaving the top line blank.
  void scrollSnowDown()
  {
    // Start on the last line and end at the 2nds to first line.
    for (int y = 0; y < 7; ++y)
    {
      memcpy(&snowLayer[y], &snowLayer[y+1], 8 * sizeof(byte));
    }

    // Blank top line.
    for (int x = 0; x < 8; ++x)
    {
      snowLayer[7][x] = Dark;
    }
  }
  
  void drawStarPixel(int x, int y, byte color)
  {
    starLayer[y][x] = color;
  }
  
  // Repaint the entire display.
  void scheduleRepaintAll()
  {
    // Mark all for repaint (clear single pixel repaint if present).
    repaintState = RepaintAll;
    drawX = 0;
    drawX = 0;
    
    // Schedule the repaint.
    awake();
  }
  
  // Repaint pixel.
  void scheduleRepaintPixel(int x, int y)
  {
    switch (repaintState)
    {
    // Ignore this request if a repaint all was already scheduled.
    case RepaintAll:
      break;
    
    // Check if a pixel was already set for repaint.
    case RepaintPixel:
      // We can only store one pixel, so convert to a repaint all operation.
      scheduleRepaintAll();
      break;

    // No repaint operation scheduled yet.
    case NoRepaint:
      repaintState = RepaintPixel;
      drawX = x;
      drawY = y;
      
      // Schedule the repaint.
      awake();
    }
  }
  
  virtual int worker()
  {
    switch (repaintState)
    {
    case RepaintPixel:
      {
        // Only repaint one pixel.
        drawPixel(drawX,drawY);
      }
      break;
    
    case RepaintAll:
      {
        // Clear entire display slate first, then redraw.
        ClearSlate();

        // Draw the information in the two layers.
        // Draw snow in front of stars.
        for (int y = 0; y < 8; ++y)
        {
          // Go through all pixels of both layers.
          for (int x = 0; x < 8; ++x)
          {
            drawPixel(x,y);
          }
        }
      }
      break;
    
    case NoRepaint:
      // Should not happen.
      break;
    }

    // Clear repaint info.
    repaintState = NoRepaint;
    drawX = 0;
    drawY = 0;

    // Update the screen.
    DisplaySlate();
    
    // Suspend this co-routine and wait to be awakened again.
    return -1;
  }
};

