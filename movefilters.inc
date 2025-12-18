/*
 * Copyright (C) 1998-2003 Gary Wong <gtw@gnu.org>
 * Copyright (C) 1999-2013 the AUTHORS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* Default movefilters */

#define MOVEFILTER_TINY	\
  { { { .Accept = 0, .Extra = 5, .Threshold = 0.08f }, { .Accept = 0, .Extra = 0 }, { .Accept = 0, .Extra = 0                     }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 5, .Threshold = 0.08f }, { .Accept = -1            }, { .Accept = 0, .Extra = 0                     }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 5, .Threshold = 0.08f }, { .Accept = -1            }, { .Accept = 0, .Extra = 2, .Threshold = 0.02f }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 5, .Threshold = 0.08f }, { .Accept = -1            }, { .Accept = 0, .Extra = 2, .Threshold = 0.02f }, { .Accept = -1            } } }

#define MOVEFILTER_NARROW \
  { { { .Accept = 0, .Extra = 8, .Threshold = 0.12f }, { .Accept = 0, .Extra = 0 }, { .Accept = 0, .Extra = 0                     }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 8, .Threshold = 0.12f }, { .Accept = -1            }, { .Accept = 0, .Extra = 0                     }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 8, .Threshold = 0.12f }, { .Accept = -1            }, { .Accept = 0, .Extra = 2, .Threshold = 0.03f }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 8, .Threshold = 0.12f }, { .Accept = -1            }, { .Accept = 0, .Extra = 2, .Threshold = 0.03f }, { .Accept = -1            } } }

#define MOVEFILTER_NORMAL \
  { { { .Accept = 0, .Extra = 8, .Threshold = 0.16f }, { .Accept = 0, .Extra = 0 }, { .Accept = 0, .Extra = 0                     }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 8, .Threshold = 0.16f }, { .Accept = -1            }, { .Accept = 0, .Extra = 0                     }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 8, .Threshold = 0.16f }, { .Accept = -1            }, { .Accept = 0, .Extra = 2, .Threshold = 0.04f }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 8, .Threshold = 0.16f }, { .Accept = -1            }, { .Accept = 0, .Extra = 2, .Threshold = 0.04f }, { .Accept = -1            } } }

#define MOVEFILTER_LARGE \
  { { { .Accept = 0, .Extra = 16, .Threshold = 0.32f }, { .Accept = 0, .Extra = 0 }, { .Accept = 0, .Extra = 0                     }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 16, .Threshold = 0.32f }, { .Accept = -1            }, { .Accept = 0, .Extra = 0                     }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 16, .Threshold = 0.32f }, { .Accept = -1            }, { .Accept = 0, .Extra = 4, .Threshold = 0.08f }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 16, .Threshold = 0.32f }, { .Accept = -1            }, { .Accept = 0, .Extra = 4, .Threshold = 0.08f }, { .Accept = -1            } } }

#define MOVEFILTER_HUGE	\
  { { { .Accept = 0, .Extra = 20, .Threshold = 0.44f }, { .Accept = 0, .Extra = 0 }, { .Accept = 0, .Extra = 0                     }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 20, .Threshold = 0.44f }, { .Accept = -1            }, { .Accept = 0, .Extra = 0                     }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 20, .Threshold = 0.44f }, { .Accept = -1            }, { .Accept = 0, .Extra = 6, .Threshold = 0.11f }, { .Accept = 0, .Extra = 0 } }, \
    { { .Accept = 0, .Extra = 20, .Threshold = 0.44f }, { .Accept = -1            }, { .Accept = 0, .Extra = 6, .Threshold = 0.11f }, { .Accept = -1            } } }

