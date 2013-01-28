/*
* ae.h
* evaluate arithmetic expressions at run time
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 06 May 2010 23:45:53
* This code is hereby placed in the public domain.
*/

// Taken from http://www.tecgraf.puc-rio.br/~lhf/ftp/lua/#ae

extern "C" {

void		ae_open		(void);
void		ae_close	(void);
double		ae_set		(const char* name, double value);
double		ae_eval		(const char* expression);
const char*	ae_error	(void);

} // extern "C"

/* reference manual

  ae_open()
	Opens ae to be used. Call it once before calling the others.
	Does nothing if ae is already open.

  ae_close()
	Closes ae after use. All variables are deleted.
	Does nothing if ae is already closed.

  ae_set(name,value)
	Sets the value of a variable.
	The value persists until it is set again or ae is closed.

  ae_eval(expression)
	Evaluates the given expression and returns its value.
	Once ae has seen an expression, ae can evaluate it repeatedly quickly.
	Returns 0 if there is an error.  ae_error returns the error message.

  ae_error()
	Returns the last error message or NULL if there is none.
*/
