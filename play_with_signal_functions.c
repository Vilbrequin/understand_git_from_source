/*
 * In order to uderstand the restore_sigpipe_to_default() function 
 * we should understand some signal manipulation fuctions available in the C libray.
 *
 * fist of all what is a signal ? 
 * by the Advanced Programming in the UNIX Envirement book, Signals are software interrupts.
 *
 * in this code example we will focuse on SIGPIPE signal, that is raised when we try to write
 * to pipe with no readers, something like "yes | head -n 1".
 *
 * when a signal occurs we should tell the kernel to do something i.e an action
 *
 * some program may block some signals for thier own reasons, so the kernel is preventing the
 * dilevery of that blocked signal to the process, some other program may change the way a signal
 * is hanled, instead of its default handler defined by the kernel, a specific handler is attached.
 *
 * the git function restore_sigpipe_to_default(), has the role to unblock the SIGPIPE  signal and 
 * restor the default behaviour via the SIG_DFL.
 */

#include <signal.h>
#include <stdio.h> // for perror


int main(void) {
	sigset_t set; // unsigned long sig[1] 

	// first we will initialize the set with sigemtyset api 
	if(sigemptyset(&set) == -1){
		perror("sigemtyset");
		return 1;
	}

	// the we need to add the SIGPIPE signal to the newlly created signal set
	if (sigaddset(&set, SIGPIPE) == -1){
		perror("sigaddset");
		return 1;
	}

	// now we should unblock the SIGPIPE if already blocked, if not nothing happen
	if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1){
		perror("sigprocmask");
		return 1;
	}

	// at this stage we are sure that the SIGPIPE signal is not blocking by the kernel
	// now we shall restore its default behaviour
	if(signal(SIGPIPE, SIG_DFL) == SIG_ERR) {
		perror("signal");
		return 1;
	}
	return 0;
}
