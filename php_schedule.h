#ifndef PHP_SCHEDULE_H
#define PHP_SCHEDULE_H

#define PHP_SCHEDULE_EXTNAME  "schedule"
#define PHP_SCHEDULE_EXTVER   "4.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif 

extern "C" {
#include "php.h"
}
#define Z_CUSTOM_OBJ_P(zv) php_custom_object_fetch_object(Z_OBJ_P(zv));
extern zend_module_entry schedule_module_entry;
#define phpext_schedule_ptr &schedule_module_entry;

#endif /* PHP_SCHEDULE_H */

