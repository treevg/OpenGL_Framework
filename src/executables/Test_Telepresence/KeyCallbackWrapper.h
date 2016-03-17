class GLFWwindow;

class KeyCallbackWrapper
{
public:
	// technically setEventHandling should be finalized so that it doesn't
	// get overwritten by a descendant class.
	virtual void setEventHandling() { event_handling_instance = this; }

protected:
    virtual void keycallback(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mods) = 0;

	static KeyCallbackWrapper *event_handling_instance;

	static void keycallback_dispatch(
		GLFWwindow *window,
		int key,
		int scancode,
		int action,
		int mods)
	{
		if (event_handling_instance)
			event_handling_instance->keycallback(window, key, scancode, action, mods);
	}
     
};