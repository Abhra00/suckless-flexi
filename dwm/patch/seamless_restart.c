void
persistmonitorstate(Monitor *m)
{
	Client *c;
	unsigned int i;

	setmonitortags(m);
	setmonitorfields(m);

	/* Set client atoms */
	for (i = 1, c = m->clients; c; c = c->next, ++i) {
		c->idx = i;
		persistclientstate(c);
		if (c->swallowing) {
			c->swallowing->idx = i;
			persistclientstate(c->swallowing);
		}
	}
}

int
restoremonitorstate(Monitor *m)
{
	return getmonitortags(m) | getmonitorfields(m);
}

void
persistclientstate(Client *c)
{
	setclienttags(c);
	setclientfields(c);
}

int
restoreclientstate(Client *c)
{
	int restored = getclientfields(c);
	getclienttags(c);
	return restored;
}

void setmonitorfields(Monitor *m)
{
	unsigned int i;
	char atom[22] = {0};
	Atom monitor_fields;
	unsigned int flextile_deluxe_bitmask;

	sprintf(atom, "_DWM_MONITOR_FIELDS_%u", m->num);
	monitor_fields = XInternAtom(dpy, atom, False);

	/* Perists workspace information in 32 bits laid out like this:
	 *
	 * |0|0000|0|0000|0000|0000|0000|0000|000|000
	 * | |    | |    |    |    |    |    |   |-- nmaster
	 * | |    | |    |    |    |    |    |-- nstack
	 * | |    | |    |    |    |    |-- layout
	 * | |    | |    |    |    |-- flextile LAYOUT (split)
	 * | |    | |    |    |-- flextile MASTER
	 * | |    | |    |-- flextile STACK1
	 * | |    | |-- flextile STACK2
	 * | |    |-- flextile mirror layout (indicated by negative layout)
	 * | |
	 * | |-- reserved
	 * |-- showbar
	 */
	for (i = 0; i <= NUMTAGS; i++) {
		flextile_deluxe_bitmask = (m->pertag->nstacks[i] & 0x7) << 3;
		if (m->pertag->ltidxs[i][m->pertag->sellts[i]]->arrange == flextile) {
			flextile_deluxe_bitmask |=
				(abs(m->pertag->ltaxis[i][LAYOUT]) & 0xF) << 10 |
				(m->pertag->ltaxis[i][MASTER] & 0xF) << 14 |
				(m->pertag->ltaxis[i][STACK]  & 0xF) << 18 |
				(m->pertag->ltaxis[i][STACK2] & 0xF) << 22 |
				(m->pertag->ltaxis[i][LAYOUT] < 0 ? 1 : 0) << 24;
		}
		uint32_t data[] = {
			flextile_deluxe_bitmask |
			(m->pertag->nmasters[i] & 0x7) |
			(getlayoutindex(m->pertag->ltidxs[i][m->pertag->sellts[i]]) & 0xF) << 6 |
			m->pertag->showbars[i] << 31
		};

		XChangeProperty(dpy, root, monitor_fields, XA_CARDINAL, 32,
			i ? PropModeAppend : PropModeReplace, (unsigned char *)data, 1);
	}
}

int
getlayoutindex(const Layout *layout)
{
	int i;

	for (i = 0; i < LENGTH(layouts) && &layouts[i] != layout; i++);
	if (i == LENGTH(layouts))
		i = 0;
	return i;
}

int
getmonitorfields(Monitor *m)
{
	int di, layout_index;
	unsigned int i, restored = 0;
	unsigned int tags = m->tagset[m->seltags] << 1;
	unsigned long dl, nitems;
	unsigned char *p = NULL;
	char atom[22] = {0};
	Atom da, state = None;

	sprintf(atom, "_DWM_MONITOR_FIELDS_%u", m->num);
	Atom dwm_monitor = XInternAtom(dpy, atom, False);
	if (!dwm_monitor)
		return 0;

	for (i = 0; i <= NUMTAGS; i++) {
		if (!(XGetWindowProperty(dpy, root, dwm_monitor, i, (NUMTAGS + 1) * sizeof dl,
				False, AnyPropertyType, &da, &di, &nitems, &dl, &p) == Success && p)) {
			break;
		}

		if (!nitems) {
			XFree(p);
			break;
		}

		/* See bit layout in the persistmonitorstate function */
		state = *(Atom *)p;

		m->pertag->nmasters[i] = state & 0x7;
		layout_index = (state >> 6) & 0xF;
		if (layout_index < LENGTH(layouts))
			m->pertag->ltidxs[i][m->pertag->sellts[i]] = &layouts[layout_index];
		m->pertag->nstacks[i] = (state >> 3) & 0x7;
		if (m->pertag->ltidxs[i][m->pertag->sellts[i]]->arrange == flextile) {
			m->pertag->ltaxis[i][LAYOUT] = (state >> 10) & 0xF;
			m->pertag->ltaxis[i][MASTER] = (state >> 14) & 0xF;
			m->pertag->ltaxis[i][STACK]  = (state >> 18) & 0xF;
			m->pertag->ltaxis[i][STACK2] = (state >> 22) & 0xF;
			if (state >> 24 & 0x1) {
				m->pertag->ltaxis[i][LAYOUT] *= -1;
			}
		}
		m->pertag->showbars[i] = (state >> 31) & 0x1;

		if (!restored && i && (tags & (1 << i))) {
			m->nmaster = m->pertag->nmasters[i];
			m->sellt = m->pertag->sellts[i];
			m->lt[m->sellt] = m->pertag->ltidxs[i][m->sellt];
			m->nstack = m->pertag->nstacks[i];
			if (m->lt[m->sellt]->arrange == flextile) {
				m->ltaxis[LAYOUT] = m->pertag->ltaxis[i][LAYOUT];
				m->ltaxis[MASTER] = m->pertag->ltaxis[i][MASTER];
				m->ltaxis[STACK]  = m->pertag->ltaxis[i][STACK];
				m->ltaxis[STACK2] = m->pertag->ltaxis[i][STACK2];
			}
			m->showbar = m->pertag->showbars[i];
			restored = 1;
		}

		XFree(p);
	}

	return restored;
}

