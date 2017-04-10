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

class Snow : public CoRoutine
{
private:
  DisplayPainter& displayPainter;

public:
  Snow(DisplayPainter& displayPainter) : displayPainter(displayPainter)
  {  }
  
  virtual int worker()
  {
    // Scroll snow down.
    displayPainter.scrollSnowDown();

    // Make random line of snow at top.
    for (int x = 0; x < 8; ++x)
    {
      if (random(100) < 15) // 15% chance of a flake in every cell.
      {
        // Paint in top row -- i.e. y = 7
        displayPainter.drawSnowPixel(x, 7, White); 
      }
    }
    
    // Schedule a complete redraw.
    displayPainter.scheduleRepaintAll();
   
    // Sleep a while before snow falls down one more row.
    return 1000;
  }
};


