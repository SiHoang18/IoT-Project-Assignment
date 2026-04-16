#ifndef TINYML_TASK_H
#define TINYML_TASK_H

#include <Arduino.h>

void initTinyML();
void inferenceTask(void *pvParameters);

#endif