void
setmonitortags(Monitor *m)
{
	char atom[22] = {0};
	Atom monitor_tags;

	sprintf(atom, "_DWM_MONITOR_TAGS_%u", m->num);
	monitor_tags = XInternAtom(dpy, atom, False);

	uint32_t data[] = { m->tagset[m->seltags] };
	XChangeProperty(dpy, root, monitor_tags, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}

int
getmonitortags(Monitor *m)
{
	int di;
	unsigned long dl, nitems;
	unsigned char *p = NULL;
	char atom[22] = {0};
	Atom da, monitor_tags = None, tags;

	sprintf(atom, "_DWM_MONITOR_TAGS_%u", m->num);
	monitor_tags = XInternAtom(dpy, atom, False);

	if (!(XGetWindowProperty(dpy, root, monitor_tags, 0L, sizeof dl,
			False, AnyPropertyType, &da, &di, &nitems, &dl, &p) == Success && p)) {
		return 0;
	}

	if (nitems) {
		tags = *(Atom *)p;
		m->tagset[m->seltags] = tags & TAGMASK;
	}

	XFree(p);
	return 1;
}

void
setclientfields(Client *c)
{
	/* Perists client information in 32 bits laid out like this:
	 *
	 * |00000000|00000|0|0|0|0|0|0|0|0|00000000|000
	 * |        |     | | | | | | | | |        |-- monitor index
	 * |        |     | | | | | | | | |-- client index
	 * |        |     | | | | | | | |-- isfloating
	 * |        |     | | | | | | |-- ispermanent
	 * |        |     | | | | | |-- isterminal
	 * |        |     | | | | |-- noswallow
	 * |        |     | | | |-- issteam
	 * |        |     | | |-- issticky
	 * |        |     | |-- fakefullscreen
	 * |        |     |-- isfreesize
	 * |        |
	 * |        |-- reserved
	 * |-- scratchkey (for scratchpads)
	 */
	uint32_t data[] = {
		(c->mon->num & 0x7)
		| (c->idx & 0xFF) << 3
		| (c->isfloating & 0x1) << 11
		| (c->isterminal & 0x1) << 13
		| (c->noswallow & 0x1) << 14
		| (c->issticky & 0x1) << 16
		| (c->scratchkey & 0xFF) << 24
	};
	XChangeProperty(dpy, c->win, clientatom[ClientFields], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}

int
getclientfields(Client *c)
{
	Monitor *m;
	Atom fields = getatomprop(c, clientatom[ClientFields], AnyPropertyType);
	if (fields == None)
		return 0;

	/* See bit layout in the setclientfields function */
	for (m = mons; m; m = m->next)
		if (m->num == (fields & 0x7)) {
			c->mon = m;
			break;
		}
	c->idx = (fields >> 3) & 0xFF;
	c->isfloating = (fields >> 11) & 0x1;
	c->isterminal = (fields >> 13) & 0x1;
	c->noswallow = (fields >> 14) & 0x1;
	c->issticky = (fields >> 16) & 0x1;
	c->scratchkey = (fields >> 24) & 0xFF;
	return 1;
}

void
setclienttags(Client *c)
{
	uint32_t data[] = { c->tags };
	XChangeProperty(dpy, c->win, clientatom[ClientTags], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}

int
getclienttags(Client *c)
{
	Atom tags = getatomprop(c, clientatom[ClientTags], AnyPropertyType);
	if (tags == None)
		return 0;

	c->tags = tags & TAGMASK;
	return 1;
}

