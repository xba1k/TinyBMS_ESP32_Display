#include <Arduino.h>
#include "util.h"

int freeMemory() {
	return ESP.getFreeHeap();
}