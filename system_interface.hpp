#ifndef SYSTEM_INTERFACE_HPP
#define SYSTEM_INTERFACE_HPP

enum ScreenMode {
	RuntimeMode,
	EditorMode
};

class SystemInterface {
	public:
		virtual bool init(int argc, char *argv[]) = 0;
		virtual void run() = 0;
		virtual ~SystemInterface() {}
};

#endif