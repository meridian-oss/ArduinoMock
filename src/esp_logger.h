/**
 * @file esp_logger.h
 * @brief ESP系マイコン用ロギングマクロ定義
 * @details ESP-IDF及びArduino環境向けのログレベル別マクロを提供します
 * @version 0.1.0
 * @date 2025-05-03
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */

#ifndef ESP_LOGGER_H
#define ESP_LOGGER_H

// Log level definitions
#define ARDUHAL_LOG_LEVEL_NONE    0
#define ARDUHAL_LOG_LEVEL_ERROR   1
#define ARDUHAL_LOG_LEVEL_WARN    2
#define ARDUHAL_LOG_LEVEL_INFO    3
#define ARDUHAL_LOG_LEVEL_DEBUG   4
#define ARDUHAL_LOG_LEVEL_VERBOSE 5

// Default log level (can be overridden)
#ifndef ARDUHAL_LOG_LEVEL
#define ARDUHAL_LOG_LEVEL ARDUHAL_LOG_LEVEL_VERBOSE
#endif

// Verbose level logging
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_VERBOSE
#ifndef USE_ESP_IDF_LOG
#define log_v(format, ...)     printf(ARDUHAL_LOG_FORMAT(V, format), ##__VA_ARGS__)
#define isr_log_v(format, ...) printf(ARDUHAL_LOG_FORMAT(V, format), ##__VA_ARGS__)
#define log_buf_v(b, l)          \
  do {                           \
    ARDUHAL_LOG_COLOR_PRINT(V);  \
    log_print_buf(b, l);         \
    ARDUHAL_LOG_COLOR_PRINT_END; \
  } while (0)
