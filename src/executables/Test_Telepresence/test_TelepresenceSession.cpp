#include "TelepresenceSession.h"


int main() {
	TelepresenceSession session = TelepresenceSession();
	session.init();
	session.setEventHandling();
	session.run();
}
