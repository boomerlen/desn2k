#include "lpc24xx.h"
#include "songs.h"

extern struct tone song_data[];
extern int song_duration;
extern void udelay(unsigned int delay_in_us);
extern void play_tone(unsigned int duration, int period,  int vol);
extern void setup_DAC(void);

int main(void) {

	// Sets the speed at which the song plays.
	int rate = 52000; 
	
	int i = 0;

	// Setup the DAC 
	setup_DAC();
	
	// Play the song
	while(1) {
		if (i > song_duration) { 		// End of the song has been reached
			i = 58; 									// Skip the intro and play the main melody again.
		} else { 										// Play the note
			play_tone(rate * song_data[i].duration, song_data[i].pitch, song_data[i].volume);
			i++;
		}
	}
}
