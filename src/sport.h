#ifndef __SPORT_H__
#define __SPORT_H__

#include "stypes.h"

#define PORT_FLAG_INPUT (1 << 0)
#define PORT_FLAG_INPUT_OPEN (1 << 1)
#define PORT_FLAG_OUTPUT (1 << 2)
#define PORT_FLAG_OUTPUT_OPEN (1 << 3)
#define PORT_FLAG_TYPE (1 << 4)
#define PORT_FLAG_TYPE_FILE (1 << 5)

#define port_is_input(p) ((p)->flags & PORT_FLAG_INPUT)
#define port_is_input_open(p) ((p)->flags & PORT_FLAG_INPUT_OPEN)
#define port_is_input_closed(p) (!port_is_input_open(p))
#define port_is_output(p) ((p)->flags & PORT_FLAG_OUTPUT)
#define port_is_output_open(p) ((p)->flags & PORT_FLAG_OUTPUT_OPEN)
#define port_is_output_closed(p) (!port_is_output_open(p))
#define port_is_text(p) ((p)->flags & PORT_FLAG_TYPE)
#define port_is_binary(p) (!port_is_text(p))
#define port_is_open(p) (port_is_input_open(p) || port_is_output_open(p))
#define port_is_closed(p) (!port_is_open(p))

struct port* open_input_file(struct state*, const char *filename, bool binary);
struct port* open_output_file(struct state*, const char *filename, bool binary);

struct port* open_input_string(struct state*, const char *data, size_t length);
struct port* open_output_string(struct state*);

#endif
