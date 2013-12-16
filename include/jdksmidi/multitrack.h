
#ifndef JDKSMIDI_MULTITRACK_H
#define JDKSMIDI_MULTITRACK_H

#include "jdksmidi/track.h"

namespace jdksmidi
{

class MIDIMultiTrack;
class MIDIMultiTrackIteratorState;
class MIDIMultiTrackIterator;

class MIDIMultiTrack
{
private:

    // delete old multitrack, construct new
    bool CreateObject ( int num_tracks_, bool deletable_ );

public:

    MIDIMultiTrack ( int max_num_tracks_ = 64, bool deletable_ = true );
    virtual ~MIDIMultiTrack();

    void SetTrack ( int track_num, MIDITrack *track )
    {
        tracks[track_num] = track;
    }

    MIDITrack *GetTrack ( int track_num )
    {
        assert( track_num < number_of_tracks );
        return tracks[track_num];
    }
    const MIDITrack *GetTrack ( int track_num ) const
    {
        assert( track_num < number_of_tracks );
        return tracks[track_num];
    }

    int GetNumTracks() const
    {
        return number_of_tracks;
    }

    // return number of tracks with events, last tracks have no events
    int GetNumTracksWithEvents() const;

    // test and sort events temporal order in all tracks
    void SortEventsOrder();

    // delete all tracks and remake multitrack with new amount of empty tracks
    bool ClearAndResize ( int num_tracks );

    // store src track and remake multitrack object with 17 tracks (src track can be a member of multitrack obiect),
    // move src track channel events to tracks 1-16, and all other types of events to track 0
    bool AssignEventsToTracks ( const MIDITrack *src );

    // the same as previous, but argument is track number of multitrack object himself
    bool AssignEventsToTracks ( int track_num = 0 )
    {
        return AssignEventsToTracks( GetTrack( track_num ) );
    }

    void Clear();

    int GetClksPerBeat() const
    {
        return clks_per_beat;
    }
    void SetClksPerBeat ( int cpb )
    {
        clks_per_beat = cpb;
    }

    int GetNumEvents() const
    {
        int num_events = 0;
        for ( int i = 0; i < number_of_tracks; ++i )
            num_events += tracks[i]->GetNumEvents();
        return num_events;
    }

protected:

    MIDITrack **tracks;
    int number_of_tracks;
    bool deletable;

    int clks_per_beat;
};

class MIDIMultiTrackIteratorState
{
public:

    MIDIMultiTrackIteratorState ( int num_tracks_ = 64 );
    MIDIMultiTrackIteratorState ( const MIDIMultiTrackIteratorState &m );
    virtual ~MIDIMultiTrackIteratorState();

    const MIDIMultiTrackIteratorState & operator = ( const MIDIMultiTrackIteratorState &m );

    int GetNumTracks() const
    {
        return num_tracks;
    }
    int GetCurEventTrack() const
    {
        return cur_event_track;
    }
    MIDIClockTime GetCurrentTime() const
    {
        return cur_time;
    }

    void Reset();
    int FindTrackOfFirstEvent();

    MIDIClockTime cur_time;
    int cur_event_track;
    int num_tracks;
    int *next_event_number;
    MIDIClockTime *next_event_time;
};

class MIDIMultiTrackIterator
{
public:

    MIDIMultiTrackIterator ( const MIDIMultiTrack *mlt );
    virtual ~MIDIMultiTrackIterator();


    void GoToTime ( MIDIClockTime time );

    bool GetCurEventTime ( MIDIClockTime *t ) const;
    bool GetCurEvent ( int *track, const MIDITimedBigMessage **msg ) const;
    bool GoToNextEvent();

    bool GoToNextEventOnTrack ( int track );

    const MIDIMultiTrackIteratorState &GetState() const
    {
        return state;
    }

    MIDIMultiTrackIteratorState &GetState()
    {
        return state;
    }

    void SetState ( const MIDIMultiTrackIteratorState &s )
    {
        state = s;
    }

    const MIDIMultiTrack * GetMultiTrack() const
    {
        return multitrack;
    }

protected:

    const MIDIMultiTrack *multitrack;
    MIDIMultiTrackIteratorState state;
};

}

#endif


