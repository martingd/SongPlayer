/*
  Simple song player library for Meggy Jr RGB.
  
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

/*
 Uncomment this define to include debug messages to the serial port.
 You must remember to call
   Serial.begin(9600);
 in your 'void setup()' function.
 */
//#define SongPlayerDebug

#include "SongPlayer.h"

#ifdef SongPlayerDebug
#include <WProgram.h>
#endif

namespace songplayer
{
  SongPlayer::SongPlayer(const Note* song, MeggyJr& meggyJr, bool autoRepeat, int internoteDelay)
    : songSegments(soleSegment), // Point to the soleSegment array.
      currentSegment(songSegments),
      nextNote(song), // Point directly to first note of song, as soleSegment array is not yet initialized.
      meggyJr(meggyJr),
      autoRepeat(autoRepeat),
      internoteDelay(internoteDelay)
  {
    soleSegment[0] = song;
    soleSegment[1] = 0; // End of segment.
  }

  SongPlayer::SongPlayer(const Note** songSegments, MeggyJr& meggyJr, bool autoRepeat, int internoteDelay)
    : songSegments(songSegments),
      currentSegment(songSegments),
      nextNote(songSegments[0]),
      meggyJr(meggyJr),
      autoRepeat(autoRepeat),
      internoteDelay(internoteDelay)
  {
    // No sole segment, this is a multi-segment song.
    // Initialize it anyway to avoid uninitialized values.
    soleSegment[0] = 0;
    soleSegment[1] = 0;
  }

  void SongPlayer::rewind()
  {
    // Rewind to the beginning of the song.
    currentSegment = songSegments;
    nextNote = currentSegment[0];
  }

  int SongPlayer::worker() {
    #ifdef SongPlayerDebug
      Serial.println("SongPlayer::worker()");
    #endif
    
    // Check if we reached the end of a segment?
    if (nextNote->duration == 0 && nextNote->pitch == 0)
    {
      #ifdef SongPlayerDebug
        Serial.println("Got Null note");
      #endif

      // Go to the next segment.
      ++currentSegment;
      
      // End of segments?
      if (*currentSegment != 0)
      {
        #ifdef SongPlayerDebug
          Serial.println("Not last segment");
        #endif

        // Not the end. Start playing this segment.
        nextNote = currentSegment[0];
      }
      else
      {
        #ifdef SongPlayerDebug
          Serial.println("Last segment");
        #endif

        // No more segments. Rewind the song.
        #ifdef SongPlayerDebug
          Serial.println("Rewind song");
        #endif

        rewind();

        if (!autoRepeat)
        {
          #ifdef SongPlayerDebug
            Serial.println("Stop playing (no auto-repeat)");
          #endif

          // Stop playing by suspending the co-routine.
          return -1;
        }
      }
    }
    
    // If we are still here, there is something to play.
    // Get the note and advance to the next one.
    const Note* const note = nextNote++;

    // Play note (or do nothing if it is a pause).
    const int pitch = note->pitch;
    if (pitch != 0)
    {
      #ifdef SongPlayerDebug
        Serial.print("Note: ");
        Serial.print(note->pitch);
        Serial.print(", ");
        Serial.println(note->duration);
      #else
        meggyJr.StartTone(note->pitch, note->duration - internoteDelay);
      #endif
    }
    else
    {
      #ifdef SongPlayerDebug
        Serial.print("Pause: ");
        Serial.println(note->duration);
      #endif
    }
    
    // Come back here, when note is done.
    return note->duration;
  }

} // end of namespace songplayer
