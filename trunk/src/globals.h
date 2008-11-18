 /*
 				globals.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	MissFITS
*
*	Author:		E.BERTIN (IAP) C. MARMO
*
*	Contents:	Global declarations.
*
*	Last modify:	06/08/2007
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include	"types.h"

/*----------------------- miscellaneous variables ---------------------------*/
char		gstr[MAXCHAR];

/*------------------------------- functions ---------------------------------*/
extern	void	makeit(void),
                write_error(char *msg1, char *msg2);
