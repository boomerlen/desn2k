#include "system.h"

struct system_state state = {
    .crazy = 3
};

int main() {
	
    system_setup();

    enter_mode_away();

    while (1) {
        // Done
    }

	return 0;
}
