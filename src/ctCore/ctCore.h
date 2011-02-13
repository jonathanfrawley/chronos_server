#ifndef  CTCORE_H
#define  CTCORE_H

#define CT_NEW new
#define CT_SAFE_DELETE(x) if(x) delete x; x=NULL;
#define CT_SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL; 

#endif   // CTCORE_H
