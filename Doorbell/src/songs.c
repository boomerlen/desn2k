#define OFF		100

#define C4 		3831
#define CS4 	3610
#define D4 		3401
#define DS4 	3215
#define E4 		3030
#define F4 		2857
#define FS4 	2725
#define G4 		2551
#define GS4 	2410
#define A4 		2272
#define AS4 	2146
#define B4 		2024

#define C5 		C4/2
#define CS5 	CS4/2
#define D5 		D4/2
#define DS5 	DS4/2
#define E5 		E4/2
#define F5 		F4/2
#define FS5 	FS4/2
#define G5 		G4/2
#define GS5 	GS4/2
#define A5 		A4/2
#define AS5 	AS4/2
#define B5 		B4/2

#define C6 		C5/2
#define CS6 	CS5/2
#define D6 		D5/2
#define DS6 	DS5/2
#define E6 		E5/2
#define F6 		F5/2
#define FS6 	FS5/2
#define G6 		G5/2
#define GS6 	GS5/2
#define A6 		A5/2
#define AS6 	AS5/2
#define B6 		B5/2

#define C3 		C4*2
#define CS3 	CS4*2
#define D3 		D4*2
#define DS3 	DS4*2
#define E3 		E4*2
#define F3 		F4*2
#define FS3 	FS4*2
#define G3 		G4*2
#define GS3 	GS4*2
#define A3 		A4*2
#define AS3 	AS4*2
#define B3 		B4*2


int song_duration = 28;

struct tone song_data[] = {
    // Doorbell chime (like ding-dong)
    {4, C6, 0x3FF},
    {4, G5, 0x3FF},
    {2, OFF, 0},

    // Alarm-like rising pitch
    {2, C5, 0x300},
    {2, D5, 0x300},
    {2, E5, 0x300},
    {2, F5, 0x300},
    {2, G5, 0x300},
    {2, A5, 0x300},
    {2, B5, 0x300},
    {2, C6, 0x3FF},
    {2, OFF, 0},

    // Buzzing bees (fast flutter between close pitches)
    {1, B5, 0x180},
    {1, C6, 0x180},
    {1, B5, 0x180},
    {1, C6, 0x180},
    {1, B5, 0x180},
    {1, C6, 0x180},
    {1, B5, 0x180},
    {1, C6, 0x180},
    {2, OFF, 0},

    // Fading swarm
    {2, G5, 0x2FF},
    {2, G5, 0x1FF},
    {2, G5, 0x0FF},
    {2, OFF, 0}
};
