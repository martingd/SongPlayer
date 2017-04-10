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

  This library provides a simple way to play a song through
  the Meggy Jr RGB's speaker. The song is run as a co-routine
  so you can perform concurrent tasks in other co-routines.
*/

#ifndef __songplayer_h__
#define __songplayer_h__

#include <CoRoutines.h>
#include <MeggyJr.h>

namespace songplayer
{
  const int duration_1_8 = 125;
  const int duration_1_4 = 2 * duration_1_8;
  const int duration_3_8 = 3 * duration_1_8;
  const int duration_1_2 = 4 * duration_1_8;
  const int duration_3_4 = 6 * duration_1_8;
  const int duration_1_1 = 8 * duration_1_8;

  struct Note
  {
    int pitch;
    int duration;
  };
  
  const Note EndOfSongSegment = {0,0};

  class SongPlayer : public coroutines::CoRoutine
  {
  private:
    // The array of song segments to be played.
    // Each segment ends with a EndOfSongSegment note.
    // The array ends with a NULL pointer.
    const Note** const songSegments;

    // Points to the segment of the song currently being played.
    const Note** currentSegment;

    // Points to the next note to play (in the current segment).
    const Note* nextNote;

    // Used for songs consisting of only one segment.
    const Note* soleSegment[2];
  
    // The MeggyJr instance to call when playing a note.
    MeggyJr& meggyJr;

    // Tells whether or not to auto-repeat the song.
    const int autoRepeat;
  
    // The (small) pause in milli seconds between individual notes.
    const int internoteDelay;
  
  public:
    SongPlayer(const Note* song, MeggyJr& meggyJr, bool autoRepeat, int internoteDelay = 20);
    SongPlayer(const Note** songSegments, MeggyJr& meggyJr, bool autoRepeat, int internoteDelay = 20);

    // Rewinds the song to the beginning.
    // (The song will auto-rewind when played to the end, 
    //  but will not restart unless 'autoRepeat' was set in the constructor call.)
    void rewind();
    
  protected:
    virtual int worker();
  };
  
} // end of namespace songplayer


#endif // __songplayer_h__