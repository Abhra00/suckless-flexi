#include <X11/Xresource.h>

void
readxresources(void)
{
	XrmInitialize();

	char* xrm;
	if ((xrm = XResourceManagerString(drw->dpy))) {
		char *type;
		XrmDatabase xdb = XrmGetStringDatabase(xrm);
		XrmValue xval;

		if (XrmGetResource(xdb, "dmenu.font", "*", &type, &xval))
			fonts[0] = strdup(xval.addr);
		else
			fonts[0] = strdup(fonts[0]);
		if (XrmGetResource(xdb, "color0", "*", &type, &xval))
			colors[SchemeNorm][ColBg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "foreground", "*", &type, &xval))
			colors[SchemeNorm][ColFg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "color17", "*", &type, &xval))
			colors[SchemeSel][ColBg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "background", "*", &type, &xval))
			colors[SchemeSel][ColFg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "color0", "*", &type, &xval))
			colors[SchemeOut][ColBg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "foreground", "*", &type, &xval))
			colors[SchemeOut][ColFg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "color17", "*", &type, &xval))
			colors[SchemeBorder][ColBg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "color17", "*", &type, &xval))
			colors[SchemeHp][ColBg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "background", "*", &type, &xval))
			colors[SchemeHp][ColFg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "color2", "*", &type, &xval))
			colors[SchemeGreen][ColBg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "background", "*", &type, &xval))
			colors[SchemeGreen][ColFg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "color4", "*", &type, &xval))
			colors[SchemeYellow][ColBg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "background", "*", &type, &xval))
			colors[SchemeYellow][ColFg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "color5", "*", &type, &xval))
			colors[SchemeBlue][ColBg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "background", "*", &type, &xval))
			colors[SchemeBlue][ColFg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "color6", "*", &type, &xval))
			colors[SchemePurple][ColBg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "background", "*", &type, &xval))
			colors[SchemePurple][ColFg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "color1", "*", &type, &xval))
			colors[SchemeRed][ColBg] = strdup(xval.addr);
		if (XrmGetResource(xdb, "background", "*", &type, &xval))
			colors[SchemeRed][ColFg] = strdup(xval.addr);
		XrmDestroyDatabase(xdb);
	}
}
