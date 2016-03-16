#include "KeyCallbackWrapper.h"
// funny thing that you have to omit `static` here. Learn about global scope
// type qualifiers to understand why.
KeyCallbackWrapper * KeyCallbackWrapper::event_handling_instance;