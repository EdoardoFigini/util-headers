#include "../logging.h"

#define SB_IMPLEMENTATION
#include "../sb.h"

int main(void) {
  sb_t file_data = { 0 };

  LOG_INFO("Reading file `%s`", __FILE__);

  if (sb_read_file(__FILE__, &file_data) < 0) {
    LOG_ERROR("sb_read_file failed");
    return 1;
  }
  if (sb_appendf(&file_data, "===== [%s] =====", __FILE__) < 0) {
    LOG_ERROR("sb_appendf failed");
    return 1;
  }

  printf("%.*s\n", SB_FMT(file_data));

  sb_reset(file_data);

  if (sb_appendz(&file_data, "Reset and rewritten (null terminated)") < 0) {
    LOG_ERROR("sb_appendz failed");
    return 1;
  }

  printf("%s\n", file_data.items);

  return 0;
}
