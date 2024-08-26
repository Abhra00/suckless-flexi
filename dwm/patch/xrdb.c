void
loadxrdb()
{
	Display *display;
	char * resm;
	XrmDatabase xrdb;
	char *type;
	XrmValue value;

	display = XOpenDisplay(NULL);

	if (display != NULL) {
		resm = XResourceManagerString(display);

		if (resm != NULL) {
			xrdb = XrmGetStringDatabase(resm);

			if (xrdb != NULL) {
				XRDB_LOAD_COLOR("foreground", normfgcolor);
				XRDB_LOAD_COLOR("background", normbgcolor);
				XRDB_LOAD_COLOR("color0",     normbordercolor);
				XRDB_LOAD_COLOR("color0",     normfloatcolor);
				XRDB_LOAD_COLOR("background", selfgcolor);
				XRDB_LOAD_COLOR("color17",    selbgcolor);
				XRDB_LOAD_COLOR("color17",    selbordercolor);
				XRDB_LOAD_COLOR("color1",     selfloatcolor);
				XRDB_LOAD_COLOR("foreground", titlenormfgcolor);
				XRDB_LOAD_COLOR("background", titlenormbgcolor);
				XRDB_LOAD_COLOR("background", titlenormbordercolor);
				XRDB_LOAD_COLOR("background", titlenormfloatcolor);
				XRDB_LOAD_COLOR("background", titleselfgcolor);
				XRDB_LOAD_COLOR("color17",    titleselbgcolor);
				XRDB_LOAD_COLOR("color17",    titleselbordercolor);
				XRDB_LOAD_COLOR("color17",    titleselfloatcolor);
				XRDB_LOAD_COLOR("foreground", tagsnormfgcolor);
				XRDB_LOAD_COLOR("background", tagsnormbgcolor);
				XRDB_LOAD_COLOR("background", tagsnormbordercolor);
				XRDB_LOAD_COLOR("background", tagsnormfloatcolor);
				XRDB_LOAD_COLOR("background", tagsselfgcolor);
				XRDB_LOAD_COLOR("color17",    tagsselbgcolor);
				XRDB_LOAD_COLOR("color17",    tagsselbordercolor);
				XRDB_LOAD_COLOR("color17",    tagsselfloatcolor);
				XRDB_LOAD_COLOR("foreground", hidnormfgcolor);
				XRDB_LOAD_COLOR("background", hidnormbgcolor);
				XRDB_LOAD_COLOR("foreground", hidselfgcolor);
				XRDB_LOAD_COLOR("background", hidselbgcolor);
				XRDB_LOAD_COLOR("foreground", urgfgcolor);
				XRDB_LOAD_COLOR("background", urgbgcolor);
				XRDB_LOAD_COLOR("color1",     urgbordercolor);
				XRDB_LOAD_COLOR("color2",     urgfloatcolor);
				XRDB_LOAD_COLOR("foreground", scratchselfgcolor);
				XRDB_LOAD_COLOR("background", scratchselbgcolor);
				XRDB_LOAD_COLOR("color3",     scratchselbordercolor);
				XRDB_LOAD_COLOR("color3",     scratchselfloatcolor);
				XRDB_LOAD_COLOR("foreground", scratchnormfgcolor);
				XRDB_LOAD_COLOR("background", scratchnormbgcolor);
				XRDB_LOAD_COLOR("color0",     scratchnormbordercolor);
				XRDB_LOAD_COLOR("color0",     scratchnormfloatcolor);
				XRDB_LOAD_COLOR("color0",     termcol0);
				XRDB_LOAD_COLOR("color1",     termcol1);
				XRDB_LOAD_COLOR("color2",     termcol2);
				XRDB_LOAD_COLOR("color3",     termcol3);
				XRDB_LOAD_COLOR("color4",     termcol4);
				XRDB_LOAD_COLOR("color5",     termcol5);
				XRDB_LOAD_COLOR("color6",     termcol6);
				XRDB_LOAD_COLOR("color7",     termcol7);
				XRDB_LOAD_COLOR("color8",     termcol8);
				XRDB_LOAD_COLOR("color9",     termcol9);
				XRDB_LOAD_COLOR("color10",    termcol10);
				XRDB_LOAD_COLOR("color11",    termcol11);
				XRDB_LOAD_COLOR("color12",    termcol12);
				XRDB_LOAD_COLOR("color13",    termcol13);
				XRDB_LOAD_COLOR("color14",    termcol14);
				XRDB_LOAD_COLOR("color15",    termcol15);

				XrmDestroyDatabase(xrdb);
			}
		}
	}

	XCloseDisplay(display);
}

void
xrdb(const Arg *arg)
{
	loadxrdb();
	int i;
	for (i = 0; i < LENGTH(colors); i++)
		scheme[i] = drw_scm_create(drw, colors[i],
		alphas[i],
		ColCount
		);
	arrange(NULL);
	focus(NULL);
}
