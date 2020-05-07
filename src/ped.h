void output(char*, unsigned int);
struct Session *newSession(char*);
void newEditor();

void startTTY();
void stopTTY();

char getChar();

void loadPlugins(struct Editor*);