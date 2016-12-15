#include "detectedBlob.hpp"

DetectedBlob::DetectedBlob(const Composante& pcomp, bool pestimated)
: comp(pcomp), estimated(pestimated), 
note(0.0)
 {}

float DetectedBlob::computeNote(float noteSurface, float noteProportion, float noteDistance){
	return noteSurface * COEF_NOTE_SURFACE + noteProportion * COEF_NOTE_PROPORTION + noteDistance * COEF_NOTE_DISTANCE;
}
