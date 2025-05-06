#ifndef rosic_MidiNoteEvent_h
#define rosic_MidiNoteEvent_h
#include <vector>

namespace rosic
{

  /**

  This is a class for representing MIDI note-events.

  */

  class MidiNoteEvent
  {

  public:

    //---------------------------------------------------------------------------------------------
    // construction/destruction:

    /** Default constructor. */
    MidiNoteEvent();

    /** Constructor with initializations. */
    MidiNoteEvent(int initKey, int initVel, int initDetune = 0, int initPriority = 0 );

    /** Destructor. */
    ~MidiNoteEvent();

    //---------------------------------------------------------------------------------------------
    // parameter settings:

    /** Sets the key of the note (as MIDI note number between 0...127). */
    void setKey(int newKey);

    /** Sets the velocity of the note (between 0...127). */
    void setVelocity(int newVelocity);

    /** Sets the detuning of the note (in semitones). */
    void setDetune(double newDetune);

    /** Sets the priority of the note. */
    void setPriority(int newPriority);

    //---------------------------------------------------------------------------------------------
    // inquiry:

    /** Returns the key of the note (as MIDI note number between 0...127). */
    int getKey() const { return key; }

    /** Returns the velocity of the note (between 0...127). */
    int getVelocity() const { return vel; }

    /** Returns the detuning of the note (in semitones). */
    double getDetune() const { return detune; }

    /** Returns the priority of the note. */
    int getPriority() const { return priority; }

    //---------------------------------------------------------------------------------------------
    // overloaded operators:

    /** Note events are interpreted as equal if the have the same key. */
    bool operator==(const MidiNoteEvent& note2) const
    {
      if( note2.key == key )
        return true;
      else
        return false;
    }

  protected:

    int    key;       // key of the note in the range 0...127
    int    vel;       // velocity of the note in the range 0...127
    double detune;    // detuning in cents (for microtuning)
    int    priority;  // a priority value

  };


  class MidiNoteList
  {
  public:

      MidiNoteList()
          : notes(128)
      {
      }

      void clear()
      {
          index = -1;
      }

      bool empty() const
      {
          return index < 0;
      }

      MidiNoteEvent& front()
      {
          return index >= 0 ? notes[index] : notes[0];
      }

      void push_front(MidiNoteEvent& m)
      {
          if (index > static_cast<int> (notes.size()) - 2)
              shiftNotesFrom(0);

          ++index;
          notes[index] = m;
      }
      
      void remove(const MidiNoteEvent& m)
      {
          const auto pos = getIndexOf(m);
          shiftNotesFrom(pos);
      }

  private:

      int getIndexOf(const MidiNoteEvent& m)
      {
          for (int i = 0; i <= index; ++i)
              if (notes[i].getKey() == m.getKey())
                  return i;

          return -1;
      }

      void shiftNotesFrom(int start)
      {
          if (start < 0 || start > index)
              return;

          for (int i = start; i < index; ++i)
              notes[i] = notes[i + 1];

          --index;
      }

      std::vector<MidiNoteEvent> notes;
      int index = -1;
  };


} // end namespace rosic

#endif // MidiNoteEvent_h