#else
#define log_v(format, ...)                                            \
  do {                                                                \
    ESP_LOG_LEVEL_LOCAL(ESP_LOG_VERBOSE, TAG, format, ##__VA_ARGS__); \
  } while (0)
#define isr_log_v(format, ...)                                              \
  do {                                                                      \
    printf(LOG_FORMAT(V, format), esp_log_timestamp(), TAG, ##__VA_ARGS__); \
  } while (0)
#define log_buf_v(b, l)                                 \
  do {                                                  \
    ESP_LOG_BUFFER_HEXDUMP(TAG, b, l, ESP_LOG_VERBOSE); \
  } while (0)
#endif
#else
#define log_v(format, ...) \
  do {                     \
  } while (0)
#define isr_log_v(format, ...) \
  do {                         \
  } while (0)
#define log_buf_v(b, l) \
  do {                  \
  } while (0)
#endif

// Debug level logging
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
#ifndef USE_ESP_IDF_LOG
#define log_d(format, ...)     printf(ARDUHAL_LOG_FORMAT(D, format), ##__VA_ARGS__)
#define isr_log_d(format, ...) printf(ARDUHAL_LOG_FORMAT(D, format), ##__VA_ARGS__)
#define log_buf_d(b, l)          \
  do {                           \
    ARDUHAL_LOG_COLOR_PRINT(D);  \
    log_print_buf(b, l);         \
    ARDUHAL_LOG_COLOR_PRINT_END; \
  } while (0)
#else
#define log_d(format, ...)                                          \
  do {                                                              \
    ESP_LOG_LEVEL_LOCAL(ESP_LOG_DEBUG, TAG, format, ##__VA_ARGS__); \
  } while (0)
#define isr_log_d(format, ...)                                              \
  do {                                                                      \
    printf(LOG_FORMAT(D, format), esp_log_timestamp(), TAG, ##__VA_ARGS__); \
  } while (0)
#define log_buf_d(b, l)                               \
  do {                                                \
    ESP_LOG_BUFFER_HEXDUMP(TAG, b, l, ESP_LOG_DEBUG); \
  } while (0)
#endif
#else
#define log_d(format, ...) \
  do {                     \
  } while (0)
#define isr_log_d(format, ...) \
  do {                         \
  } while (0)
#define log_buf_d(b, l) \
  do {                  \
  } while (0)
#endif

// Info level logging
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
#ifndef USE_ESP_IDF_LOG
#define log_i(format, ...)     printf(ARDUHAL_LOG_FORMAT(I, format), ##__VA_ARGS__)
#define isr_log_i(format, ...) printf(ARDUHAL_LOG_FORMAT(I, format), ##__VA_ARGS__)
#define log_buf_i(b, l)          \
  do {                           \
    ARDUHAL_LOG_COLOR_PRINT(I);  \
    log_print_buf(b, l);         \
    ARDUHAL_LOG_COLOR_PRINT_END; \
  } while (0)
#else
#define log_i(format, ...)                                         \
  do {                                                             \
    ESP_LOG_LEVEL_LOCAL(ESP_LOG_INFO, TAG, format, ##__VA_ARGS__); \
  } while (0)
#define isr_log_i(format, ...)                                              \
  do {                                                                      \
    printf(LOG_FORMAT(I, format), esp_log_timestamp(), TAG, ##__VA_ARGS__); \
  } while (0)
#define log_buf_i(b, l)                              \
  do {                                               \
    ESP_LOG_BUFFER_HEXDUMP(TAG, b, l, ESP_LOG_INFO); \
  } while (0)
#endif
#else
#define log_i(format, ...) \
  do {                     \
  } while (0)
#define isr_log_i(format, ...) \
  do {                         \
  } while (0)
#define log_buf_i(b, l) \
  do {                  \
  } while (0)
#endif

// Warn level logging
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_WARN
#ifndef USE_ESP_IDF_LOG
#define log_w(format, ...)     printf(ARDUHAL_LOG_FORMAT(W, format), ##__VA_ARGS__)
#define isr_log_w(format, ...) printf(ARDUHAL_LOG_FORMAT(W, format), ##__VA_ARGS__)
#define log_buf_w(b, l)          \
  do {                           \
    ARDUHAL_LOG_COLOR_PRINT(W);  \
    log_print_buf(b, l);         \
    ARDUHAL_LOG_COLOR_PRINT_END; \
  } while (0)
#else
#define log_w(format, ...)                                         \
  do {                                                             \
    ESP_LOG_LEVEL_LOCAL(ESP_LOG_WARN, TAG, format, ##__VA_ARGS__); \
  } while (0)
#define isr_log_w(format, ...)                                              \
  do {                                                                      \
    printf(LOG_FORMAT(W, format), esp_log_timestamp(), TAG, ##__VA_ARGS__); \
  } while (0)
#define log_buf_w(b, l)                              \
  do {                                               \
    ESP_LOG_BUFFER_HEXDUMP(TAG, b, l, ESP_LOG_WARN); \
  } while (0)
#endif
#else
#define log_w(format, ...) \
  do {                     \
  } while (0)
#define isr_log_w(format, ...) \
  do {                         \
  } while (0)
#define log_buf_w(b, l) \
  do {                  \
  } while (0)
#endif

// Error level logging
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
#ifndef USE_ESP_IDF_LOG
#define log_e(format, ...)     printf(ARDUHAL_LOG_FORMAT(E, format), ##__VA_ARGS__)
#define isr_log_e(format, ...) printf(ARDUHAL_LOG_FORMAT(E, format), ##__VA_ARGS__)
#define log_buf_e(b, l)          \
  do {                           \
    ARDUHAL_LOG_COLOR_PRINT(E);  \
    log_print_buf(b, l);         \
    ARDUHAL_LOG_COLOR_PRINT_END; \
  } while (0)
#else
#define log_e(format, ...)                                          \
  do {                                                              \
    ESP_LOG_LEVEL_LOCAL(ESP_LOG_ERROR, TAG, format, ##__VA_ARGS__); \
  } while (0)
#define isr_log_e(format, ...)                                              \
  do {                                                                      \
    printf(LOG_FORMAT(E, format), esp_log_timestamp(), TAG, ##__VA_ARGS__); \
  } while (0)
#define log_buf_e(b, l)                               \
  do {                                                \
    ESP_LOG_BUFFER_HEXDUMP(TAG, b, l, ESP_LOG_ERROR); \
  } while (0)
#endif
#else
#define log_e(format, ...) \
  do {                     \
  } while (0)
#define isr_log_e(format, ...) \
  do {                         \
  } while (0)
#define log_buf_e(b, l) \
  do {                  \
  } while (0)
#endif

// None level logging (disabled)
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_NONE
#ifndef USE_ESP_IDF_LOG
#define log_n(format, ...)     printf(ARDUHAL_LOG_FORMAT(E, format), ##__VA_ARGS__)
#define isr_log_n(format, ...) printf(ARDUHAL_LOG_FORMAT(E, format), ##__VA_ARGS__)
#define log_buf_n(b, l)          \
  do {                           \
    ARDUHAL_LOG_COLOR_PRINT(E);  \
    log_print_buf(b, l);         \
    ARDUHAL_LOG_COLOR_PRINT_END; \
  } while (0)
#else
#define log_n(format, ...)                                          \
  do {                                                              \
    ESP_LOG_LEVEL_LOCAL(ESP_LOG_ERROR, TAG, format, ##__VA_ARGS__); \
  } while (0)
#define isr_log_n(format, ...)                                              \
  do {                                                                      \
    printf(LOG_FORMAT(E, format), esp_log_timestamp(), TAG, ##__VA_ARGS__); \
  } while (0)
#define log_buf_n(b, l)                               \
  do {                                                \
    ESP_LOG_BUFFER_HEXDUMP(TAG, b, l, ESP_LOG_ERROR); \
  } while (0)
#endif
#else
#define log_n(format, ...) \
  do {                     \
  } while (0)
#define isr_log_n(format, ...) \
  do {                         \
  } while (0)
#define log_buf_n(b, l) \
  do {                  \
  } while (0)
#endif

#endif // ESP_LOGGER_H
